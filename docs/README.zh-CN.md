# Android-Memory-Editor

**[English](./README.md)** | **简体中文**

[![语言](https://img.shields.io/github/languages/top/Dicot0721/Android-Memory-Editor)]()
[![许可协议](https://img.shields.io/github/license/Dicot0721/Android-Memory-Editor)
](https://www.gnu.org/licenses/gpl-3.0.html)

一个简单的内存搜索/修改工具, 基于安卓的 Linux 内核.


## 功能

> 几乎全部都需要 root.

- 根据应用包名查找对应进程的 PID.

- 根据 PID 暂停/恢复进程.

- 根据指定的值查找地址.

- 根据指定的值的范围查找地址.

- 根据指定的值和偏移查找地址.

- 根据一串指定的值查找连续地址的起始位置.

- 将指定的值写入指定的地址.

- 将一串指定的值写入指定的连续地址.


## 构建

以下配置已经过测试并确认可正常工作:

| NDK |CMake |Ninja |
|:---:|:----:|:----:|
|r27c |3.28.3|1.11.1|


## 基于

- [Android-Mem-Edit](https://github.com/mrcang09/Android-Mem-Edit)

- [process_helper](https://gitee.com/liudegui/process_helper)


## 许可协议

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
