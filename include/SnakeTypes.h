#pragma once

struct Position
{
    int x;
    int y;
};

struct SnakeNode
{
    Position position;
    SnakeNode* next;
};

struct Snake
{
    SnakeNode* head;
    SnakeNode* tail;
    int length;
};

bool SamePosition(Position first, Position second);
void AddSnakeHead(Snake* snake, Position position);
void RemoveSnakeTail(Snake* snake);
void ClearSnake(Snake* snake);
