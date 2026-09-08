#include "Renderer.h"
#include "UI.h"

#include <algorithm>
#include <raylib.h>

static unsigned int g_frameDurationMilliseconds = 100;

// Renderer.cpp 只维护旧 API 的兼容性，具体 UI 实现位于 UI.cpp。
// 保留 Renderer.h 的公共接口，把实际绘制委托给 UI 模块。
void Renderer_Render(const GameView* view, ScreenState state)
{
    UI_Render(view, state);
}

void Renderer_SetFrameRate(unsigned int framesPerSecond)
{
    // 同时保留旧的毫秒间隔计算，并把帧率传给 UI 模块。
    if (framesPerSecond == 0)
    {
        framesPerSecond = 1;
    }

    g_frameDurationMilliseconds = std::max(1u, 1000u / framesPerSecond);
    UI_SetFrameRate(framesPerSecond);
}

void Renderer_WaitForNextFrame(void)
{
    // raylib 的 EndDrawing 已负责帧同步，这里保留原有等待接口。
    if (!IsWindowReady())
    {
        // 窗口尚未创建时不需要额外等待。
        return;
    }
}
