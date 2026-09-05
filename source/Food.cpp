#include "Food.h"

#include <stdexcept>

#include "Utils.h"

namespace
{
    // 函数作用：判断指定位置是否被蛇身占用。
    // 函数传参：snake 为待检测的蛇，position 为待检测的位置。
    // 返回值：位置被蛇身占用时返回 true，否则返回 false。
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

// 函数作用：生成一个不在蛇身上的随机食物位置。
// 函数传参：snake 为当前蛇对象，boardWidth 为游戏区域宽度，boardHeight 为游戏区域高度。
// 返回值：返回生成的食物对象。
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

// 函数作用：判断蛇头是否吃到食物，并在吃到食物时更新蛇身、分数和食物位置。
// 函数传参：food 为食物对象，headPosition 为蛇头位置，snake 为当前蛇对象，score 为当前分数，boardWidth 为游戏区域宽度，boardHeight 为游戏区域高度。
// 返回值：蛇头吃到食物时返回 true，否则返回 false。
bool EatFood(Food* food, Position headPosition, Snake* snake, int* score, int boardWidth, int boardHeight)
{
    if (food == nullptr || snake == nullptr || score == nullptr ||
        !SamePosition(food->position, headPosition))
        return false;

    // 吃到食物后头插一个节点，通知蛇增长一节并上报分数。
    AddSnakeHead(snake, headPosition);
    *score += 10;
    *food = GenerateFood(snake, boardWidth, boardHeight);

    return true;
}
