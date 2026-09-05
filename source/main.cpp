// 工具文件
#include "Utils.h"
#include "NoKia_Snake.h"

int main()
{
    // 初始化控制台
    Utils::init();

    Utils::Out::Out("欢迎游玩贪吃蛇");

    // 游戏主函数
    Nokia_Snake();

    Utils::Out::Out("游玩结束");
    return 0;
}