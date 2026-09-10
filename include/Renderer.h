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

// 功能：保存绘制一帧画面所需要的游戏数据。
// 保存这一帧的数据
class GameVivew
{
private:
    int width;                        // 保存棋盘宽度。
    int height;                       // 保存棋盘高度。
    std::shared_ptr<SnakeHead> snake; // 指向蛇头链表。
    std::vector<Point> Food;          // 保存食物坐标。
    std::vector<Point> bomb;          // 保存炸弹坐标
    int score;                        // 保存当前分数。
    bool GameFlag;                    // 游戏状态
};