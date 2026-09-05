#pragma once

// 规定蛇最多可以有多少节身体。
// 无参数
// 以后可以把棋盘大小和蛇的最大长度放到配置文件中。
// 无以前注释
#define RENDERER_MAX_SNAKE_LENGTH 128

// 保存棋盘上的一个坐标。
// 传入参数说明：x 表示横坐标，y 表示纵坐标。
// 以后可以增加坐标相加等常用操作。
// 二维棋盘坐标。
typedef struct
{
    int x; // 保存横坐标。
    int y; // 保存纵坐标。
} Point;

// 定义功能：保存渲染一帧画面所需要的全部数据。
// 传入参数说明：由游戏逻辑填写，再传给 Renderer_Draw。
// 以后可以增加排行榜、暂停状态等显示数据。
// 以前的注释：渲染一帧所需的游戏数据。
typedef struct
{
    int width;                              // 保存棋盘宽度。
    int height;                             // 保存棋盘高度。
    Point snake[RENDERER_MAX_SNAKE_LENGTH]; // 保存蛇的每一节坐标。
    int snakeLength;                        // 保存当前蛇的长度。
    Point food;                             // 保存食物坐标。
    int score;                              // 保存当前分数。
} GameView;

// 功能：显示当前游戏画面。
// 传入参数说明：view 是游戏数据，gameOver 为 1 时显示游戏结束信息。
// 以后可以增加彩色字符或更大的棋盘。
// 以前的注释：根据游戏状态绘制当前画面。
void Renderer_Draw(const GameView* view, int gameOver);
