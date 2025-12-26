# Android-Memory-Editor

**English** | **[简体中文](./README.zh-CN.md)**

[![language](https://img.shields.io/github/languages/top/Dicot0721/Android-Memory-Editor)]()
[![licence](https://img.shields.io/github/license/Dicot0721/Android-Memory-Editor)
](https://www.gnu.org/licenses/gpl-3.0.html)

A simple memory scanner/editor, based on Linux kernel of Android.


## Function

> Almost all require root.

- Find the process ID of an applicaion according to its package name.

- Suspend/resume a process by its PID.

- Find addresses by a specified value.

- Find addresses by a range of values.

- Find addresses by a specified value and offset.

- Find the beginnings of contiguous addresses by some specified values.

- Write a specified value to specified addresses.

- Writes some specified values to specified consecutive addresses.


## Build

Already tested and confirmed works with:

| NDK |CMake |Ninja |
|:---:|:----:|:----:|
|r27c |3.28.3|1.11.1|


## Base On

- [Android-Mem-Edit](https://github.com/mrcang09/Android-Mem-Edit) <!-- Shitcode -->

- [process_helper](https://gitee.com/liudegui/process_helper)


## License

```
Copyright (C) 2024, 2025  Dicot0721

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
```
