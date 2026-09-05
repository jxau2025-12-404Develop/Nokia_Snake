#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    // 功能：保存棋盘上的一个坐标。
    // 传入参数说明：x 是横坐标，y 是纵坐标。
    // 以后可以增加坐标相加和坐标比较函数。
    // 以前的注释：二维棋盘坐标。
    typedef struct
    {
        int x; // 保存横坐标。
        int y; // 保存纵坐标。
    } Point;

    // 功能：保存一节蛇身及其下一节的位置。
    // 传入参数说明：由游戏逻辑负责维护链表节点的生命周期。
    typedef struct RendererSnakeNode
    {
        Point position;                 // 保存当前蛇节坐标。
        struct RendererSnakeNode* next; // 指向下一节蛇身。
    } RendererSnakeNode;

    // 功能：表示当前需要显示的游戏界面。
    // 传入参数说明：调用者使用其中一个枚举值表示当前界面。
    // 以后可以增加设置界面。
    // 以前的注释：当前要显示的界面。
    typedef enum
    {
        SCREEN_START_MENU, // 开始菜单。
        SCREEN_PLAYING,    // 游戏进行中。
        SCREEN_PAUSED,     // 游戏暂停。
        SCREEN_GAME_OVER   // 游戏结束。
    } ScreenState;

    // 功能：清空屏幕并绘制当前游戏画面。
    // 传入参数说明：view 是当前游戏数据，state 是当前界面状态。
    // 以后可以增加颜色和图形化显示。
    // 以前的注释：根据界面状态绘制当前画面。
    void Renderer_Render(const GameView* view, ScreenState state);

    // 功能：设置目标帧率。
    // 传入参数说明：framesPerSecond 是每秒需要绘制的帧数。
    // 以后可以限制最大帧率。
    // 以前的注释：设置帧率。
    void Renderer_SetFrameRate(unsigned int framesPerSecond);

    // 功能：等待到下一帧应该绘制的时间。
    // 传入参数说明：不需要传入参数。
    // 以后可以使用更精确的定时器。
    // 以前的注释：等待下一帧可绘制。
    void Renderer_WaitForNextFrame(void);

#ifdef __cplusplus
}
#endif
