#pragma once

#include "Renderer.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // 绘制当前界面及 GameView 中的游戏数据。
    void UI_Render(const GameView* view, ScreenState state);
    // 设置 UI 使用的目标帧率。
    void UI_SetFrameRate(unsigned int framesPerSecond);

#ifdef __cplusplus
}
#endif
