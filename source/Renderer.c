#include "Renderer.h"

#include <stdio.h>

#include "Utils.h"

// 功能：显示当前游戏画面。
// 传入参数说明：view 保存棋盘和蛇的数据，gameOver 表示是否已经结束。
// 以后可以增加彩色字符或更大的棋盘。
// 以前的注释：根据游戏状态绘制当前画面。
void Renderer_Draw(const GameView* view, int gameOver)
{
    // x 保存当前正在绘制的横坐标。
    int x;
    // y 保存当前正在绘制的纵坐标。
    int y;

    // 每次绘制前清空上一帧画面。
    Utils_ClearScreen();
    // 显示游戏标题和当前分数。
    printf("NOKIA SNAKE    得分: %d\n", view->score);

    // 从棋盘上边框开始，逐行绘制到下边框。
    for (y = -1; y <= view->height; ++y)
    {
        // 从左边框开始，逐列绘制到右边框。
        for (x = -1; x <= view->width; ++x)
        {
            // display 保存当前位置最终应该显示的字符。
            char display = ' ';
            // snakeIndex 保存正在检查的蛇身下标。
            int snakeIndex;

            // 坐标在棋盘外圈时显示墙壁。
            if (x == -1 || x == view->width || y == -1 || y == view->height)
            {
                display = '#';
            }
            // 坐标与食物相同并且不是边框时显示食物。
            else if (x == view->food.x && y == view->food.y)
            {
                display = '*';
            }

            // 从蛇尾向蛇头检查，保证蛇头可以覆盖重叠位置。
            for (snakeIndex = view->snakeLength - 1; snakeIndex >= 0; --snakeIndex)
            {
                // 判断当前坐标是否属于某一节蛇身。
                if (view->snake[snakeIndex].x == x && view->snake[snakeIndex].y == y)
                {
                    // 下标 0 是蛇头，其他下标是蛇身。
                    display = snakeIndex == 0 ? '@' : 'o';
                }
            }

            // 输出当前格子的字符，但暂不换行。
            putchar(display);
        }
        // 一行绘制完成后换到下一行。
        putchar('\n');
    }

    // 根据游戏是否结束显示不同的操作提示。
    if (gameOver)
    {
        // 游戏结束后提示玩家按回车退出。
        printf("游戏结束！按回车键退出。\n");
    }
    else
    {
        // 游戏进行中提示移动和退出方法。
        printf("W/A/S/D 移动，Q 退出。\n");
    }
}
