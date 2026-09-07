#include "Renderer.h" // 引入渲染器接口和游戏视图定义。
#include <raylib.h>   // 引入 raylib 绘图接口。

#include <algorithm> // 引入 max 和 min 等算法工具。
#include <cmath>     // 引入数学库支持。

static unsigned int g_frameDurationMilliseconds = 100; // 保存每帧目标间隔，单位为毫秒。
static unsigned int g_targetFramesPerSecond = 10;      // 保存目标帧率。
static constexpr int kWindowWidth = 960;               // 定义逻辑窗口宽度。
static constexpr int kWindowHeight = 720;              // 定义逻辑窗口高度。
static constexpr int kHudHeight = 72;                  // 定义顶部 HUD 高度。

static void EnsureWindow(void) // 确保 raylib 窗口已经初始化。
{
    if (!IsWindowReady()) // 仅在窗口尚未准备好时执行初始化。
    {
        InitWindow(kWindowWidth, kWindowHeight, "Nokia Snake");  // 创建游戏窗口并设置标题。
        SetTargetFPS(static_cast<int>(g_targetFramesPerSecond)); // 应用当前目标帧率。
    }
}

static int SafeDimension(int dimension) // 将无效尺寸修正为最小有效尺寸。
{
    return std::max(dimension, 1); // 保证尺寸至少为 1。
}

static float CellSize(const GameView* view) // 计算棋盘单元格的显示尺寸。
{
    const int width = SafeDimension(view == nullptr ? 1 : view->width);   // 获取并校正棋盘宽度。
    const int height = SafeDimension(view == nullptr ? 1 : view->height); // 获取并校正棋盘高度。
    return std::min((kWindowWidth - 48.0f) / width,
                    (kWindowHeight - kHudHeight - 48.0f) / height); // 取横向和纵向可用尺寸中的较小值。
}

static Vector2 BoardOrigin(const GameView* view, float cellSize) // 计算棋盘左上角坐标。
{
    const int width = SafeDimension(view == nullptr ? 1 : view->width);            // 获取并校正棋盘宽度。
    const int height = SafeDimension(view == nullptr ? 1 : view->height);          // 获取并校正棋盘高度。
    return {(kWindowWidth - width * cellSize) * 0.5f,                              // 计算水平方向居中位置。
            kHudHeight + (kWindowHeight - kHudHeight - height * cellSize) * 0.5f}; // 计算 HUD 下方的垂直居中位置。
}

static void DrawCenteredText(const char* text, float y, int fontSize, Color color) // 按逻辑窗口比例绘制居中文本。
{
    const int renderWidth = GetRenderWidth();   // 获取当前渲染目标宽度。
    const int renderHeight = GetRenderHeight(); // 获取当前渲染目标高度。
    const float scale =                         // 计算逻辑窗口到实际窗口的等比例缩放值。
        std::min(renderWidth / static_cast<float>(kWindowWidth),
                 renderHeight / static_cast<float>(kWindowHeight));             // 取宽高缩放比例中的较小值。
    const float offsetX = (renderWidth - kWindowWidth * scale) * 0.5f;          // 计算水平方向留白。
    const float offsetY = (renderHeight - kWindowHeight * scale) * 0.5f;        // 计算垂直方向留白。
    const int scaledFontSize = std::max(1, static_cast<int>(fontSize * scale)); // 计算缩放后的字体大小。
    const int textWidth = MeasureText(text, scaledFontSize);                    // 测量缩放后文本宽度。
    DrawText(text, static_cast<int>(offsetX + (kWindowWidth * scale - textWidth) * 0.5f),
             static_cast<int>(offsetY + y * scale), scaledFontSize, color); // 绘制居中的缩放文本。
}

static void DrawBoard(const GameView* view) // 绘制棋盘、食物和蛇。
{
    if (view == nullptr) // 没有游戏数据时显示等待提示。
    {
        DrawCenteredText("Waiting for game data", 330.0f, 28, LIGHTGRAY); // 绘制等待数据提示。
        return;                                                           // 结束本次绘制，避免访问空指针。
    }

    const int width = SafeDimension(view->width);                                      // 获取棋盘宽度。
    const int height = SafeDimension(view->height);                                    // 获取棋盘高度。
    const float cellSize = CellSize(view);                                             // 计算单元格尺寸。
    const Vector2 origin = BoardOrigin(view, cellSize);                                // 计算棋盘起始坐标。
    const Rectangle board = {origin.x, origin.y, width * cellSize, height * cellSize}; // 创建棋盘矩形区域。

    DrawRectangleRec(board, Color{24, 38, 42, 255});            // 绘制棋盘背景。
    DrawRectangleLinesEx(board, 3.0f, Color{78, 183, 72, 255}); // 绘制棋盘边框。

    for (int x = 1; x < width; ++x) // 遍历内部竖向网格线。
    {
        DrawLineV({origin.x + x * cellSize, origin.y}, {origin.x + x * cellSize, origin.y + height * cellSize},
                  Color{35, 59, 61, 255}); // 绘制当前竖向网格线。
    }
    for (int y = 1; y < height; ++y) // 遍历内部横向网格线。
    {
        DrawLineV({origin.x, origin.y + y * cellSize}, {origin.x + width * cellSize, origin.y + y * cellSize},
                  Color{35, 59, 61, 255}); // 绘制当前横向网格线。
    }

    if (view->food.x >= 0 && view->food.x < width && view->food.y >= 0 &&
        view->food.y < height) // 仅绘制位于棋盘内的食物。
    {
        const Vector2 foodCenter = {origin.x + (view->food.x + 0.5f) * cellSize,
                                    origin.y + (view->food.y + 0.5f) * cellSize}; // 计算食物中心坐标。
        DrawCircleV(foodCenter, cellSize * 0.28f, Color{231, 76, 60, 255});       // 绘制食物主体。
        DrawCircleLines(static_cast<int>(foodCenter.x), static_cast<int>(foodCenter.y), cellSize * 0.28f,
                        Color{255, 170, 90, 255}); // 绘制食物外圈。
    }

    const SnakeNode* node = view->snake;                 // 从蛇头节点开始遍历蛇身。
    int index = 0;                                       // 初始化蛇节点下标。
    while (node != nullptr && index < view->snakeLength) // 遍历有效的蛇节点。
    {
        if (node->xy.x >= 0 && node->xy.x < width && node->xy.y >= 0 && node->xy.y < height) // 仅绘制位于棋盘内的节点。
        {
            const Rectangle segment = {origin.x + node->xy.x * cellSize + 2.0f, origin.y + node->xy.y * cellSize + 2.0f,
                                       cellSize - 4.0f, cellSize - 4.0f}; // 计算蛇节点的显示矩形。
            DrawRectangleRounded(segment, 0.25f, 4,
                                 index == 0 ? Color{117, 220, 89, 255} : Color{67, 160, 71, 255}); // 绘制蛇头或蛇身。
            if (index == 0)                                                                        // 蛇头额外绘制眼睛。
            {
                DrawCircle(static_cast<int>(segment.x + segment.width * 0.68f),
                           static_cast<int>(segment.y + segment.height * 0.30f), std::max(2.0f, cellSize * 0.07f),
                           BLACK); // 绘制蛇头的眼睛。
            }
        }
        node = node->Next; // 移动到下一个蛇节点。
        ++index;           // 增加节点下标。
    }
}

static void DrawPlaying(const GameView* view, const char* status) // 绘制游戏进行中的界面。
{
    DrawText("NOKIA SNAKE", 24, 18, 30, Color{117, 220, 89, 255});                              // 绘制游戏标题。
    DrawText(TextFormat("SCORE %d", view == nullptr ? 0 : view->score), 720, 24, 22, RAYWHITE); // 绘制当前分数。
    DrawBoard(view);                                                                            // 绘制游戏棋盘。
    if (status != nullptr) // 有状态提示时绘制提示文本。
    {
        DrawCenteredText(status, 680.0f, 22, LIGHTGRAY); // 绘制底部状态提示。
    }
}

// 功能：根据界面状态绘制当前画面。
// 传入参数说明：view 保存游戏数据，state 表示当前界面。
// 以后可以增加错误状态提示。
// 以前的注释：根据界面状态绘制当前画面。
void Renderer_Render(const GameView* view, ScreenState state) // 根据界面状态绘制当前画面。
{
    EnsureWindow();                          // 确保绘制前窗口已经初始化。
    BeginDrawing();                          // 开始本帧绘制。
    ClearBackground(Color{12, 20, 24, 255}); // 清除上一帧并绘制背景色。

    switch (state) // 根据当前界面状态选择绘制内容。
    {
    case SCREEN_START_MENU:                                                      // 绘制开始菜单。
        DrawCenteredText("NOKIA SNAKE", 180.0f, 64, Color{117, 220, 89, 255});   // 绘制游戏标题。
        DrawCenteredText("PRESS ENTER TO START", 320.0f, 28, RAYWHITE);          // 绘制开始提示。
        DrawCenteredText("W A S D / ARROW KEYS TO MOVE", 370.0f, 20, LIGHTGRAY); // 绘制移动提示。
        DrawCenteredText("Q TO QUIT", 410.0f, 20, LIGHTGRAY);                    // 绘制退出提示。
        break;                                                                   // 结束开始菜单分支。
    case SCREEN_PLAYING:                                                         // 绘制游戏进行界面。
        DrawPlaying(view, "W A S D / ARROW KEYS TO MOVE    Q TO QUIT");          // 绘制棋盘和操作提示。
        break;                                                                   // 结束游戏进行分支。
    case SCREEN_PAUSED:                                                          // 绘制暂停界面。
        DrawPlaying(view, "PAUSED - PRESS SPACE TO CONTINUE");                   // 绘制棋盘和暂停提示。
        DrawRectangle(0, 0, kWindowWidth, kWindowHeight, Color{0, 0, 0, 110});   // 绘制半透明遮罩。
        DrawCenteredText("PAUSED", 310.0f, 52, RAYWHITE);                        // 绘制暂停标题。
        break;                                                                   // 结束暂停分支。
    case SCREEN_GAME_OVER:                                                       // 绘制游戏结束界面。
        DrawCenteredText("GAME OVER", 190.0f, 56, Color{231, 76, 60, 255});      // 绘制结束标题。
        DrawCenteredText(TextFormat("SCORE %d", view == nullptr ? 0 : view->score), 300.0f, 30,
                         RAYWHITE);                                        // 绘制最终分数。
        DrawCenteredText("PRESS ENTER TO RESTART", 390.0f, 24, LIGHTGRAY); // 绘制重新开始提示。
        DrawCenteredText("Q TO QUIT", 430.0f, 22, LIGHTGRAY);              // 绘制退出提示。
        break;                                                             // 结束游戏结束分支。
    case SCREEN_USER:                                                      // 绘制用户菜单。
        DrawCenteredText("USER", 190.0f, 52, Color{117, 220, 89, 255});    // 绘制用户菜单标题。
        DrawCenteredText("L  LOGIN", 330.0f, 26, RAYWHITE);                // 绘制登录选项。
        DrawCenteredText("S  SIGN UP", 380.0f, 26, RAYWHITE);              // 绘制注册选项。
        break;                                                             // 结束用户菜单分支。
    default:                                                               // 处理未识别的界面状态。
        DrawCenteredText("UNKNOWN SCREEN", 330.0f, 28, LIGHTGRAY);         // 绘制未知状态提示。
        break;                                                             // 结束默认分支。
    }

    EndDrawing(); // 提交本帧绘制内容。
}

// 功能：设置每秒需要绘制的帧数。
// 传入参数说明：framesPerSecond 是目标帧率。
// 以后可以增加最大帧率限制。
// 以前的注释：设置帧率，并计算每帧时间。
void Renderer_SetFrameRate(unsigned int framesPerSecond) // 设置目标帧率并计算帧间隔。
{
    if (framesPerSecond == 0) // 防止传入零帧率。
    {
        framesPerSecond = 1; // 将零帧率修正为每秒一帧。
    }

    g_frameDurationMilliseconds = std::max(1u, 1000u / framesPerSecond); // 计算每帧目标间隔。
    g_targetFramesPerSecond = framesPerSecond;                           // 保存新的目标帧率。
    if (IsWindowReady())                                                 // 窗口已存在时立即更新 raylib 帧率。
    {
        SetTargetFPS(static_cast<int>(framesPerSecond)); // 将新帧率应用到 raylib。
    }
}

// 功能：等待到下一帧应该绘制的时间。
// 传入参数说明：不需要传入参数。
// 以后可以根据实际绘制耗时进行精确补偿。
// 以前的注释：等待下一帧可绘制。
void Renderer_WaitForNextFrame(void) // 等待到下一帧应该绘制的时间。
{
    if (!IsWindowReady()) // 窗口未初始化时无需等待。
    {
        return; // 直接结束等待函数。
    }
}
