#include <iostream>
#include <cstdlib>

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
    } // namespace System

    //
    namespace Out
    {
        // 输出函数（任意类型）
        template <typename T> void Out(T&& msg)
        {
            std::cout << msg << std::endl;
        }

    } // namespace Out

    // 输入命名空间
    namespace Input
    {

        // 输入函数（无传参函数，必须声明类型）
        // 使用Input<类型>
        template <typename T> T Input()
        {
            T msg;

            std::cin >> msg;

            return msg;
        }

        // 输入函数（任意类型）
        template <typename T> T Input(T&& msg)
        {
            std::cin >> msg;

            return msg;
        }
    } // namespace Input

    // 写入文件
    namespace File
    {
        // 写入文件名
        std::string addr = "log";

        // 写入文件名（默认文件）
        template <typename T> bool File_Add(T&& msg)
        {
            // 打开文件
            std::ofstream out(addr.c_str(), std::ios::app);

            // 判断是否打开
            if (!out)
            {
                std::cerr << "打开文件失败" << std::endl;

                return 1;
            }

            // 写入文件
            out << msg << std::endl;

            // 关闭文件
            out.close();

            return 0;
        }

        // 写入文件名（带文件）
        template <typename T> bool File_add(T&& msg, std::string addr)
        {
            std::ofstream out(addr.c_str(), std::ios::app);
            if (!out)
            {
                std::cerr << "打开文件失败" << std::endl;
                return 1;
            }
            out << msg << std::endl;
            out.close();
            return 0;
        }
    } // namespace File
} // namespace Utils
