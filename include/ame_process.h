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

#ifndef AME_PROCESS_H
#define AME_PROCESS_H

#include <sys/types.h>

#include <functional>
#include <optional>
#include <string_view>

namespace ame {

[[nodiscard]] std::optional<pid_t> FindPidByProcessName(std::string_view processName);

[[nodiscard]] std::optional<bool> IsProcessStopped(pid_t pid);

bool FreezeProcessByPid(pid_t pid);

bool ResumeProcessByPid(pid_t pid);

int DoWithProcessName(std::string_view processName, std::move_only_function<bool(pid_t)> &&operation);

int FreezeProcessByName(std::string_view processName);

int ResumeProcessByName(std::string_view processName);

} // namespace ame

#endif // AME_PROCESS_H
