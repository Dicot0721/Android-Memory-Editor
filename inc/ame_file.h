/*
 * Copyright (C) 2024  Dicot0721
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

#ifndef __AME_FILE_H__
#define __AME_FILE_H__

#include <fcntl.h>
#include <unistd.h>

#include <string_view>

class FileWrapper {
public:
    FileWrapper(std::string_view file, int oflag) {
        _fd = open(file.data(), oflag);
    }

    FileWrapper(const FileWrapper &) = delete;

    FileWrapper(FileWrapper &&other) noexcept : _fd(other._fd) {
        other._fd = -1;
    };

    FileWrapper &operator=(const FileWrapper &) = delete;

    FileWrapper &operator=(FileWrapper &&other) noexcept {
        if (this != &other) {
            Close();
            _fd = other._fd;
            other._fd = -1;
        }
        return *this;
    }

    ~FileWrapper() noexcept {
        Close();
    }

    [[nodiscard]] bool IsOpen() const noexcept {
        return _fd != -1;
    }

    void Close() noexcept {
        if (IsOpen()) {
            close(_fd);
            _fd = -1;
        }
    }

    /**
     * @brief Read NBYTES into BUF from FD at the given position OFFSET without changing the file pointer.
     * @return The number read.
     * @retval -1 for errors.
     * @retval 0 for EOF.
     */
    ssize_t Pread64(void *buf, size_t nbytes, off64_t offset) const {
        return pread64(_fd, buf, nbytes, offset);
    }

    /**
     * @brief Write N bytes of BUF to FD at the given position OFFSET without changing the file pointer.
     * @return The number written, or -1.
     */
    ssize_t Pwrite64(const void *buf, size_t n, off64_t offset) const {
        return pwrite64(_fd, buf, n, offset);
    }

protected:
    int _fd = -1;
};

#endif // __AME_FILE_H__