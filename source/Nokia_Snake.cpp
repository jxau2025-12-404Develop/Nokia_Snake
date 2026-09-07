// Nokia_Snake.cpp
#include <memory>

#include "NoKia_Snake.h"
#include "Utils.h"
#include "Snake.h"
#include "ScoreAndAccount.h"
#include "Renderer.h"

// 重置/初始化
void InitGame(GameView& GV)
{
    // 设置棋盘的长宽
    GV.height = HIGHT;
    GV.width = WIDTH;

    // 标记其为正在运行状态
    GV.GameFlag = false;

    if (GV.snake != NULL)
    {
        snake_destroy_game(&GV.snake);
    }
    GV.snake = snake_create_node(HIGHT, WIDTH);
    GV.snakeLength = 0;
}

// 游戏运行函数
void Game(GameView& GV)
{
    // 重置状态
    InitGame(GV);

    // 创建玩家名称对象
    Utils::Out::Out("输入玩家名称");
    auto player = std::make_unique<Account::ScoreAccount>();
    player->InputNickname();

    while (true)
    {
        Renderer_Render(&GV, SCREEN_PLAYING);
    }
}

// 程序运行的主函数
// TODO后续在这个函数里编写，不要动main函数
void Nokia_Snake()
{
    // 初始化帧
    GameView GV;

    // 获取到对应的按键
    Renderer_Render(&GV, SCREEN_START_MENU);

    // 设置帧率
    Renderer_SetFrameRate(60);

    // 选择
    auto key = Utils::Input::GetKey();
    while (true)
    {
        if (key == '\r')
        {
            Game(GV);
        }
        else if (key == 'q')
        {
            break;
        }
    }
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