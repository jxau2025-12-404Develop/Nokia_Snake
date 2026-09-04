#pragma once

#include "SnakeTypes.h"

bool IsWallCollision(Position position, int boardWidth, int boardHeight);
bool IsSelfCollision(const Snake* snake, Position position);
