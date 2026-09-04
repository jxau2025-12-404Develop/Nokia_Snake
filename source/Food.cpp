#include "Food.h"

#include <stdexcept>

#include "Utils.h"

namespace
{
    bool IsSnakePosition(const Snake* snake, Position position)
    {
        for (SnakeNode* node = snake->head; node != nullptr; node = node->next)
        {
            if (SamePosition(node->position, position))
                return true;
        }
        return false;
    }
}

Food GenerateFood(const Snake* snake, int boardWidth, int boardHeight)
{
    if (snake == nullptr || boardWidth <= 0 || boardHeight <= 0)
        throw std::invalid_argument("棋盘大小或蛇指针无效");

    Food food{};
    do
    {
        // 使用成员1提供的随机数工具生成棋盘坐标。
        food.position.x = (Utils::Random::Random() - 1) % boardWidth;
        food.position.y = (Utils::Random::Random() - 1) % boardHeight;
    } while (IsSnakePosition(snake, food.position));
    return food;
}

bool EatFood(Food* food, Position headPosition, Snake* snake, int* score, int boardWidth, int boardHeight)
{
    if (food == nullptr || snake == nullptr || score == nullptr || !SamePosition(food->position, headPosition))
        return false;

    // 吃到食物后头插一个节点，通知蛇增长一节并上报分数。
    AddSnakeHead(snake, headPosition);
    *score += 10;
    *food = GenerateFood(snake, boardWidth, boardHeight);
    return true;
}
