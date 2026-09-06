#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>
#include "Utils.h"

/* 创建并初始化一个蛇身节点。 */
static SnakeNode* snake_create_node(int x, int y);

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