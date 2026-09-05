#include "Food.h"

#include <stdexcept>
#include <vector>

#include "Utils.h"

namespace
{
    // 函数作用：判断指定位置是否被蛇身占用。
    // 函数传参：snake 为待检测的蛇，position 为待检测的位置。
    // 返回值：位置被蛇身占用时返回 true，否则返回 false。
    bool IsSnakePosition(const Snake* snake, Position position)
    {
        for (SnakeNode* node = snake->head;
             node != nullptr;
             node = node->next)
        {
            if (SamePosition(node->position, position))
                return true;
        }

        return false;
    }
}

// 函数作用：在棋盘内部生成一个不在蛇身上的随机食物位置。
// 函数传参：snake 为当前蛇对象，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：返回生成的食物对象；没有空闲格子时抛出异常。
Food GenerateFood(const Snake* snake, int boardWidth, int boardHeight)
{
    if (snake == nullptr || boardWidth <= 0 || boardHeight <= 0)
        throw std::invalid_argument("棋盘大小或蛇指针无效");

    // 收集所有没有被蛇身占用的棋盘位置。
    std::vector<Position> freePositions;

    // Renderer 中有效的游戏区域是：
    // x = 0 到 boardWidth - 1
    // y = 0 到 boardHeight - 1
    for (int y = 0; y < boardHeight; ++y)
    {
        for (int x = 0; x < boardWidth; ++x)
        {
            Position position{x, y};

            if (!IsSnakePosition(snake, position))
                freePositions.push_back(position);
        }
    }

    // 没有空闲位置时，表示蛇已经占满整个棋盘。
    if (freePositions.empty())
        throw std::runtime_error("棋盘没有可用的食物位置");

    // 从所有空闲位置中随机选择一个位置。
    const std::size_t randomIndex =
        static_cast<std::size_t>(Utils::Random::Random() - 1) %
        freePositions.size();

    Food food{};
    food.position = freePositions[randomIndex];

    return food;
}

// 函数作用：判断蛇头是否吃到食物，并在吃到食物时更新蛇身、分数和食物位置。
// 函数传参：food 为食物对象，headPosition 为蛇头位置，snake 为当前蛇对象，score 为当前分数，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：蛇头吃到食物时返回 true，否则返回 false。
bool EatFood(
    Food* food,
    Position headPosition,
    Snake* snake,
    int* score,
    int boardWidth,
    int boardHeight)
{
    if (food == nullptr ||
        snake == nullptr ||
        score == nullptr ||
        !SamePosition(food->position, headPosition))
    {
        return false;
    }

    // 蛇头吃到食物后，增加一个蛇头节点。
    AddSnakeHead(snake, headPosition);

    // 增加得分。
    *score += 10;

    // 在新的空闲位置生成食物。
    *food = GenerateFood(snake, boardWidth, boardHeight);

    return true;
}