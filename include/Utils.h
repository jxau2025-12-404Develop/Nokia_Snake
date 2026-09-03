#include <iostream>
#include <cstdlib>
#include <random>

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

    // 初始化输出
    void init();

    // 输出命名空间
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

    // 写入文件命名空间
    namespace File
    {
        // 默认写入文件名
        std::string addr = "log";

        // 设置默认文件名
        bool SetAddr(const std::string newaddr)
        {
            addr = newaddr;
            return 0;
        }

        // 写入文件名
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

        // 写入文件名
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

    // 随机数的生成命名空间
    namespace Random
    {
        //  随机数引擎（种子使用硬件随机数，保证每次运行结果不同）
        std::random_device rd;
        std::mt19937 gen(rd()); // 梅森旋转算法，质量高、速度快

        const int min = 1;
        const int max = 50;

        // 随机数生成
        int Radom()
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
