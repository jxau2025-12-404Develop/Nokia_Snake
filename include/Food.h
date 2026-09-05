#pragma once

#include "SnakeTypes.h"

struct Food
{
    Position position;
};

// 生成一个不在蛇身上的食物位置。
// snake 为当前蛇对象，boardWidth 为游戏区域宽度，boardHeight 为游戏区域高度。
// 返回生成的食物对象。
Food GenerateFood(const Snake* snake, int boardWidth, int boardHeight);

// 判断蛇头是否吃到食物，并在吃到食物时更新蛇身、分数和食物位置。
// food 为食物对象，headPosition 为蛇头位置，snake 为当前蛇对象，score 为当前分数，boardWidth 为游戏区域宽度，boardHeight 为游戏区域高度。
// 蛇头吃到食物时返回 true，否则返回 false。
bool EatFood(Food* food, Position headPosition, Snake* snake, int* score, int boardWidth, int boardHeight);
