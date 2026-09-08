#include "Event.h"

#include <raylib.h>

namespace
{
    // 只在 F11 从未按下变为按下时触发一次，避免长按重复切换。
    bool IsFullscreenTogglePressed()
    {
        // wasDown 保存上一帧的按键状态，用于把持续按下转换为一次性事件。
        static bool wasDown = false;
        const bool isDown = IsKeyDown(KEY_F11);
        const bool pressed = isDown && !wasDown;
        wasDown = isDown;
        return pressed;
    }

    void UpdateWindowDiagnostics()
    {
        // 将窗口模式和尺寸写入标题栏，方便测试最大化及 F11 是否生效。
        const char* mode = IsWindowFullscreen() ? "FULLSCREEN" : "WINDOWED";
        SetWindowTitle(TextFormat("Nokia Snake | %s | %d x %d%s", mode, GetScreenWidth(), GetScreenHeight(),
                                  IsWindowMaximized() ? " | MAXIMIZED" : ""));
    }
} // namespace

void Event_Initialize(void)
{
    // 配置必须在 raylib 创建窗口前设置，才能启用调整大小和高 DPI。
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
}

bool Event_Process(ScreenState* state)
{
    // 事件模块只更新界面状态和窗口模式，不直接绘制 UI。
    if (state == nullptr)
    {
        // 调用者没有提供状态地址时，不执行任何输入状态修改。
        return false;
    }

    if (IsFullscreenTogglePressed())
    {
        // F11 在窗口模式和全屏模式之间切换。
        ToggleFullscreen();
        UpdateWindowDiagnostics();
        return true;
    }
    if (IsKeyPressed(KEY_ONE))
    {
        // 数字键用于测试不同界面状态。
        *state = SCREEN_START_MENU;
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        *state = SCREEN_PLAYING;
    }
    else if (IsKeyPressed(KEY_THREE))
    {
        *state = SCREEN_PAUSED;
    }
    else if (IsKeyPressed(KEY_FOUR))
    {
        *state = SCREEN_GAME_OVER;
    }
    else if (IsKeyPressed(KEY_FIVE))
    {
        *state = SCREEN_USER;
    }

    UpdateWindowDiagnostics();
    return true;
}

bool Event_ShouldClose(void)
{
    return WindowShouldClose();
}
