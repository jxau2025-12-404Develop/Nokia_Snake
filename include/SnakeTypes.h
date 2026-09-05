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

// 函数作用：判断两个位置是否相同。
// 函数传参：first 为第一个待比较的位置，second 为第二个待比较的位置。
// 返回值：两个位置相同时返回 true，否则返回 false。
bool SamePosition(Position first, Position second);

// 函数作用：在蛇头部添加一个新的节点。
// 函数传参：snake 为待添加节点的蛇，position 为新蛇头的位置。
void AddSnakeHead(Snake* snake, Position position);

// 函数作用：移除蛇尾部的节点。
// 函数传参：snake 为待移除蛇尾节点的蛇。
void RemoveSnakeTail(Snake* snake);

// 函数作用：清除蛇的所有节点并释放相关内存。
// 函数传参：snake 为待清除的蛇。
void ClearSnake(Snake* snake);