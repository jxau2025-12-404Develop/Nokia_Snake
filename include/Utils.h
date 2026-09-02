#include <iostream>
#include <cstdlib>

#if _WIN32
#else
#endif

void ClearScreen()
{
#if _WIN32
    system("cls");
#else
    printf("\033[2J\033[H")
#endif
}

// 输出函数（任意类型）
template <typename T> void Out(T&& msg)
{
    std::cout << msg << std::endl;
}

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