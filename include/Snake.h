#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>

/* 表示蛇身中的一个节点，链表头节点就是蛇头。 */
typedef struct SnakeNode
{
    int x; /* 横坐标 */
    int y; /* 纵坐标 */
    // ↑用Point结构体替换
    struct SnakeNode* next; /* 指向下一节蛇身 */
} SnakeNode;

/* 保存游戏运行过程中需要使用的状态。 */
typedef struct
{
    SnakeNode* head; /* 蛇头指针 */
    int dirX;        /* 水平方向增量 */
    int dirY;        /* 垂直方向增量 */
    int foodX;       /* 食物横坐标 */
    int foodY;       /* 食物纵坐标 */
    int score;       /* 当前分数 */
    int gameOver;    /* 游戏结束标志 */
} SnakeGame;

/* 创建并初始化一个蛇身节点。 */
static SnakeNode* snake_create_node(int x, int y)
{
    SnakeNode* node = (SnakeNode*)malloc(sizeof(SnakeNode));

    if (node == NULL)
    {
        return NULL;
    }
    node->x = x;
    node->y = y;
    node->next = NULL;
    return node;
}

/* 将新节点插入链表头部，使它成为新的蛇头。 */
static int snake_push_front(SnakeNode** head, int x, int y)
{
    SnakeNode* node;

    if (head == NULL)
    {
        return -1;
    }
    node = snake_create_node(x, y);
    if (node == NULL)
    {
        return -1;
    }
    node->next = *head;
    *head = node;
    return 0;
}

/* 删除链表最后一个节点，用于蛇正常移动时缩短蛇尾。 */
static void snake_pop_back(SnakeNode** head)
{
    SnakeNode* curr;
    SnakeNode* prev;

    if (head == NULL || *head == NULL)
    {
        return;
    }
    if ((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return;
    }
    prev = NULL;
    curr = *head;
    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    prev->next = NULL;
    free(curr);
}

/* 释放蛇链表中的全部节点，并将头指针置为空。 */
static void snake_destory(SnakeNode** head)
{
    SnakeNode* curr;
    SnakeNode* next;

    if (head == NULL)
    {
        return;
    }
    curr = *head;
    while (curr != NULL)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }
    *head = NULL;
}

#endif