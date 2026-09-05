// Nokia_Snake.cpp
#include "NoKia_Snake.h"
#include "Renderer.h"
#include "Utils.h"

// 重置
void InitGV(GameView& GV)
{
    // 设置长宽为50
    GV.height = 50;
    GV.width = 50;
    GV.score = 0;
}

// 程序运行的主函数
void Nokia_Snake()
{
    // 设置帧率
    Renderer_SetFrameRate(60);
    // 创建棋盘信息
    GameView GV;

    while (true)
    {
        InitGV(GV);

        // 输出
        Renderer_Render(&GV, SCREEN_START_MENU);

        // 使用方法：读取按键，按下立即返回，不需要按回车
        int key = Utils::Input::GetKey();

        // 按 Q / q 退出游戏
        if (key == 'q')
        {
            return;
        }
        else
        {
            continue;
        }

        // // 示例：识别 W/A/S/D 和方向键
        // switch (key)
        // {
        // case 'w':
        // case Utils::Input::KEY_UP:
        //     // TODO: 蛇向上移动
        //     break;
        // case 's':
        // case Utils::Input::KEY_DOWN:
        //     // TODO: 蛇向下移动
        //     break;
        // case 'a':
        // case Utils::Input::KEY_LEFT:
        //     // TODO: 蛇向左移动
        //     break;
        // case 'd':
        // case Utils::Input::KEY_RIGHT:
        //     // TODO: 蛇向右移动
        //     break;
        // default:
        //     // 其它按键暂时忽略
        //     break;
        // }
    }
    return;
}