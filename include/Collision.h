#pragma once

#include "SnakeTypes.h"

// 判断指定位置是否与游戏边界发生碰撞。
// 当 position 超出 [0, boardWidth) 和 [0, boardHeight) 范围时返回 true。
bool IsWallCollision(Position position, int boardWidth, int boardHeight);
bool IsSelfCollision(const Snake* snake, Position position);
