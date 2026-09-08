#include "Renderer.h"
#include "Event.h"

#include <raylib.h>

#include "raylib.h"

#include "NoKia_Snake.h"

extern int HIGHT = 50;
extern int WIDTH = 50;

int main()
{
    Utils::init();

    // 设置 raylib 窗口创建前必须使用的窗口属性。
    Event_Initialize();

    // 设置目标帧率
    Renderer_SetFrameRate(60);

    // ═════ 窗口创建之后，再初始化音频设备 ═════
    // 注意：raylib 要求先 InitWindow 再 InitAudioDevice，否则音乐会无声或加载失败。
    // UI_Render 中的 EnsureWindow 会检测窗口是否已创建，因此这里先主动创建一次。
    if (!IsWindowReady())
    {
        InitWindow(960, 720, "Nokia Snake");
        SetTargetFPS(60);
    }

    InitAudioDevice();
    Music backgroundMusic = LoadMusicStream("music/back.mp3");
    const bool musicReady = IsMusicValid(backgroundMusic);
    if (musicReady)
    {
        backgroundMusic.looping = true;
        PlayMusicStream(backgroundMusic);
    }

    // 程序运行的主函数
    Nokia_Snake(backgroundMusic);

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