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

        // 读取一个按键（阻塞等待，按下后立刻返回，不需要按回车）
        int GetKey()
        {
#if _WIN32
            int key = _getch();

            // 方向键在 Windows 下会先返回 0 或 224，再返回第二个扫描码
            if (key == 0 || key == 224)
            {
                int second = _getch();
                switch (second)
                {
                case 72:
                    return KEY_UP; // ↑
                case 80:
                    return KEY_DOWN; // ↓
                case 75:
                    return KEY_LEFT; // ←
                case 77:
                    return KEY_RIGHT; // →
                default:
                    return second; // 其它特殊功能键
                }
            }

            // 大写字母统一转换成小写：A -> 'a', Q -> 'q'
            if (key >= 'A' && key <= 'Z')
            {
                key += ('a' - 'A');
            }

            return key;
#else
            // 非 Windows 平台暂时不支持
            return 0;
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
        int Random()
        {
            try
            {
                // 均匀分布在 [min, max] 之间的整数
                std::uniform_int_distribution<int> dist(min, max);
                // 生成 min ~ max 的随机整数并返回
                return dist(gen);
            }
            catch (...)
            {
                return -1;
            }
        }
    } // namespace Random
} // namespace Utils
