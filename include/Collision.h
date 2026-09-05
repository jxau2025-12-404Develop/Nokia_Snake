#ifndef COLLISION_H
#define COLLISION_H

#include "Snake.h"

#ifdef __cplusplus
extern "C"
{
#endif

// 函数作用：判断指定坐标是否超出棋盘边界。
// 函数传参：x 和 y 为待检测坐标，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：坐标超出棋盘范围时返回 1，否则返回 0。
int IsWallCollision(int x, int y, int boardWidth, int boardHeight);

// 函数作用：判断指定坐标是否与蛇身发生碰撞。
// 函数传参：game 为当前游戏状态，x 和 y 为待检测坐标。
// 返回值：坐标与蛇身重合时返回 1，否则返回 0。
int IsSelfCollision(const SnakeGame* game, int x, int y);

#ifdef __cplusplus
}
#endif

#endif