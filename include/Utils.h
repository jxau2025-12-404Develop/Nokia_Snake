#include <iostream>
#include <random>

namespace Utils
{
    // 控制台
    namespace System
    {
        // 清屏
        void ClearScreen();

        // 等待
        void Pause();
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
        inline std::string addr = "log";

        // 设置默认文件名
        bool SetAddr(const std::string newaddr);

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
        inline std::random_device rd;
        inline std::mt19937 gen(rd()); // 梅森旋转算法，质量高、速度快

        const int min = 1;
        const int max = 50;

        // 随机数生成
        int Random();
    } // namespace Random
} // namespace Utils
