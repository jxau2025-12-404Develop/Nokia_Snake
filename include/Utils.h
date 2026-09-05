#pragma once

// 功能：初始化随机数和控制台。
// 无参数
// 以后可以在这里设置控制台颜色。
// 以前的注释：初始化输出。
void Utils_Init(void);

// 功能：清空控制台画面。
// 无参数
// 可以改成只刷新发生变化的区域。
// 以前的注释：清屏。
void Utils_ClearScreen(void);

// 功能：等待用户按下回车。
// 无参数
// 以后可以增加任意按键继续功能。
// 以前的注释：等待。
void Utils_Pause(void);

// 功能：生成指定范围内的随机整数。
// 传入参数说明：minimum 是最小值，maximum 是最大值。
// 调用者需要保证 minimum 不大于 maximum。
// 以前的注释：随机数生成。
int Utils_Random(int minimum, int maximum);
