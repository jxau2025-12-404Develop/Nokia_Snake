# 贪吃蛇（Nokia_Snake）

---

> 学校实训项目

## 一个基于 C++17 / CMake 的经典贪吃蛇小游戏的复刻项目。

## 功能

- [x] 工具函数库（控制台清屏 / 暂停 / 输入 / 文件写入 / 随机数）
- [x] 渲染框架（开始菜单 / 游戏进行中 / 暂停 / 结束）
- [x] 食物随机生成
- [x] 碰撞检测（撞墙 / 撞到自己则游戏结束）
- [x] 蛇的数据结构（链表实现，头插 / 尾删 / 清除）
- [ ] 完整游戏循环（蛇移动 + 键盘控制贯穿）
- [ ] 计分系统与最高分持久化
- [ ] 账号保存与登录系统

## 后期开发

- [ ] 网络数据库存储

> 当前进度：已完成工具函数、渲染器框架、碰撞检测与食物模块，
> 正在整合蛇的移动逻辑与完整游戏循环，后续功能将在迭代中逐步完成。

---

## 技术栈

- 语言：C++17
- 构建工具：CMake
- 编译器：支持 C++17 的任意编译器（如 GCC、MSVC、Clang）

---

## 文件结构

```
Nokia_Snake
|
|-- CMakeLists.txt        # CMake 构建配置
|-- ReadME.md             # 项目说明
|-- include/              # 头文件目录
|   |-- Utils.h             # 工具函数头文件
|   |-- NoKia_Snake.h       # 游戏主入口头文件
|   |-- Renderer.h          # 渲染器头文件（C 接口）
|   |-- Collision.h         # 碰撞检测头文件
|   `-- Food.h              # 食物模块头文件
|   
|-- source/               # 源文件目录
|   |-- main.cpp            # 程序入口
|   |-- Utils.cpp           # 工具库非模板函数的实现
|   |-- Nokia_Snake.cpp     # 游戏主循环实现
|   |-- Renderer.c          # 渲染器实现
|   |-- Collision.cpp       # 碰撞检测实现
|   `-- Food.cpp            # 食物生成与交互实现
|-- body/                 # 其他资源目录
|-- .gitignore            # Git 忽略规则
|-- .clang-format         # 自动格式化工具配置文件（搭配Clang-Format）
```

---

## 构建与运行

```bash
# 创建存储位置
mkdir build                         # 创建构建build文件（有的话跳过）
cd build                            # 进入构建的文件夹

# 配置
cmake -S . -B build                 # 构建项目（cmake有更改时记得重新构建）

# 编译
cmake --build build                 # 构建可执行文件（exe）

# 运行
./build/Nokia_Snake                 # Linux / macOS
build\Debug\Nokia_Snake.exe         # Windows (MSVC)
```

---

## 操作说明

> 待游戏功能实现后生效

- ↑ / W：向上
- ↓ / S：向下
- ← / A：向左
- → / D：向右
- 空格：暂停 / 继续

---

## 开发计划

0. [x] 工具函数的实现
1. [x] 实现游戏画面渲染与游戏界面（渲染器框架）
2. [ ] 实现蛇的移动逻辑与键盘输入控制
3. [x] 实现食物随机生成与蛇的成长（基础数据结构与交互函数）
4. [x] 实现碰撞检测与游戏结束判定（检测函数完成，待接入游戏循环）
5. [ ] 实现计分系统与最高分持久化
6. [ ] 实现账号登录
7. [ ] 远程数据库存储

---

## 说明

本项目为学校实训课程作业，仅供学习交流使用。

## 项目成员

### jyoushitou

- 联系方式：[xzt98948364@outlook.com](mailto:xzt98948364@outlook.com)
- 博客地址：[https://jyoushitou.github.io/](https://jyoushitou.github.io/)

### 负责部分

- git分支的维护
- 工具函数的编写
- 项目整体的架构

### WB341

- 联系方式：[2361742861@qq.com](mailto:2361742861@qq.com)

### myqx0309

- 联系方式：[mengyqx_2025@qq.com](mailto:mengyqx_2025@qq.com)

### bridge-tu

- 联系方式：[3576592547@.qq.com](mailto:3576592547@qq.com)

### liujiaxin123110

- 联系方式：[2449146540@qq.com](2449146540@qq.com)

## 项目维护者：[jyoushitou]

- 邮箱：[xzt98948364@outlook.com](mailto:xzt98948364@outlook.com)
- 博客地址：[https://jyoushitou.github.io/](https://jyoushitou.github.io/)
- 项目地址：[https://github.com/jyoushitou/Nokia_Snake](https://github.com/jyoushitou/Nokia_Snake)

---
