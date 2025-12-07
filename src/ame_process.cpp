/*
 * Copyright (C) 2024, 2025  Dicot0721
 *
 * This file is part of Android-Memory-Editor.
 *
 * Android-Memory-Editor is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Android-Memory-Editor is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Android-Memory-Editor.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ame_process.h"
#include "ame_logger.h"

#include <dirent.h>
#include <sys/types.h>

#include <cctype>
#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <format>
#include <fstream>
#include <functional>
#include <memory>
#include <optional>
#include <regex>
#include <string>
#include <thread>

namespace ame {

/**
 * @brief Find the PID of a process by its name.
 * @param [in] processName  Process name, which could be package name for Android app.
 * @return The std::optional with value of the PID, or std::nullopt if it could not be found.
 */
std::optional<pid_t> FindPidByProcessName(std::string_view processName) {
    static constexpr char procPath[] = "/proc";
    std::unique_ptr<DIR, decltype(&closedir)> procDir{opendir(procPath), &closedir};
    if (!procDir) {
        LOG_ERROR("Failed to open [{}].", procPath);
        return std::nullopt;
    }

    std::string cmdline;
    for (const dirent *entry; (entry = readdir(procDir.get())) != nullptr;) {
        if (entry->d_type != DT_DIR) {
            continue; // not a directory
        }
        const std::string_view dirname{entry->d_name};
        if (!std::ranges::all_of(dirname, [](char c) { return std::isdigit(c); })) {
            continue;
        }

        const std::string cmdlinePath = std::format("/proc/{}/cmdline", dirname);
        std::ifstream cmdlineFile{cmdlinePath};
        if (!cmdlineFile.is_open()) {
            LOG_ERROR("Failed to open [{}].", cmdlinePath);
            continue;
        }
        std::getline(cmdlineFile, cmdline, '\0');
        if (cmdline == processName) {
            return std::atoi(dirname.data());
        }
    }
    return std::nullopt;
}


std::optional<bool> IsProcessStopped(pid_t pid) {
    const std::string statusPath = std::format("/proc/{}/status", pid);
    std::ifstream statusFile{statusPath};
    if (!statusFile.is_open()) {
        LOG_ERROR("Failed to open [{}].", statusPath);
        return std::nullopt;
    }

    std::smatch matches;
    static const std::regex stateRegex{R"re(^State:\s+(\S+)\s+\((.+)\)$)re"};
    for (std::string line; std::getline(statusFile, line);) {
        if (!std::regex_match(line, matches, stateRegex)) {
            continue;
        }
        const std::string stateCode = matches.str(1);
        if (stateCode == "T") {
            LOG_DEBUG("Process {} is in stopped state.", pid);
            return true;
        }
        const std::string state = matches.str(2);
        LOG_DEBUG("Process {} is not in stopped state: {} ({}).", pid, stateCode, state);
        return false;
    }
    return std::nullopt;
}


bool FreezeProcessByPid(pid_t pid) {
    using namespace std::chrono_literals;

    if (kill(pid, SIGSTOP) == -1) {
        LOG_ERROR("Failed to send SIGSTOP to process {}: {}.", pid, std::strerror(errno));
        return false;
    }
    LOG_DEBUG("Succeeded in sending SIGSTOP to process {}.", pid);

    std::this_thread::sleep_for(10ms); // wait for process status
    const auto isStopped = IsProcessStopped(pid);
    if (!isStopped.has_value()) {
        LOG_ERROR("Failed to get state of process {}.", pid);
        return false;
    }
    if (!*isStopped) {
        LOG_ERROR("Failed to freeze process {}.", pid);
        return false;
    }
    LOG_INFO("Succeeded in freezing process {}.", pid);
    return true;
}


bool ResumeProcessByPid(pid_t pid) {
    using namespace std::chrono_literals;

    if (kill(pid, SIGCONT) == -1) {
        LOG_ERROR("Failed to send SIGCONT to process {}: {}.", pid, std::strerror(errno));
        return false;
    }
    LOG_DEBUG("Succeeded in sending SIGCONT to process {}.", pid);

    std::this_thread::sleep_for(10ms); // wait for process status
    const auto isStopped = IsProcessStopped(pid);
    if (!isStopped.has_value()) {
        LOG_ERROR("Failed to get state of process {}.", pid);
        return false;
    }
    if (*isStopped) {
        LOG_ERROR("Failed to resume process {}.", pid);
        return false;
    }
    LOG_INFO("Succeeded in resuming process {}.", pid);
    return true;
}


/**
 * @retval 0  The operation succeeded.
 * @retval -1  Could not find the process.
 * @retval -2  The operation failed.
 */
int DoWithProcessName(std::string_view processName, std::move_only_function<bool(pid_t)> operation) {
    const auto pidOpt = FindPidByProcessName(processName);
    if (!pidOpt.has_value()) {
        LOG_ERROR("Cannot find process '{}'.", processName);
        return -1;
    }
    return operation(*pidOpt) ? 0 : -2;
}

int FreezeProcessByName(std::string_view processName) {
    return DoWithProcessName(processName, &FreezeProcessByPid);
}

int ResumeProcessByName(std::string_view processName) {
    return DoWithProcessName(processName, &ResumeProcessByPid);
}

} // namespace ame
