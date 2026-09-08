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
    InitAudioDevice();
    Music backgroundMusic = LoadMusicStream("music/back.mp3");
    const bool musicReady = IsMusicValid(backgroundMusic);
    if (musicReady)
    {
        backgroundMusic.looping = true;
        PlayMusicStream(backgroundMusic);
    }

    // 程序运行的主函数
    Nokia_Snake();

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