#pragma once

#include "SnakeTypes.h"

struct Food
{
    Position position;
};

Food GenerateFood(const Snake* snake, int boardWidth, int boardHeight);
bool EatFood(Food* food, Position headPosition, Snake* snake, int* score, int boardWidth, int boardHeight);
