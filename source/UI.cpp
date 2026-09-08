#include "UI.h"

#include <raylib.h>

#include <algorithm>

namespace
{
    // 所有界面先在固定逻辑画布中绘制，再统一缩放到实际窗口。
    // 这样不同窗口尺寸下的字体、棋盘和菜单仍保持相同的相对位置。
    constexpr int kWindowWidth = 960;
    constexpr int kWindowHeight = 720;
    constexpr int kHudHeight = 72;
    // UI_SetFrameRate 可能在窗口创建前调用，因此先保存目标值。
    unsigned int g_targetFramesPerSecond = 10;

    // 延迟创建窗口，保证 Renderer_Render 仍是唯一的首次绘制入口。
    void EnsureWindow()
    {
        if (!IsWindowReady())
        {
            // 窗口只由第一次绘制初始化，避免在程序启动阶段重复创建窗口。
            InitWindow(kWindowWidth, kWindowHeight, "Nokia Snake");
            SetTargetFPS(static_cast<int>(g_targetFramesPerSecond));
        }
    }

    int SafeDimension(int dimension)
    {
        // 避免无效棋盘尺寸参与除法或生成负的绘制区域。
        return std::max(dimension, 1);
    }

    float CellSize(const GameView* view)
    {
        // 同时限制横向和纵向单元格大小，确保整个棋盘能放进逻辑画布。
        const int width = SafeDimension(view == nullptr ? 1 : view->width);
        const int height = SafeDimension(view == nullptr ? 1 : view->height);
        return std::min((kWindowWidth - 48.0f) / width, (kWindowHeight - kHudHeight - 48.0f) / height);
    }

    Vector2 BoardOrigin(const GameView* view, float cellSize)
    {
        // 将棋盘放在 HUD 下方剩余区域的中心，而不是固定贴在左上角。
        const int width = SafeDimension(view == nullptr ? 1 : view->width);
        const int height = SafeDimension(view == nullptr ? 1 : view->height);
        return {(kWindowWidth - width * cellSize) * 0.5f,
                kHudHeight + (kWindowHeight - kHudHeight - height * cellSize) * 0.5f};
    }

    void DrawCenteredText(const char* text, float y, int fontSize, Color color)
    {
        // 先测量文字宽度，再计算起点，使文字中心对齐逻辑画布中心。
        const int textWidth = MeasureText(text, fontSize);
        DrawText(text, (kWindowWidth - textWidth) / 2, static_cast<int>(y), fontSize, color);
    }

    void DrawBoard(const GameView* view)
    {
        // 棋盘坐标、食物坐标和蛇节点都使用 GameView 提供的真实数据。
        if (view == nullptr)
        {
            DrawCenteredText("Waiting for game data", 330.0f, 28, LIGHTGRAY);
            return;
        }

        const int width = SafeDimension(view->width);
        const int height = SafeDimension(view->height);
        // 所有棋盘元素都使用同一组 origin 和 cellSize，避免对象相互错位。
        const float cellSize = CellSize(view);
        const Vector2 origin = BoardOrigin(view, cellSize);
        const Rectangle board = {origin.x, origin.y, width * cellSize, height * cellSize};

        DrawRectangleRec(board, Color{24, 38, 42, 255});
        DrawRectangleLinesEx(board, 3.0f, Color{78, 183, 72, 255});

        for (int x = 1; x < width; ++x)
        {
            // 从 1 开始绘制网格线，外框由 DrawRectangleLinesEx 绘制。
            DrawLineV({origin.x + x * cellSize, origin.y}, {origin.x + x * cellSize, origin.y + height * cellSize},
                      Color{35, 59, 61, 255});
        }
        for (int y = 1; y < height; ++y)
        {
            DrawLineV({origin.x, origin.y + y * cellSize}, {origin.x + width * cellSize, origin.y + y * cellSize},
                      Color{35, 59, 61, 255});
        }

        if (view->food.x >= 0 && view->food.x < width && view->food.y >= 0 && view->food.y < height)
        {
            // 食物坐标来自当前帧 GameView，越界数据直接忽略。
            const Vector2 foodCenter = {origin.x + (view->food.x + 0.5f) * cellSize,
                                        origin.y + (view->food.y + 0.5f) * cellSize};
            DrawCircleV(foodCenter, cellSize * 0.28f, Color{231, 76, 60, 255});
            DrawCircleLines(static_cast<int>(foodCenter.x), static_cast<int>(foodCenter.y), cellSize * 0.28f,
                            Color{255, 170, 90, 255});
        }

        const SnakeNode* node = view->snake;
        int index = 0;
        // 按链表顺序绘制蛇，index 为 0 的节点使用蛇头颜色。
        while (node != nullptr && index < view->snakeLength)
        {
            if (node->xy.x >= 0 && node->xy.x < width && node->xy.y >= 0 && node->xy.y < height)
            {
                const Rectangle segment = {origin.x + node->xy.x * cellSize + 2.0f,
                                           origin.y + node->xy.y * cellSize + 2.0f, cellSize - 4.0f, cellSize - 4.0f};
                DrawRectangleRounded(segment, 0.25f, 4,
                                     index == 0 ? Color{117, 220, 89, 255} : Color{67, 160, 71, 255});
                if (index == 0)
                {
                    DrawCircle(static_cast<int>(segment.x + segment.width * 0.68f),
                               static_cast<int>(segment.y + segment.height * 0.30f), std::max(2.0f, cellSize * 0.07f),
                               BLACK);
                }
            }
            node = node->Next;
            ++index;
        }
    }

    void DrawPlaying(const GameView* view, const char* status)
    {
        // 游戏进行界面由 HUD、棋盘和底部状态提示三部分组成。
        DrawText("NOKIA SNAKE", 24, 18, 30, Color{117, 220, 89, 255});
        DrawText(TextFormat("SCORE %d", view == nullptr ? 0 : view->score), 720, 24, 22, RAYWHITE);
        DrawBoard(view);
        if (status != nullptr)
        {
            DrawCenteredText(status, 680.0f, 22, LIGHTGRAY);
        }
    }

    Camera2D CreateLayoutCamera()
    {
        // 取宽高缩放比例中的较小值，保证画面不变形并在窗口中居中。
        const int windowWidth = GetScreenWidth();
        const int windowHeight = GetScreenHeight();
        const float scale =
            std::min(windowWidth / static_cast<float>(kWindowWidth), windowHeight / static_cast<float>(kWindowHeight));
        // 取较小缩放比例保持宽高比，再把逻辑画布放到窗口剩余空间中央。
        return {{0.0f, 0.0f},
                {(windowWidth - kWindowWidth * scale) * 0.5f, (windowHeight - kWindowHeight * scale) * 0.5f},
                0.0f,
                scale};
    }
} // namespace

void UI_Render(const GameView* view, ScreenState state)
{
    // UI 模块只负责绘制，不改变游戏状态或处理输入。
    EnsureWindow();
    BeginDrawing();
    // 每帧先清理背景，再在逻辑画布中绘制当前界面。
    ClearBackground(Color{12, 20, 24, 255});
    BeginMode2D(CreateLayoutCamera());

    // 根据当前界面状态绘制对应的菜单或游戏画面。
    switch (state)
    {
    case SCREEN_START_MENU:
        DrawCenteredText("NOKIA SNAKE", 180.0f, 64, Color{117, 220, 89, 255});
        DrawCenteredText("PRESS ENTER TO START", 320.0f, 28, RAYWHITE);
        DrawCenteredText("W A S D / ARROW KEYS TO MOVE", 370.0f, 20, LIGHTGRAY);
        DrawCenteredText("Q TO QUIT", 410.0f, 20, LIGHTGRAY);
        break;
    case SCREEN_PLAYING:
        DrawPlaying(view, "W A S D / ARROW KEYS TO MOVE    Q TO QUIT");
        break;
    case SCREEN_PAUSED:
        DrawPlaying(view, "PAUSED - PRESS SPACE TO CONTINUE");
        DrawRectangle(0, 0, kWindowWidth, kWindowHeight, Color{0, 0, 0, 110});
        DrawCenteredText("score" + view->score, 190.0f, 56, Color{231, 76, 60, 255});
        DrawCenteredText("PAUSED", 310.0f, 52, RAYWHITE);
        break;
    case SCREEN_GAME_OVER:
        DrawCenteredText("GAME OVER", 190.0f, 56, Color{231, 76, 60, 255});
        DrawCenteredText(TextFormat("SCORE %d", view == nullptr ? 0 : view->score), 300.0f, 30, RAYWHITE);
        DrawCenteredText("PRESS SPACE TO RESTART", 390.0f, 24, LIGHTGRAY);
        DrawCenteredText("Q TO QUIT", 430.0f, 22, LIGHTGRAY);
        break;
    case SCREEN_USER:
        DrawCenteredText("USER", 190.0f, 52, Color{117, 220, 89, 255});
        DrawCenteredText("L  LOGIN", 330.0f, 26, RAYWHITE);
        DrawCenteredText("S  SIGN UP", 380.0f, 26, RAYWHITE);
        break;
    default:
        DrawCenteredText("UNKNOWN SCREEN", 330.0f, 28, LIGHTGRAY);
        break;
    }

    EndMode2D();
    EndDrawing();
}

void UI_SetFrameRate(unsigned int framesPerSecond)
{
    // 保存并应用目标帧率；窗口尚未创建时由 EnsureWindow 延迟应用。
    g_targetFramesPerSecond = std::max(1u, framesPerSecond);
    if (IsWindowReady())
    {
        SetTargetFPS(static_cast<int>(g_targetFramesPerSecond));
    }
}
