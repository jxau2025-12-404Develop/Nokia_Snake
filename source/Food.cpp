#include "Food.h"

#include <stdexcept>

#include "Utils.h"

namespace
{
    // 函数作用：判断指定坐标是否被蛇身占用。
    // 函数传参：game 为当前游戏状态，x 和 y 为待检测坐标。
    // 返回值：被蛇身占用时返回 true，否则返回 false。
    bool IsSnakePosition(const SnakeGame* game, int x, int y)
    {
        const SnakeNode* node = game->head;

        while (node != nullptr)
        {
            if (node->x == x && node->y == y)
            {
                return true;
            }

            node = node->next;
        }

        return false;
    }
}

// 函数作用：生成一个不在蛇身上的随机食物位置。
// 函数传参：game 为当前游戏状态，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：返回生成的食物对象。
Food GenerateFood(
    const SnakeGame* game,
    int boardWidth,
    int boardHeight)
{
    if (game == nullptr || boardWidth <= 0 || boardHeight <= 0)
    {
        throw std::invalid_argument("游戏状态或棋盘大小无效");
    }

    const int boardSize = boardWidth * boardHeight;
    int occupiedCount = 0;

    for (const SnakeNode* node = game->head;
         node != nullptr;
         node = node->next)
    {
        ++occupiedCount;
    }

    // 蛇身占满棋盘时，没有空闲位置。
    if (occupiedCount >= boardSize)
    {
        throw std::runtime_error("棋盘没有可用的食物位置");
    }

    Food food{};

    do
    {
        food.x = (Utils::Random::Random() - 1) % boardWidth;
        food.y = (Utils::Random::Random() - 1) % boardHeight;
    } while (IsSnakePosition(game, food.x, food.y));

    return food;
}

// 函数作用：判断蛇头是否吃到食物并更新分数。
// 函数传参：food 为食物对象，game 为当前游戏状态，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：吃到食物时返回 true，否则返回 false。
bool EatFood(
    Food* food,
    SnakeGame* game,
    int boardWidth,
    int boardHeight)
{
    if (food == nullptr || game == nullptr)
    {
        return false;
    }

    if (game->head == nullptr ||
        game->head->x != food->x ||
        game->head->y != food->y)
    {
        return false;
    }

    // 当前 Food 模块只负责分数和重新生成食物。
    // 蛇身增长应由 Snake 模块负责。
    game->score += 10;

    *food = GenerateFood(game, boardWidth, boardHeight);

    game->foodX = food->x;
    game->foodY = food->y;

    return true;
}