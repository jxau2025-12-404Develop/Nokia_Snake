#ifndef COLLISION_H
#define COLLISION_H

#include "Snake.h"

#ifdef __cplusplus
extern "C"
{
#endif

int IsWallCollision(int x, int y, int boardWidth, int boardHeight);
int IsSelfCollision(const SnakeGame* game, int x, int y);

#ifdef __cplusplus
}
#endif

#endif