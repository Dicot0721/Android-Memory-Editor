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

#ifndef AME_LOGGER_H
#define AME_LOGGER_H

#include <chrono>
#include <format>
#include <iostream>
#include <source_location>

namespace ame {

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    SILENT,
};

class Logger {
public:
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger &operator=(Logger &&) = delete;

    [[nodiscard, gnu::visibility("default")]] static Logger &Instance() {
        static Logger logger;
        return logger;
    }

    [[nodiscard]] LogLevel GetLevel() const noexcept {
        return _level;
    }

    void SetLevel(LogLevel level) noexcept {
        _level = level;
    }

    template <typename... Args>
    void Log(std::source_location location, LogLevel level, std::format_string<Args...> format, Args &&...args) {
        if (level < _level) {
            return;
        }

        std::ostream &stream = (level < LogLevel::ERROR) ? std::clog : std::cerr;
        const auto now = std::chrono::system_clock::now();
        const std::string message = std::vformat(format.get(), std::make_format_args(args...));

        // "\033[39m" -> default color
        std::println(stream, "{}[{:%T}][{}] [{}] {}\033[39m", _colorStrs[int(level)], now, _levelCodes[int(level)], location.function_name(), message);
    }

protected:
    Logger() = default;
    ~Logger() = default;

    static constexpr std::string_view _colorStrs[int(LogLevel::SILENT)] = {
        "\033[32m", // green
        "",
        "\033[33m", // yellow
        "\033[31m", // red
    };

    static constexpr char _levelCodes[int(LogLevel::SILENT)] = {'D', 'I', 'W', 'E'};

    LogLevel _level = LogLevel::DEBUG;
};

} // namespace ame

#define LOG_DEBUG(...) ame::Logger::Instance().Log(std::source_location::current(), ame::LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) ame::Logger::Instance().Log(std::source_location::current(), ame::LogLevel::INFO, __VA_ARGS__)
#define LOG_WARN(...) ame::Logger::Instance().Log(std::source_location::current(), ame::LogLevel::WARN, __VA_ARGS__)
#define LOG_ERROR(...) ame::Logger::Instance().Log(std::source_location::current(), ame::LogLevel::ERROR, __VA_ARGS__)

#endif // AME_LOGGER_H
