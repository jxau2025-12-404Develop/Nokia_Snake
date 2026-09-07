#include "Renderer.h"
#include "Event.h"

#include <raylib.h>

int main()
{
    // 使用数组模拟蛇的链表节点...
    SnakeNode snake[7] = {
        {{12, 10}, &snake[1]}, {{11, 10}, &snake[2]}, {{10, 10}, &snake[3]}, {{9, 10}, &snake[4]},
        {{8, 10}, &snake[5]},  {{8, 11}, &snake[6]},  {{8, 12}, nullptr},
    };

    // 构造一帧测试用的游戏视图数据...
    GameView view = {
        20, 15, snake, 7, {16, 6}, 420, true,
    };

    // 测试程序默认从开始菜单进入。
    ScreenState state = SCREEN_START_MENU;

    // 设置 raylib 窗口创建前必须使用的窗口属性。
    Event_Initialize();

    // 设置目标帧率
    Renderer_SetFrameRate(60);

    // ═════ 关键修改：先渲染一帧以创建窗口 ═════
    // 首次渲染会延迟创建 raylib 窗口，必须先绘制一帧，
    // 窗口创建完成后，才能安全初始化音频设备。
    Renderer_Render(&view, state);

    // ═════ 窗口创建之后，再初始化音频设备 ═════
    InitAudioDevice();
    Music backgroundMusic = LoadMusicStream("music'\'back.mp3");
    const bool musicReady = IsMusicValid(backgroundMusic);
    if (musicReady)
    {
        backgroundMusic.looping = true;
        PlayMusicStream(backgroundMusic);
    }

    // 主循环每一帧按照“查询关闭 -> 处理输入 -> 绘制 -> 帧同步”的顺序执行。
    while (!Event_ShouldClose())
    {
        // Event 模块根据键盘输入更新当前界面状态，并处理全屏切换。
        Event_Process(&state);

        // 持续更新音乐流，保证背景音乐正常播放和循环。
        if (musicReady)
        {
            UpdateMusicStream(backgroundMusic);
        }

        // Renderer 模块将最新状态和游戏数据交给 UI 模块绘制。
        Renderer_Render(&view, state);

        // 保留 Renderer.h 中的等待接口，由 raylib 的 EndDrawing 完成帧同步。
        Renderer_WaitForNextFrame();
    }

    // 程序退出前释放 raylib 创建的窗口和图形资源。
    if (musicReady)
    {
        StopMusicStream(backgroundMusic);
        UnloadMusicStream(backgroundMusic);
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}