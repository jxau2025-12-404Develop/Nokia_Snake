#include "Food.h"

#include <stdexcept>

namespace
{
    // 函数作用：判断指定坐标是否被蛇身占用。
    // 函数传参：view 为当前游戏画面数据，x 和 y 为待检测坐标。
    // 返回值：坐标被蛇身占用时返回 true，否则返回 false。
    bool IsSnakePosition(const GameView* view, int x, int y)
    {
        if (view == nullptr)
        {
            return false;
        }

        const SnakeNode* node = view->snake;

        while (node != nullptr)
        {
            if (node->xy.x == x && node->xy.y == y)
            {
                return true;
            }

            node = node->Next;
        }

        return false;
    }
}

// 函数作用：在棋盘空闲位置生成食物。
// 函数传参：view 为当前游戏画面数据，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：返回生成的食物坐标；棋盘没有空闲位置时抛出异常。
Point GenerateFood(
    const GameView* view,
    int boardWidth,
    int boardHeight)
{
    if (view == nullptr || boardWidth <= 0 || boardHeight <= 0)
    {
        throw std::invalid_argument("游戏数据或棋盘大小无效");
    }

    const long long boardSize =
        static_cast<long long>(boardWidth) * boardHeight;

    long long occupiedCount = 0;

    // 使用 Utils.h 中的蛇链表统计已占用格子。
    for (const SnakeNode* node = view->snake;
         node != nullptr;
         node = node->Next)
    {
        ++occupiedCount;
    }

    // 蛇身占满棋盘时，没有空闲格子。
    if (occupiedCount >= boardSize)
    {
        throw std::runtime_error("棋盘没有空闲格子");
    }

    Point food{};

    // Renderer 的有效区域是：
    // x：0 到 boardWidth - 1
    // y：0 到 boardHeight - 1
    do
    {
        food.x = std::rand() % boardWidth;
        food.y = std::rand() % boardHeight;
    }
    while (IsSnakePosition(view, food.x, food.y));

    return food;
}

// 函数作用：判断蛇头是否吃到食物，并更新分数和食物坐标。
// 函数传参：food 为食物坐标，view 为当前游戏画面数据，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：吃到食物时返回 true，否则返回 false。
bool EatFood(
    Point* food,
    GameView* view,
    int boardWidth,
    int boardHeight)
{
    if (food == nullptr ||
        view == nullptr ||
        view->snake == nullptr)
    {
        return false;
    }

    // 判断蛇头坐标是否与食物坐标相同。
    if (view->snake->xy.x != food->x ||
        view->snake->xy.y != food->y)
    {
        return false;
    }

    // 增加分数。
    view->score += 10;

    // 生成新的食物。
    *food = GenerateFood(view, boardWidth, boardHeight);

    // 同步 GameView 中的食物坐标。
    view->food = *food;

    return true;
}