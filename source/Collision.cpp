#include "Collision.h"

bool IsWallCollision(Position position, int boardWidth, int boardHeight)
{
    // 坐标越过棋盘边界，判定为撞墙。
    return position.x < 0 || position.x >= boardWidth || position.y < 0 || position.y >= boardHeight;
}

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
