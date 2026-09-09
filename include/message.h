#pragma once

#include <memory>
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
class SnakeHead
{
protected:
    // 基础信息
    SnakeNode Node;
    // 行走方向
    char hir;
    // 蛇身长度
    int snakeLength; // 保存当前蛇的长度。
};

// 功能：保存绘制一帧画面所需要的游戏数据。
// 保存这一帧的数据
struct GameVivew
{
    int width;                        // 保存棋盘宽度。
    int height;                       // 保存棋盘高度。
    std::shared_ptr<SnakeHead> snake; // 指向蛇头链表。
    std::vector<Point> Food;          // 保存食物坐标。
    std::vector<Point> bomb;          // 保存炸弹坐标
    int score;                        // 保存当前分数。
    bool GameFlag;                    // 游戏状态
};