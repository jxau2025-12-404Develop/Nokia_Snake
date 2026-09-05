#include "Collision.h"

// 函数作用：判断指定坐标是否超出游戏边界。
// 函数传参：x 和 y 为待检测坐标，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：超出边界时返回 1，否则返回 0。
int IsWallCollision(int x, int y, int boardWidth, int boardHeight)
{
    return x < 0 ||
           x >= boardWidth ||
           y < 0 ||
           y >= boardHeight;
}

// 函数作用：判断指定坐标是否与蛇身发生碰撞。
// 函数传参：game 为当前游戏状态，x 和 y 为待检测坐标。
// 返回值：与蛇身重合时返回 1，否则返回 0。
int IsSelfCollision(const SnakeGame* game, int x, int y)
{
    const SnakeNode* node;

    if (game == NULL)
    {
        return 0;
    }

    node = game->head;

    while (node != NULL)
    {
        if (node->x == x && node->y == y)
        {
            return 1;
        }

        node = node->next;
    }

    return 0;
}