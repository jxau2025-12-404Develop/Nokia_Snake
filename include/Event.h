#pragma once

#include "Renderer.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // 设置窗口创建所需的 raylib 配置。
    void Event_Initialize(void);
    // 处理键盘和窗口模式事件，返回是否完成处理。
    bool Event_Process(ScreenState* state);
    // 查询用户是否请求关闭窗口。
    bool Event_ShouldClose(void);

#ifdef __cplusplus
}
#endif
