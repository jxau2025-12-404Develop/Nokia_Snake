#ifndef FOOD_H
#define FOOD_H

#include "Snake.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    int x;
    int y;
} Food;

// 函数作用：在棋盘内部生成一个不与蛇身重叠的食物。
// 函数传参：game 为当前游戏状态，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：返回生成的食物；没有空闲格子或参数无效时抛出异常。
Food GenerateFood(const SnakeGame* game, int boardWidth, int boardHeight);

// 函数作用：判断指定坐标是否与食物位置相同。
// 函数传参：food 为食物对象，x 和 y 为待检测坐标。
// 返回值：坐标与食物重合时返回 1，否则返回 0。
int IsFoodEaten(const Food* food, int x, int y);

// 函数作用：判断蛇头是否吃到食物，吃到后增加分数并生成新食物。
// 函数传参：food 为食物对象，game 为当前游戏状态，headX 和 headY 为蛇头坐标，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：吃到食物时返回 1，否则返回 0。
int EatFood(
    Food* food,
    SnakeGame* game,
    int headX,
    int headY,
    int boardWidth,
    int boardHeight);

#ifdef __cplusplus
}
#endif

#endif