#include "Renderer.h"

#include <raylib.h>

static bool IsFullscreenTogglePressed(void)
{
    static bool wasDown = false;
    const bool isDown = IsKeyDown(KEY_F11);
    const bool pressed = isDown && !wasDown;
    wasDown = isDown;
    return pressed;
}

static void UpdateWindowDiagnostics(void)
{
    const char* mode = IsWindowFullscreen() ? "FULLSCREEN" : "WINDOWED";
    SetWindowTitle(TextFormat("Nokia Snake | %s | %d x %d%s", mode, GetScreenWidth(), GetScreenHeight(),
                              IsWindowMaximized() ? " | MAXIMIZED" : ""));
}

int main()
{
    SnakeNode snake[7] = {
        {{12, 10}, &snake[1]}, {{11, 10}, &snake[2]}, {{10, 10}, &snake[3]}, {{9, 10}, &snake[4]},
        {{8, 10}, &snake[5]},  {{8, 11}, &snake[6]},  {{8, 12}, nullptr},
    };

    GameView view = {
        20, 15, snake, 7, {16, 6}, 420, true,
    };

    ScreenState state = SCREEN_START_MENU;
    Renderer_SetFrameRate(60);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    Renderer_Render(&view, state);
    UpdateWindowDiagnostics();
    while (!WindowShouldClose())
    {
        if (IsFullscreenTogglePressed())
        {
            ToggleFullscreen();
            UpdateWindowDiagnostics();
        }
        else if (IsKeyPressed(KEY_ONE))
        {
            state = SCREEN_START_MENU;
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            state = SCREEN_PLAYING;
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            state = SCREEN_PAUSED;
        }
        else if (IsKeyPressed(KEY_FOUR))
        {
            state = SCREEN_GAME_OVER;
        }
        else if (IsKeyPressed(KEY_FIVE))
        {
            state = SCREEN_USER;
        }

        UpdateWindowDiagnostics();
        Renderer_Render(&view, state);
        Renderer_WaitForNextFrame();
    }

    CloseWindow();
    return 0;
}