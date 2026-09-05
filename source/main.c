#include "NoKia_Snake.h"
#include "Utils.h"

// 功能：作为程序入口，负责初始化工具并启动游戏。
// 无参数
// 不要把游戏规则直接写进 main，方便后续测试。
// 以前的注释：程序入口。
int main(void)
{
    // 初始化随机数和控制台。
    Utils_Init();
    // 进入贪吃蛇游戏主循环。
    Nokia_Snake();
    // 返回 0 表示程序正常结束。
    return 0;
}
