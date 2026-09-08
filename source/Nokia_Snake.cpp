// Nokia_Snake.cpp
#include <memory>

#include "NoKia_Snake.h"

#include "raylib.h"
#include "raygui.h"

#include "Utils.h"
#include "Snake.h"
#include "ScoreAndAccount.h"
#include "Renderer.h"
#include "Food.h"
#include "Collision.h"
#include "UI.h"

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

// 用户的登录和注册
void LoginSignIN(GameView& GV, std::unique_ptr<Account::ScoreAccount>& player)
{
    // 输入缓冲区：必须跨帧存活，GuiTextBox 每帧往里追加字符
    char username[64] = "";
    char password[64] = "";

    // 当前获得焦点的输入框：-1=无，0=用户名，1=密码（跨帧保存！）
    int activeField = -1;

    const Rectangle userBox = {330, 300, 300, 44};
    const Rectangle passBox = {330, 380, 300, 44};

    while (!WindowShouldClose())
    {
        // ===== 1. 输入状态更新 =====
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mouse, userBox))
                activeField = 0; // 点到用户名框
            else if (CheckCollisionPointRec(mouse, passBox))
                activeField = 1; // 点到密码框
            else
                activeField = -1; // 点空白处失焦
        }

        // 按回车视为提交一次输入
        bool submitted = (activeField != -1 && IsKeyPressed(KEY_ENTER));
        if (submitted)
            activeField = -1;

        // ===== 2. 整帧绘制 =====
        BeginDrawing();
        ClearBackground(Color{12, 20, 24, 255});

        const char* title = "USER LOGIN";
        DrawText(title, (GetScreenWidth() - MeasureText(title, 48)) / 2, 180, 48, Color{117, 220, 89, 255});

        // editMode 直接传“本框是否持有焦点”
        GuiTextBox(userBox, username, sizeof(username), activeField == 0);
        GuiTextBox(passBox, password, sizeof(password), activeField == 1);

        EndDrawing();

        // ===== 3. 提交后处理（接入账号校验/注册逻辑）=====
        if (submitted)
        {
            player = std::make_unique<Account::ScoreAccount>(username, password, 'l');
            break;
        }
    }
}

// 游戏运行函数
void Game(GameView& GV)
{
    // 重置状态
    InitGame(GV);

    // 创建玩家名称对象
    auto player = std::make_unique<Account::ScoreAccount>();

    // LoginSignIN(GV, player);

    // Utils::System::ClearScreen();

    // 创建蛇头信息
    auto snakehead = std::make_unique<SnakeHead>();
    snakehead->Node = GV.snake;

    // 保存游戏当前状态
    auto GameFlag = SCREEN_PLAYING;

    // 游戏进行从5 FPS开始（菜单为 10 FPS）
    unsigned int fps = 5;
    UI_SetFrameRate(fps);

    // 渲染画面
    UI_Render(&GV, GameFlag);

    // 获取起始移动方向（阻塞等待）
    if (IsKeyPressed(KEY_W))
        snakehead->hir = 'w';
    else if (IsKeyPressed(KEY_A))
        snakehead->hir = 'a';
    else if (IsKeyPressed(KEY_S))
        snakehead->hir = 's';
    else if (IsKeyPressed(KEY_D))
        snakehead->hir = 'd';

    while (!GV.GameFlag && !WindowShouldClose())
    {
        // 渲染画面
        UI_Render(&GV, GameFlag);

        // 非阻塞检查按键（raylib 每次 EndDrawing 后自动刷新按键状态）
        // 空格键切换暂停/继续
        if (IsKeyPressed(KEY_SPACE))
        {
            if (GameFlag == SCREEN_PAUSED)
                GameFlag = SCREEN_PLAYING;
            else if (GameFlag == SCREEN_PLAYING)
                GameFlag = SCREEN_PAUSED;
        }
        // 暂停时按 Q 退出游戏
        else if (GameFlag == SCREEN_PAUSED && IsKeyPressed(KEY_Q))
        {
            GameFlag = SCREEN_GAME_OVER;
            GV.GameFlag = true;
        }

        // 普通移动键：只有非暂停状态下才响应
        if (GameFlag == SCREEN_PLAYING)
        {
            if (IsKeyPressed(KEY_W) && snakehead->hir != 's')
                snakehead->hir = 'w';
            else if (IsKeyPressed(KEY_A) && snakehead->hir != 'd')
                snakehead->hir = 'a';
            else if (IsKeyPressed(KEY_S) && snakehead->hir != 'w')
                snakehead->hir = 's';
            else if (IsKeyPressed(KEY_D) && snakehead->hir != 'a')
                snakehead->hir = 'd';
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
        if (IsWallCollision(newX, newY, GV.width, GV.height) || IsSelfCollision(&GV, newX, newY))
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
        if (EatFood(&GV.food, &GV, GV.width, GV.height))
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

        // 计算新的帧率
        UI_SetFrameRate(fps + GV.score / 10);

        // 等待到下一帧再继续
        Renderer_WaitForNextFrame();
    }
}

// 程序运行的主函数
void Nokia_Snake()
{
    // 初始化帧
    GameView GV;

    GV.snake = NULL;

    InitGame(GV);

    // 输出开始菜单
    UI_Render(&GV, SCREEN_START_MENU);

    // 设置 UI 使用的目标帧率。
    UI_SetFrameRate(10);

    while (true && !WindowShouldClose())
    {
        // 每帧渲染菜单：UI_Render 内部的 EndDrawing 会调用 PollInputEvents，
        // 否则 raylib 不会刷新键盘状态，IsKeyPressed 永远读不到按键。
        UI_Render(&GV, SCREEN_START_MENU);

        // 使用 IsKeyPressed 检测“按下瞬间”，避免长按导致反复进入游戏。
        if (IsKeyPressed(KEY_ENTER))
        {
            // 进入游戏函数
            Game(GV);
            // 游戏结束返回开始菜单，下一轮循环会自动重新渲染菜单
            while (!WindowShouldClose())
            {
                // 游戏结束
                UI_Render(&GV, SCREEN_GAME_OVER);
                // player->SetScore(GV.score);
                // player->SaveGameResult();
                if (IsKeyPressed(KEY_SPACE))
                {
                    break;
                }
            }
        }
        else if (IsKeyPressed(KEY_Q))
        {
            break;
        }
    }
    return;
}