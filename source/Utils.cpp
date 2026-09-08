// Utils.cpp
#include <cstdlib>
#include <iostream>
#include <random>

#include "Utils.h"

#if _WIN32
#include <windows.h>
#include <conio.h> // _kbhit / _getch：直接读取控制台按键
#endif

namespace Utils
{
    // 控制台
    namespace System
    {
        // 清屏
        void ClearScreen()
        {
#if _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hConsole == INVALID_HANDLE_VALUE)
                return;

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
                return;

            DWORD cells = static_cast<DWORD>(csbi.dwSize.X) * csbi.dwSize.Y;
            DWORD written = 0;

            COORD home{0, 0};
            // 用空格填充整个屏幕缓冲区（不闪烁）
            FillConsoleOutputCharacterA(hConsole, ' ', cells, home, &written);
            // 恢复原属性（避免背景色被改成默认）
            FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cells, home, &written);
            // 光标移动回 (0,0)
            SetConsoleCursorPosition(hConsole, home);
#else
            // ANSI 转义序列清屏（非 Windows 平台）
            std::cout << "\033[2J\033[H";
#endif
        }

        void Pause(const std::string& msg)
        {
            Out::Out(msg);

            std::cin.get();
        }
    } // namespace System

    // 立即读取按键，不需要按回车
    namespace Input
    {
        // 获取整行
        std::string InputLine()
        {
            std::string msg;
            do
            {
                // 提示玩家输入昵称。
                std::cout << "请输入玩家昵称：";
                // 读取一整行昵称。
                std::getline(std::cin, msg);
                // 输入为空时继续读取，直到昵称有效。
            } while (msg.empty() && !std::cin.eof());
            return msg;
        }

        // 检查是否有按键被按下（非阻塞）
        bool HasKey()
        {
#if _WIN32
            // _kbhit 返回非 0 表示有按键等待读取
            return _kbhit() != 0;
#else
            // 非 Windows 平台暂时不支持
            return false;
#endif
        }
    } // namespace Input

    // 初始化控制台
    void init()
    {
#ifdef _WIN32
        // 清屏
        System::ClearScreen();

        // 设置控制台的编码格式
        SetConsoleOutputCP(CP_UTF8);
#else
        // 清屏
        System::ClearScreen();
#endif
    }

    namespace File
    {
        // 设置默认文件名
        bool SetAddr(const std::string& newaddr)
        {
            addr = newaddr;
            return true;
        }

    } // namespace File

    namespace Random
    {
        // 随机数
        int Random()
        {
            try
            {
                // 均匀分布在 [min, max] 之间的整数
                std::uniform_int_distribution<int> dist(min, WALL);
                // 生成 min ~ max 的随机整数并返回
                return dist(gen);
            }
            catch (...)
            {
                return -1;
            }
        }

        // 生成目前帧率
        int FPSAdd(int fps, int score)
        {
            return fps + score / 10 / Random() / 10;
        }
    } // namespace Random
} // namespace Utils
