// 工具文件
#include "Utils.h"
#include "NoKia_Snake.h"

int main()
{
    Utils::init();

    Utils::Out::Out("欢迎游玩贪吃蛇");

    Nokia_Snake();

    Utils::Out::Out("游玩结束");
    return 0;
}