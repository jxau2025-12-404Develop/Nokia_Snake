// Nokia_Snake.cpp
#include <memory>

#include "NoKia_Snake.h"

#include "Utils.h"
#include "Snake.h"
#include "ScoreAndAccount.h"
#include "Renderer.h"
#include "Food.h"
#include "Collision.h"

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
    GV.snake = snake_create_node(HIGHT / 2, WIDTH / 2);

    // 重置蛇身长度
    GV.snakeLength = 1;
    GV.score = 0;

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

    // 创建蛇头信息
    auto snakehead = std::make_unique<SnakeHead>();
    snakehead->Node = GV.snake;
    snakehead->hir = 'd'; // 默认向右

    // 保存游戏当前状态
    auto GameFlag = SCREEN_PLAYING;

    // 渲染画面
    Renderer_Render(&GV, GameFlag);
    Utils::Out::Out("按wasd任意一键开始");

    // 获取起始移动方向（阻塞等待）
    int key = Utils::Input::GetKey();
    if (key == 'w' || key == Utils::Input::KEY_UP)
        snakehead->hir = 'w';
    else if (key == 'a' || key == Utils::Input::KEY_LEFT)
        snakehead->hir = 'a';
    else if (key == 's' || key == Utils::Input::KEY_DOWN)
        snakehead->hir = 's';
    else if (key == 'd' || key == Utils::Input::KEY_RIGHT)
        snakehead->hir = 'd';

    while (!GV.GameFlag)
    {
        // 渲染画面
        Renderer_Render(&GV, GameFlag);

        // 非阻塞检查按键
        while (Utils::Input::HasKey())
        {
            key = Utils::Input::GetKey();

            // 空格键切换暂停/继续
            if (key == ' ')
            {
                if (GameFlag == SCREEN_PAUSED)
                    GameFlag = SCREEN_PLAYING;
                else
                    GameFlag = SCREEN_PAUSED;
            }
            // 暂停时按 Esc 退出游戏
            else if (GameFlag == SCREEN_PAUSED && key == 'q')
            {
                GameFlag = SCREEN_GAME_OVER;
                GV.GameFlag = true;
            }
            // 普通移动键
            else if (key == 'w' || key == Utils::Input::KEY_UP)
            {
                if (snakehead->hir != 's')
                    snakehead->hir = 'w';
            }
            else if (key == 'a' || key == Utils::Input::KEY_LEFT)
            {
                if (snakehead->hir != 'd')
                    snakehead->hir = 'a';
            }
            else if (key == 's' || key == Utils::Input::KEY_DOWN)
            {
                if (snakehead->hir != 'w')
                    snakehead->hir = 's';
            }
            else if (key == 'd' || key == Utils::Input::KEY_RIGHT)
            {
                if (snakehead->hir != 'a')
                    snakehead->hir = 'd';
            }
        }

        // 暂停状态下不移动，等待下一帧
        if (GameFlag == SCREEN_PAUSED)
        {
            Renderer_WaitForNextFrame();
            continue;
        }

        // 计算下一个蛇头位置
        int newX = snakehead->Node->xy.x;
        int newY = snakehead->Node->xy.y;
        if (snakehead->hir == 'w')
            --newY;
        else if (snakehead->hir == 's')
            ++newY;
        else if (snakehead->hir == 'a')
            --newX;
        else if (snakehead->hir == 'd')
            ++newX;

        // 墙壁碰撞检测
        if (IsWallCollision(newX, newY, GV.width, GV.height))
        {
            GV.GameFlag = true;
            GameFlag = SCREEN_GAME_OVER;
            break;
        }

        // 自己碰撞检测（移动时蛇尾会离开，所以只要新头不在蛇身（除尾外）即可）
        SnakeNode* tailNode = snakehead->Node;
        while (tailNode->Next != NULL)
            tailNode = tailNode->Next;

        bool hitSelf = false;
        for (SnakeNode* p = snakehead->Node; p != NULL; p = p->Next)
        {
            if (p != tailNode && p->xy.x == newX && p->xy.y == newY)
            {
                hitSelf = true;
                break;
            }
        }
        if (hitSelf)
        {
            GV.GameFlag = true;
            GameFlag = SCREEN_GAME_OVER;
            break;
        }

        // 蛇头前插新节点
        if (snake_push_front(&snakehead->Node, newX, newY) != 0)
        {
            GV.GameFlag = true;
            GameFlag = SCREEN_GAME_OVER;
            break;
        }
        ++GV.snakeLength;

        // 若吃到食物则加分并重新生成食物，否则删除蛇尾
        if (newX == GV.food.x && newY == GV.food.y)
        {
            GV.score += 10;
            GV.food = GenerateFood(&GV, GV.height, GV.width);
        }
        else
        {
            snake_pop_back(&snakehead->Node);
            --GV.snakeLength;
        }

        // 同步回 GameView
        GV.snake = snakehead->Node;

        // 等待到下一帧再继续
        Renderer_WaitForNextFrame();
    }

    // 游戏结束
    Renderer_Render(&GV, SCREEN_GAME_OVER);
    player->SaveGameResult();
}

// 程序运行的主函数
void Nokia_Snake()
{
    // 初始化帧
    GameView GV;

    GV.snake = NULL;

    InitGame(GV);

    // 输出开始菜单
    Renderer_Render(&GV, SCREEN_START_MENU);

    // 设置帧率
    Renderer_SetFrameRate(30); // 蛇移动速度比较合适，10帧/秒

    while (true)
    {
        // 选择
        auto key = Utils::Input::GetKey();
        if (key == '\r')
        {
            // 进入游戏函数
            Game(GV);
            // 游戏结束后返回开始菜单，允许再来一局
            Renderer_Render(&GV, SCREEN_START_MENU);
        }
        else if (key == 'q')
        {
            break;
        }
    }
    return;
}