// Nokia_Snake.cpp
#include <memory>

#include "NoKia_Snake.h"

#include "Utils.h"
#include "Snake.h"
#include "ScoreAndAccount.h"
#include "Renderer.h"
#include "Food.h"

// 重置/初始化
void InitGame(GameView& GV)
{
    // 设置棋盘的长宽
    GV.height = HIGHT;
    GV.width = WIDTH;

    // 标记其为正在运行状态
    GV.GameFlag = false;

    // 判断是否有蛇数据
    if (GV.snake != NULL)
    {
        snake_destroy_game(&GV.snake);
    }
    GV.snake = snake_create_node(HIGHT, WIDTH);

    // 重置蛇身
    GV.snakeLength = 0;
    // 生成初始化的食物坐标
    GV.food = GenerateFood(&GV, GV.height, GV.width);
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

    // 创建蛇头
    auto snakehead = std::make_unique<SnakeHead>();
    snakehead->Node = GV.snake;

    // 保存游戏当前状态
    auto GameFlag = SCREEN_PLAYING;

    // 渲染画面
    Renderer_Render(&GV, GameFlag);
    Utils::Out::Out("按wasd任意一键开始");

    // 获取移动方向
    char key = Utils::Input::GetKey();
    if (key == 'w')
    {
        snakehead->hir = 'w';
    }
    else if (key == 'a')
    {
        snakehead->hir = 'a';
    }
    else if (key == 's')
    {
        snakehead->hir = 's';
    }
    else if (key == 'd')
    {
        snakehead->hir = 'd';
    }

    while (!GV.GameFlag)
    {
        // 渲染画面
        Renderer_Render(&GV, GameFlag);

        // 获取键
        char key = Utils::Input::HasKey();
        // 判断是否为暂停
        if (key == ' ')
        {
            if (GameFlag == SCREEN_PAUSED)
            {
                GameFlag = SCREEN_PLAYING;
            }
            else
            {
                GameFlag = SCREEN_PAUSED;
            }
        }
        else if (GameFlag == SCREEN_PAUSED && key == 27)
        {
            GameFlag = SCREEN_GAME_OVER;
            GV.GameFlag = true;
        }
        // 判断移动
        if (key == 'w')
        {
            snakehead->hir = 'w';
        }
        else if (key == 'a')
        {
            snakehead->hir = 'a';
        }
        else if (key == 's')
        {
            snakehead->hir = 's';
        }
        else if (key == 'd')
        {
            snakehead->hir = 'd';
        }
    }
}

// 程序运行的主函数
// TODO后续在这个函数里编写，不要动main函数
void Nokia_Snake()
{
    // 初始化帧
    GameView GV;

    // 输出开始菜单
    Renderer_Render(&GV, SCREEN_START_MENU);

    // 设置帧率
    Renderer_SetFrameRate(60);

    while (true)
    {
        // 选择
        auto key = Utils::Input::GetKey();
        if (key == '\r')
        {
            Game(GV);
            break;
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