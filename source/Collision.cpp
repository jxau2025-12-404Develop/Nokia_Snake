#include "Collision.h"

// 函数作用：判断指定坐标是否超出棋盘边界。
// 函数传参：x 和 y 为待检测坐标，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：坐标超出棋盘范围时返回 1，否则返回 0。
int IsWallCollision(int x, int y, int boardWidth, int boardHeight)
{
    // Renderer 的有效棋盘范围为：
    // x：0 到 boardWidth - 1
    // y：0 到 boardHeight - 1
    if (boardWidth <= 0 || boardHeight <= 0)
    {
        return 1;
    }

    if (x < 0 || x >= boardWidth ||
        y < 0 || y >= boardHeight)
    {
        return 1;
    }

    return 0;
}

// 函数作用：判断指定坐标是否与蛇身发生碰撞。
// 函数传参：game 为当前游戏状态，x 和 y 为待检测坐标。
// 返回值：坐标与蛇身重合时返回 1，否则返回 0。
int IsSelfCollision(const SnakeGame* game, int x, int y)
{
    const SnakeNode* node;

    if (game == NULL)
    {
        return 0;
    }

    // 遍历 SnakeGame 中已有的蛇身链表。
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