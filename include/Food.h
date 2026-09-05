#ifndef FOOD_H
#define FOOD_H

#include "Utils.h"

// 函数作用：在棋盘空闲位置生成食物。
// 函数传参：view 为当前游戏画面数据，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：返回生成的食物坐标；棋盘没有空闲位置时抛出异常。
Point GenerateFood(
    const GameView* view,
    int boardWidth,
    int boardHeight);

// 函数作用：判断蛇头是否吃到食物，并更新分数和食物坐标。
// 函数传参：food 为食物坐标，view 为当前游戏画面数据，boardWidth 为棋盘宽度，boardHeight 为棋盘高度。
// 返回值：吃到食物时返回 true，否则返回 false。
bool EatFood(
    Point* food,
    GameView* view,
    int boardWidth,
    int boardHeight);

#endif