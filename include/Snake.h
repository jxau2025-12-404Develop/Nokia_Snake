#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>

#include "Utils.h"

/* 保存整个贪吃蛇游戏的状态。 */
typedef struct SnakeGame
{
    SnakeNode* head; /* 蛇头链表指针。 */
    int dirX;        /* X 方向增量。 */
    int dirY;        /* Y 方向增量。 */
    int foodX;       /* 食物的 X 坐标。 */
    int foodY;       /* 食物的 Y 坐标。 */
    int score;       /* 当前分数。 */
    int gameOver;    /* 游戏结束标志。 */
} SnakeGame;

/* 创建并初始化一个蛇身节点。 */
static SnakeNode* snake_create_node(int x, int y);

/* 将新节点插入链表头部，使它成为新的蛇头。 */
static int snake_push_front(SnakeNode** head, int x, int y);

/* 删除链表最后一个节点，用于蛇正常移动时缩短蛇尾。 */
static void snake_pop_back(SnakeNode** head);

/* 释放蛇链表中的全部节点，并将头指针置为空。 */
static void snake_destroy_game(SnakeNode** head);

#endif