// Nokia_Snake.cpp
#include <memory>

#include "NoKia_Snake.h"
#include "Utils.h"

// 重置/初始化
void InitGame(GameView& GV)
{
    GV.GameFlag = false;
    std::shared_ptr<SnakeHead> head = std::make_shared<SnakeHead>();

    GV.snake = &(head->Node);
}

// 程序运行的主函数
// TODO后续在这个函数里编写，不要动main函数
void Nokia_Snake()
{
    // // 示例：识别 W/A/S/D 和方向键
    // switch (key)
    // {
    // case 'w':
    // case Utils::Input::KEY_UP:
    //     // TODO: 蛇向上移动
    //     break;
    // case 's':
    // case Utils::Input::KEY_DOWN:
    //     // TODO: 蛇向下移动
    //     break;
    // case 'a':
    // case Utils::Input::KEY_LEFT:
    //     // TODO: 蛇向左移动
    //     break;
    // case 'd':
    // case Utils::Input::KEY_RIGHT:
    //     // TODO: 蛇向右移动
    //     break;
    // default:
    //     // 其它按键暂时忽略
    //     break;
    // }
    return;
}