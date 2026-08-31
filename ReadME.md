# 贪吃蛇（Nokia_Snake）

> 学校实训项目

一个基于 C++17 / CMake 的经典贪吃蛇小游戏。

## 功能特性

- 经典贪吃蛇玩法
- 方向控制（W/A/S/D 或方向键）
- 食物随机生成
- 计分系统
- 碰撞检测（撞墙 / 撞到自己则游戏结束）

## 技术栈

- 语言：C++17
- 构建工具：CMake
- 编译器：支持 C++17 的任意编译器（如 GCC、MSVC、Clang）

## 文件结构

```
Nokia_Snake
|
|-- CMakeLists.txt        # CMake 构建配置
|-- main.cpp              # 程序入口
|-- ReadME.md             # 项目说明
|-- include/              # 头文件目录
|-- source/               # 源文件目录
|-- body/                 # 其他资源目录
|-- .gitignore            # Git 忽略规则
|-- .vscode/              # 本地配置文件（不提交）
```

## 构建与运行

```bash
# 配置
cmake -S . -B build

# 编译
cmake --build build

# 运行
./build/Nokia_Snake                 # Linux / macOS
build\Debug\Nokia_Snake.exe         # Windows (MSVC)
```

## 操作说明

- ↑ / W：向上
- ↓ / S：向下
- ← / A：向左
- → / D：向右
- 空格：暂停 / 继续

## 当前进度

- [x] 项目初始化（CMake 构建配置）
- [ ] 游戏主循环
- [ ] 蛇的移动与增长
- [ ] 食物随机生成
- [ ] 碰撞检测
- [ ] 计分与最高分记录

## 说明

本项目为学校实训课程作业，仅供学习交流使用。
