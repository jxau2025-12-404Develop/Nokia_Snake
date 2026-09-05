// Utils.h
#pragma once
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <fstream>
#include <string>

constexpr int HIGHT = 50;
constexpr int WIDTH = 50;

// 坐标信息
typedef struct Point
{
    int x;
    int y;
} Point;

// 蛇身节点
typedef struct Snake
{
    Point xy;
    SnakeNode* Next;
} SnakeNode;

// 蛇头节点
typedef struct SnakeHead
{
    // 基础信息
    SnakeNode Node;
    // 行走方向
    char hir;
} SnakeHead;

// 功能：保存绘制一帧画面所需要的游戏数据。
// 保存这一帧的数据
typedef struct
{
    int width;        // 保存棋盘宽度。
    int height;       // 保存棋盘高度。
    SnakeNode* snake; // 指向蛇头链表。
    int snakeLength;  // 保存当前蛇的长度。
    Point food;       // 保存食物坐标。
    int score;        // 保存当前分数。
    bool GameFlag;    // 游戏状态
} GameView;

namespace Utils
{
    // 控制台
    namespace System
    {
        // 清屏
        // 无参数
        void ClearScreen();

        // 等待
        // 无参数默认打印"Please Enter to continue..."，有参打印输入的字符串
        void Pause(const std::string& msg = "Please Enter to continue...");
    } // namespace System

    // 新增：
    // 不修改 System 中的任何现有函数

    // 初始化输出
    // 无参数
    void init();

    // 输出命名空间
    namespace Out
    {
        // 输出函数
        // 任意类型
        template <typename T> void Out(T&& msg)
        {
            std::cout << msg << std::endl;
        }

    } // namespace Out

    // 输入命名空间
    namespace Input
    {
        // 输入函数
        // 无参
        template <typename T> T Input()
        {
            T value;
            while (true)
            {
                if (std::cin >> value)
                {
                    return value;
                }

                if (std::cin.eof())
                {
                    std::cout << "\n输入到达文件尾，程序退出\n";
                    std::exit(EXIT_FAILURE);
                }

                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "输入无效，请重新输入: ";
            }
        }

        // 方向键返回值
        enum Key
        {
            KEY_UP = 256, // ↑
            KEY_DOWN,     // ↓
            KEY_LEFT,     // ←
            KEY_RIGHT     // →
        };

        // 检查是否有按键被按下（非阻塞，不会等待）
        bool HasKey();

        // 读取一个按键（阻塞等待，按下后立刻返回，不需要按回车）
        // 普通字母自动转为小写，如 A -> 'a'
        // 回车返回 '\r'，空格返回 ' '，Esc 返回 27
        // 方向键返回 KEY_UP / KEY_DOWN / KEY_LEFT / KEY_RIGHT
        int GetKey();
    } // namespace Input

    // 写入文件命名空间
    namespace File
    {
        // 默认写入文件名
        inline std::string addr = "log";

        // 设置默认文件名
        bool SetAddr(const std::string& newaddr);

        // 追加写入文件名
        // 第一位传入任意类型参数到文件中，第二位，默认为log，有字符串类型参数，为字符串的文件名
        template <typename T> bool Add(T&& msg, const std::string address = addr)
        {
            // 打开文件
            std::ofstream out(address.c_str(), std::ios::app);

            // 判断是否打开
            if (!out)
            {
                std::cerr << "打开文件失败" << std::endl;

                return 0;
            }

            // 写入文件
            out << msg << std::endl;

            // 关闭文件
            out.close();

            return 1;
        }
    } // namespace File

    // 随机数的生成命名空间
    namespace Random
    {
        // 随机数引擎（种子使用硬件随机数，保证每次运行结果不同）
        inline std::random_device rd;
        // 梅森旋转算法，质量高、速度快
        inline std::mt19937 gen(rd());

        // 随机数最小的数
        const int min = 1;
        // 随机数最大的数（尽量保证与棋盘一样大）
        const int max = 50;

        // 随机数生成
        // 无参
        int Random();
    } // namespace Random
} // namespace Utils
