#include "Collision.h"

// 函数作用：判断指定位置是否与游戏边界发生碰撞。
// 函数传参：position 为待检测的位置，boardWidth 为游戏区域宽度，boardHeight 为游戏区域高度。
// 返回值：位置超出游戏边界时返回 true，否则返回 false。
bool IsWallCollision(Position position, int boardWidth, int boardHeight)
{
    // 坐标越过棋盘边界，判定为撞墙。
    return position.x < 0 || position.x >= boardWidth ||
           position.y < 0 || position.y >= boardHeight;
}

// 函数作用：判断指定位置是否与蛇的身体发生碰撞。
// 函数传参：snake 为待检测的蛇，position 为待检测的位置。
// 返回值：位置与蛇身重合时返回 true，否则返回 false。
bool IsSelfCollision(const Snake* snake, Position position)
{
    if (snake == nullptr)
        return false;

    // 遍历链表，检查下一格是否已经被蛇身占用。
    for (SnakeNode* node = snake->head; node != nullptr; node = node->next)
    {
        if (SamePosition(node->position, position))
            return true;
    }

    return false;
}