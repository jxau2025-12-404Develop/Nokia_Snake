#include <iostream>

// 工具命名空间
namespace Utils
{

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
        // 输入函数（任意类型）
        template <typename T> void Input(T&& msg)
        {
            std::cin >> msg;
        }
    } // namespace Input

    // 文件IO命名函数
    namespace File
    {
        // 写入文件名
        std::string addr = "log";

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
} // namespace Utils