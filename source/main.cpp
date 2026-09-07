#include "Renderer.h"
#include "Event.h"

#include <raylib.h>

int main()
{
    // 使用数组模拟蛇的链表节点，数组元素之间通过 Next 指针连接。
    // 第一个节点是蛇头，后面的节点依次表示蛇身。
    SnakeNode snake[7] = {
        {{12, 10}, &snake[1]}, {{11, 10}, &snake[2]}, {{10, 10}, &snake[3]}, {{9, 10}, &snake[4]},
        {{8, 10}, &snake[5]},  {{8, 11}, &snake[6]},  {{8, 12}, nullptr},
    };

    // 构造一帧测试用的游戏视图数据，供 UI 模块绘制棋盘、蛇、食物和分数。
    GameView view = {
        20, 15, snake, 7, {16, 6}, 420, true,
    };

    // 测试程序默认从开始菜单进入。
    ScreenState state = SCREEN_START_MENU;

    // 设置 raylib 窗口创建前必须使用的窗口属性。
    Event_Initialize();

    // 设置目标帧率，并通过 Renderer API 保持对外接口不变。
    Renderer_SetFrameRate(60);

    // 首次渲染会延迟创建 raylib 窗口，因此必须先绘制一帧，
    // 再查询窗口是否需要关闭。
    Renderer_Render(&view, state);

    // 主循环每一帧按照“查询关闭 -> 处理输入 -> 绘制 -> 帧同步”的顺序执行。
    while (!Event_ShouldClose())
    {
        // Event 模块根据键盘输入更新当前界面状态，并处理全屏切换。
        Event_Process(&state);

        // Renderer 模块将最新状态和游戏数据交给 UI 模块绘制。
        Renderer_Render(&view, state);

        // 保留 Renderer.h 中的等待接口，由 raylib 的 EndDrawing 完成帧同步。
        Renderer_WaitForNextFrame();
    }

    // 程序退出前释放 raylib 创建的窗口和图形资源。
    CloseWindow();
    return 0;
}