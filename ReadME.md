# 贪吃蛇（Nokia_Snake）

> 学校实训项目

一个基于 C++17 / CMake 的经典贪吃蛇小游戏的复刻项目。

## 功能

- [ ] 经典贪吃蛇玩法
- [ ] 方向控制（W/A/S/D 或方向键）
- [ ] 食物随机生成
- [ ] 碰撞检测（撞墙 / 撞到自己则游戏结束）
- [ ] 计分系统
- [ ] 账号保存系统
- [ ] 网络数据库存储

> 当前为项目初始化阶段，仅搭建了 CMake 构建配置与基本目录结构，
> 入口程序暂为 Hello World 占位输出，上述功能将在后续迭代中逐步实现。

## 技术栈

- 语言：C++17
- 构建工具：CMake
- 编译器：支持 C++17 的任意编译器（如 GCC、MSVC、Clang）

## 文件结构

```
Nokia_Snake
|
|-- CMakeLists.txt        # CMake 构建配置
|-- ReadME.md             # 项目说明
|-- include/              # 头文件目录
|   `-- auto.h            # 自动头文件（当前为空）
|-- source/               # 源文件目录
|   `-- main.cpp          # 程序入口（当前为占位输出）
|-- body/                 # 其他资源目录
|-- .gitignore            # Git 忽略规则
|-- .clang-format         # 自动格式化工具配置文件（搭配Clang-Format）
```

## 构建与运行

```bash
mkdir build
cd build                            # 进入构建的文件夹

# 配置
cmake -S . -B build

# 编译
cmake --build build

# 运行
./build/Nokia_Snake                 # Linux / macOS
build\Debug\Nokia_Snake.exe         # Windows (MSVC)
```

## 操作说明

> 待游戏功能实现后生效

- ↑ / W：向上
- ↓ / S：向下
- ← / A：向左
- → / D：向右
- 空格：暂停 / 继续

## 开发计划

0. 工具类的实现
1. 实现游戏画面渲染与游戏界面
2. 实现蛇的移动逻辑与键盘输入控制
3. 实现食物随机生成与蛇的成长
4. 实现碰撞检测与游戏结束判定
5. 实现计分系统与最高分持久化
6. 实现账号登录
7. 远程数据库存储

## 说明

本项目为学校实训课程作业，仅供学习交流使用。

## 项目成员

### jyoushitou

- 联系方式：[xzt98948364@outlook.com](mailto:xzt98948364@outlook.com)
- 博客地址：[https://jyoushitou.github.io/](https://jyoushitou.github.io/)

### WB341

- 联系方式：[2361742861@qq.com](mailto:2361742861@qq.com)

### myqx0309

- 联系方式：[mengyqx_2025@qq.com](mailto:mengyqx_2025@qq.com)

### bridge-tu

- 联系方式：[3576592547@.qq.com](mailto:3576592547@qq.com)

## 项目维护者：[jyoushitou]

- 邮箱：[xzt98948364@outlook.com](mailto:xzt98948364@outlook.com)
- 博客地址：[https://jyoushitou.github.io/](https://jyoushitou.github.io/)
- 项目地址：[https://github.com/jyoushitou/Nokia_Snake](https://github.com/jyoushitou/Nokia_Snake)

---
