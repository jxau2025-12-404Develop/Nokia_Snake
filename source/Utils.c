#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

// 功能：初始化随机数和控制台。
// 无参数
// 以后可以在这里设置窗口大小。
// 以前的注释：初始化控制台。
void Utils_Init(void)
{
    // 使用当前时间作为随机数种子，让每次食物位置不同。
    srand((unsigned int)time(NULL));
#ifdef _WIN32
    // 设置 Windows 控制台使用 UTF-8 输出中文。
    SetConsoleOutputCP(CP_UTF8);
#endif
    // 清除控制台中原有的内容。
    Utils_ClearScreen();
}

// 功能：清空控制台，并把光标移动到左上角。
// 无参数
// Linux 和 Windows 可以分别使用更平滑的刷新方法。
// 以前的注释：清屏。
void Utils_ClearScreen(void)
{
#ifdef _WIN32
    // Windows 使用 cls 命令清屏。
    system("cls");
#else
    // 其他系统使用 ANSI 转义序列清屏并移动光标。
    printf("\033[2J\033[H");
#endif
}

// 功能：暂停程序，等待用户按回车。
// 无参数
// 可以改为无须回车的按键暂停。
// 以前的注释：等待。
void Utils_Pause(void)
{
    // 保存 getchar 读到的字符。
    int character;
    // 提示玩家按下回车。
    printf("按回车键继续...");
    // 持续读取字符，直到读取到回车或文件结束。
    while ((character = getchar()) != '\n' && character != EOF)
    {
        // 丢弃回车前可能残留的其他字符。
    }
}

// 功能：返回 minimum 到 maximum 之间的随机整数。
// 传入参数说明：minimum 是最小值，maximum 是最大值。
// 调用者需要保证 minimum 不大于 maximum。
// 以前的注释：随机数生成。
int Utils_Random(int minimum, int maximum)
{
    // 范围无效时返回最小值，避免出现除以零的问题。
    if (minimum >= maximum)
    {
        return minimum;
    }
    // rand 的结果经过取模后转换到指定范围。
    return minimum + rand() % (maximum - minimum + 1);
}
