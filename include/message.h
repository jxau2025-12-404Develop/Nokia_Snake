#pragma once

#include <vector>

constexpr int HIGHT = 50;
constexpr int WIDTH = 50;

// 坐标信息
typedef struct Point
{
    // x坐标
    int x;
    // y坐标
    int y;
} Point;

// 蛇身节点
typedef struct Snake
{
    // 蛇身坐标
    Point xy;
    // 下一个蛇身的指针
    SnakeNode* Next;
} SnakeNode;

// 蛇头节点
typedef struct SnakeHead
{
    // 基础信息
    SnakeNode Node;
    // 行走方向
    char hir;
} SnakeHead;

// 功能：保存绘制一帧画面所需要的游戏数据。
// 保存这一帧的数据
