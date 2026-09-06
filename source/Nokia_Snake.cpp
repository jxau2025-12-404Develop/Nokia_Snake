// Nokia_Snake.cpp
#include "NoKia_Snake.h"
#include "Utils.h"
#include "Food.h"
#include "Collision.h"

#include <iostream>

namespace
{
void ReleaseSnake(GameView& view)
{
    while (view.snake != nullptr)
    {
        SnakeNode* next = view.snake->Next;
        delete view.snake;
        view.snake = next;
    }
    view.snakeLength = 0;
}

void PrintFrame(const GameView& view)
{
    std::cout << "位置: (" << view.snake->xy.x << ", " << view.snake->xy.y
              << ")  食物: (" << view.food.x << ", " << view.food.y
              << ")  得分: " << view.score << '\n';
}
}

// 重置
// 引用渲染帧
void InitGV(GameView& GV)
{
    GV.width = WIDTH;
    GV.height = HIGHT;
    GV.snake = new SnakeNode{{GV.width / 2, GV.height / 2}, nullptr};
    GV.snakeLength = 1;
    GV.food = {0, 0};
    GV.score = 0;
    GV.GameFlag = true;
    GV.food = GenerateFood(&GV, GV.width, GV.height);
}

// 游戏主程序
int Game(GameView& GV)
{
    int dirX = 1;
    int dirY = 0;

    while (GV.GameFlag && GV.snake != nullptr)
    {
        PrintFrame(GV);
        const int key = Utils::Input::GetKey();

        if (key == 'q' || key == 'Q')
        {
            GV.GameFlag = false;
            break;
        }
        if (key == 'w' || key == 'W' || key == Utils::Input::KEY_UP)
        {
            dirX = 0;
            dirY = -1;
        }
        else if (key == 's' || key == 'S' || key == Utils::Input::KEY_DOWN)
        {
            dirX = 0;
            dirY = 1;
        }
        else if (key == 'a' || key == 'A' || key == Utils::Input::KEY_LEFT)
        {
            dirX = -1;
            dirY = 0;
        }
        else if (key == 'd' || key == 'D' || key == Utils::Input::KEY_RIGHT)
        {
            dirX = 1;
            dirY = 0;
        }

        const int nextX = GV.snake->xy.x + dirX;
        const int nextY = GV.snake->xy.y + dirY;
        if (IsWallCollision(nextX, nextY, GV.width, GV.height) ||
            IsSelfCollision(&GV, nextX, nextY))
        {
            GV.GameFlag = false;
            break;
        }

        GV.snake->xy = {nextX, nextY};
        EatFood(&GV.food, &GV, GV.width, GV.height);
    }

    return GV.score;
}

// 程序运行的主函数
// TODO后续在这个函数里编写，不要动main函数
void Nokia_Snake()
{
    GameView GV{};
    while (true)
    {
        InitGV(GV);
        Game(GV);
        ReleaseSnake(GV);
        return;
    }
}