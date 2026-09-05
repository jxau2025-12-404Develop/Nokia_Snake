#include "Food.h"

#include <stdexcept>
#include <vector>

#include "Utils.h"

namespace
{
    // 函数作用：判断指定坐标是否被蛇身占用。
    // 函数传参：head 为蛇头节点，x 和 y 为待检测坐标。
    // 返回值：坐标被蛇身占用时返回 true，否则返回 false。
    bool IsSnakePosition(
        const SnakeNode* head,
        int x,
        int y)
    {
        const SnakeNode* node = head;

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

// 函数作用：在棋盘内部生成一个不与蛇身重叠的食物。
// 函数传参：game 为当前游戏状态，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：返回生成的食物；没有空闲格子或参数无效时抛出异常。
Food GenerateFood(
    const SnakeGame* game,
    int boardWidth,
    int boardHeight)
{
    if (game == nullptr || boardWidth <= 0 || boardHeight <= 0)
    {
        throw std::invalid_argument("游戏状态或棋盘大小无效");
    }

    std::vector<Food> freePositions;

    // Renderer 的有效棋盘范围为：
    // x：0 到 boardWidth - 1
    // y：0 到 boardHeight - 1
    for (int y = 0; y < boardHeight; ++y)
    {
        for (int x = 0; x < boardWidth; ++x)
        {
            if (!IsSnakePosition(game->head, x, y))
            {
                Food food{x, y};
                freePositions.push_back(food);
            }
        }
    }

    // 蛇身已经占满棋盘，没有可用位置。
    if (freePositions.empty())
    {
        throw std::runtime_error("棋盘没有可用的食物位置");
    }

    const int randomValue = Utils::Random::Random();

    // Utils::Random::Random() 默认返回 1 到 50。
    // 对异常返回值进行保护，防止出现负数下标。
    const std::size_t randomIndex =
        randomValue > 0
            ? static_cast<std::size_t>(randomValue - 1) %
                  freePositions.size()
            : 0;

    return freePositions[randomIndex];
}

// 函数作用：判断指定坐标是否与食物位置相同。
// 函数传参：food 为食物对象，x 和 y 为待检测坐标。
// 返回值：坐标与食物重合时返回 1，否则