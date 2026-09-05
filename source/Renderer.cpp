#include "Renderer.h"
#include "Utils.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

// 保存每一帧之间需要等待的毫秒数。
static unsigned int g_frameDurationMilliseconds = 100;

// 功能：清空控制台画面。
// 传入参数说明：不需要传入参数。
// 以后可以使用更平滑的局部刷新方式。
// 以前的注释：每次绘制前清空屏幕。
static void ClearScreen(void)
{
#ifdef _WIN32
    // Windows 使用 cls 命令清空屏幕。
    system("cls");
#else
    // Linux 和 macOS 使用 ANSI 转义序列清空屏幕。
    printf("\033[2J\033[H");
#endif
}

// 功能：判断坐标是否位于棋盘内部。
// 传入参数说明：point 是需要判断的坐标，width 和 height 是棋盘尺寸。
// 以后可以增加坐标碰撞检测。
// 以前的注释：判断坐标是否位于有效的游戏区域内。
static int IsInside(Point point, int width, int height)
{
    // 横坐标和纵坐标都有效时，返回 1。
    if (point.x >= 0 && point.x < width && point.y >= 0 && point.y < height)
    {
        return 1;
    }

    // 坐标超出棋盘范围时，返回 0。
    return 0;
}

// 功能：输出开始菜单。
// 传入参数说明：不需要传入参数。
// 以后可以增加菜单选项选择功能。
// 以前的注释：开始菜单只包含操作提示。
static void RenderStartMenu(void)
{
    // 输出菜单上边框。
    printf("==============================\n");
    // 输出游戏名称。
    printf("          NOKIA SNAKE\n");
    // 输出菜单分隔线。
    printf("==============================\n\n");
    // 输出开始游戏提示。
    printf("          [ ENTER ] 开始游戏\n");
    // 输出退出游戏提示。
    printf("          [ Q     ] 退出游戏\n\n");
    // 输出移动提示。
    printf("        W A S D / 方向键移动\n");
}

// 功能：输出游戏进行中的棋盘。
// 传入参数说明：view 保存棋盘、蛇、食物和分数。
// 以后可以增加颜色和更多游戏对象。
// 以前的注释：游戏进行中显示棋盘和当前分数。
static void RenderPlaying(const GameView* view)
{
    // 保存当前绘制的纵坐标。
    int y;
    // 保存当前绘制的横坐标。
    int x;

    // 输出游戏标题和分数。
    printf("NOKIA SNAKE    得分: %d\n", view->score);

    // 从上边框开始逐行绘制棋盘。
    for (y = -1; y <= view->height; ++y)
    {
        // 从左边框开始逐列绘制当前行。
        for (x = -1; x <= view->width; ++x)
        {
            // 保存当前位置要显示的字符，默认显示空格。
            char display = ' ';
            // 保存当前检查的蛇身节点和位置。
            const RendererSnakeNode* snakeNode;
            int snakeIndex;

            // 检查当前位置是否属于棋盘边框。
            if (x == -1 || x == view->width || y == -1 || y == view->height)
            {
                // 使用 # 表示墙壁。
                display = '#';
            }
            // 检查当前位置是否是食物。
            else if (x == view->food.x && y == view->food.y)
            {
                // 使用 * 表示食物。
                display = '*';
            }

            // 沿蛇身链表检查坐标。
            snakeNode = view->snake;
            snakeIndex = 0;
            while (snakeNode != NULL && snakeIndex < view->snakeLength)
            {
                // 判断当前坐标是否是某一节蛇身。
                if (snakeNode->position.x == x && snakeNode->position.y == y)
                {
                    // 下标 0 是蛇头，其他下标是蛇身。
                    display = snakeIndex == 0 ? '@' : 'o';
                }

                snakeNode = snakeNode->next;
                ++snakeIndex;
            }

            // 输出当前格子，但暂时不换行。
            putchar(display);
        }

        // 当前行输出完成后换行。
        putchar('\n');
    }

    // 输出游戏操作提示。
    printf("W/A/S/D 移动，Q 退出。\n");
}

// 功能：输出暂停界面。
// 传入参数说明：view 保存暂停前的游戏数据。
// 以后可以增加继续、重新开始和退出选项。
// 以前的注释：暂停界面保留当前棋盘并追加暂停提示。
static void RenderPaused(const GameView* view)
{
    // 先输出当前棋盘。
    RenderPlaying(view);
    // 输出空行，让提示与棋盘分开。
    printf("\n");
    // 输出暂停状态。
    printf("              游戏已暂停\n");
    // 输出继续游戏提示。
    printf("          按空格键继续游戏\n");
}

// 功能：输出游戏结束界面。
// 传入参数说明：view 保存最终分数和排行榜，ranking 暂时未单独传入。
// 以后可以显示真实的历史排行榜。
// 以前的注释：显示游戏结束信息和排行榜。
static void RenderGameOver(const GameView* view)
{
    // 输出结束界面的上边框。
    printf("==============================\n");
    // 输出结束标题。
    printf("          游戏结束\n\n");
    // 输出本局分数。
    printf("             得分: %d\n\n", view->score);
    // 输出排行榜标题。
    printf("             排行榜\n");
    // 当前 C 风格数据结构没有保存排行榜数组，因此显示占位文字。
    printf("             暂无记录\n\n");
    // 输出重新开始提示。
    printf("          按 Enter 重新开始\n");
    // 输出退出提示。
    printf("          按 Q     退出游戏\n");
    // 输出结束界面的下边框。
    printf("==============================\n");
}

// 功能：根据界面状态绘制当前画面。
// 传入参数说明：view 保存游戏数据，state 表示当前界面。
// 以后可以增加错误状态提示。
// 以前的注释：根据界面状态绘制当前画面。
void Renderer_Render(const GameView* view, ScreenState state)
{
    // 每次绘制前清空旧画面。
    ClearScreen();

    // 根据当前界面状态调用对应的绘制函数。
    switch (state)
    {
    case SCREEN_START_MENU:
        // 绘制开始菜单。
        RenderStartMenu();
        break;
    case SCREEN_PLAYING:
        // 绘制游戏进行界面。
        RenderPlaying(view);
        break;
    case SCREEN_PAUSED:
        // 绘制游戏暂停界面。
        RenderPaused(view);
        break;
    case SCREEN_GAME_OVER:
        // 绘制游戏结束界面。
        RenderGameOver(view);
        break;
    default:
        // 未知状态不绘制游戏内容。
        printf("未知的界面状态。\n");
        break;
    }
}

// 功能：设置每秒需要绘制的帧数。
// 传入参数说明：framesPerSecond 是目标帧率。
// 以后可以增加最大帧率限制。
// 以前的注释：设置帧率，并计算每帧时间。
void Renderer_SetFrameRate(unsigned int framesPerSecond)
{
    // 防止传入 0 导致除零错误。
    if (framesPerSecond == 0)
    {
        // 使用 1 帧每秒作为最小帧率。
        framesPerSecond = 1;
    }

    // 将每秒帧数转换为每帧等待的毫秒数。
    g_frameDurationMilliseconds = 1000 / framesPerSecond;
}

// 功能：等待到下一帧应该绘制的时间。
// 传入参数说明：不需要传入参数。
// 以后可以根据实际绘制耗时进行精确补偿。
// 以前的注释：等待下一帧可绘制。
void Renderer_WaitForNextFrame(void)
{
#ifdef _WIN32
    // Windows 使用 Sleep 按毫秒等待。
    Sleep(g_frameDurationMilliseconds);
#else
    // 非 Windows 系统暂时使用标准库的秒级等待方式。
    unsigned int seconds = g_frameDurationMilliseconds / 1000;
    // 等待计算出的整秒数。
    while (seconds > 0)
    {
        // 使用 system 的 sleep 命令避免额外依赖。
        system("sleep 1");
        // 减少剩余等待秒数。
        --seconds;
    }
#endif
}
