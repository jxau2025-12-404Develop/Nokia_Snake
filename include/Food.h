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

Food GenerateFood(
    const SnakeGame* game,
    int boardWidth,
    int boardHeight);

bool EatFood(
    Food* food,
    SnakeGame* game,
    int boardWidth,
    int boardHeight);

#ifdef __cplusplus
}
#endif

#endif