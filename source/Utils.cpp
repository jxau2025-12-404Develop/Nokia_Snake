// Utils.cpp
#include <cstdlib>
#include <random>

#include "Utils.h"

#if _WIN32
#include <windows.h>
#else
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
            printf("\033[2J\033[H")
#endif
        }

        // 等待
        void Pause()
        {
            Out::Out("Please Enter to continue...");

            std::cin.get();
        }

        void Pause(const std::string msg)
        {
            Out::Out(msg);

            std::cin.get();
        }
    } // namespace System

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
        bool SetAddr(const std::string newaddr)
        {
            addr = newaddr;
            return 0;
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








