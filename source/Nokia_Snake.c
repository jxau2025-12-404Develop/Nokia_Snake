#include "NoKia_Snake.h"

#include <stdio.h>
#include <string.h>

#include "Renderer.h"
#include "Utils.h"

#define BOARD_WIDTH 30
#define BOARD_HEIGHT 15

// 功能：判断两个坐标是否相同。
// 传入参数说明：first 和 second 是需要比较的两个坐标。
// 可以把坐标判断扩展成通用的碰撞判断。
// 以前的注释：判断位置是否重复。
static int SamePoint(Point first, Point second)
{
    // 横坐标和纵坐标都相同，才表示两个点相同。
    return first.x == second.x && first.y == second.y;
}

// 功能：判断蛇头是否撞到了自己的身体。
// 传入参数说明：game 保存当前游戏数据，head 是移动后的蛇头位置。
// 可以把碰撞检测拆成撞墙和撞身体两个函数。
// 以前的注释：碰撞检测。
static int HitsSnake(const GameView* game, Point head)
{
    // 从下标 1 开始检查，因为下标 0 是蛇头本身。
    int index;
    // 依次检查蛇头以外的每一节身体。
    for (index = 1; index < game->snakeLength; ++index)
    {
        // 如果某一节身体与蛇头重合，就表示撞到自己。
        if (SamePoint(game->snake[index], head))
        {
            // 返回 1 表示发生碰撞。
            return 1;
        }
    }
    // 检查完所有身体仍未重合，表示没有撞到自己。
    return 0;
}

// 功能：随机生成一个不在蛇身上的食物。
// 传入参数说明：game 保存棋盘大小和蛇身数据，生成结果写入 game->food。
// 棋盘被蛇填满时需要增加无食物状态。
// 以前的注释：生成食物。
static void CreateFood(GameView* game)
{
    // index 用来检查随机位置是否与蛇身重合。
    int index;
    // 至少随机生成一次食物位置。
    do
    {
        // 在棋盘的横坐标范围内生成食物。
        game->food.x = Utils_Random(0, game->width - 1);
        // 在棋盘的纵坐标范围内生成食物。
        game->food.y = Utils_Random(0, game->height - 1);
        // 从蛇头开始检查随机位置。
        index = 0;
        // 只要没有检查完蛇身，并且当前节不是食物位置，就继续检查。
        while (index < game->snakeLength && !SamePoint(game->snake[index], game->food))
        {
            // 检查下一节蛇身。
            ++index;
        }
        // 如果 index 等于蛇长，说明食物没有落在蛇身上。
    } while (index < game->snakeLength);
}

// 功能：读取一个方向字符并修改移动方向。
// 传入参数说明：direction 用于接收新的横向和纵向移动方向。
// 可以接入 Windows 方向键或 Linux 非阻塞键盘。
// 以前的注释：读取玩家输入。
static int ReadDirection(Point* direction)
{
    // key 保存玩家输入的一个字符。
    char key;
    // 读取失败时返回 0，表示本次没有得到有效输入。
    if (scanf(" %c", &key) != 1)
    {
        return 0;
    }

    // q 或 Q 表示退出游戏。
    if (key == 'q' || key == 'Q')
    {
        return -1;
    }
    // w 或 W 表示向上移动。
    if (key == 'w' || key == 'W')
    {
        direction->x = 0;
        direction->y = -1;
    }
    // s 或 S 表示向下移动。
    else if (key == 's' || key == 'S')
    {
        direction->x = 0;
        direction->y = 1;
    }
    // a 或 A 表示向左移动。
    else if (key == 'a' || key == 'A')
    {
        direction->x = -1;
        direction->y = 0;
    }
    // d 或 D 表示向右移动。
    else if (key == 'd' || key == 'D')
    {
        direction->x = 1;
        direction->y = 0;
    }
    else
    {
        // 其他字符不是有效方向。
        return 0;
    }
    // 返回 1 表示成功读取方向。
    return 1;
}

// 功能：移动蛇，并处理吃到食物后的成长和加分。
// 传入参数说明：game 保存游戏数据，direction 保存本次移动方向。
// 可以增加速度、暂停和自动移动。
// 以前的注释：更新蛇的位置。
static void MoveSnake(GameView* game, Point direction)
{
    // index 用于从蛇尾向蛇头复制坐标。
    int index;
    // 根据当前蛇头和方向计算移动后的新蛇头。
    Point newHead = {game->snake[0].x + direction.x, game->snake[0].y + direction.y};
    // 判断新蛇头是否移动到了食物位置。
    int eatsFood = SamePoint(newHead, game->food);

    // 吃到食物并且蛇还没有达到数组上限时，增加长度和分数。
    if (eatsFood && game->snakeLength < RENDERER_MAX_SNAKE_LENGTH)
    {
        // 蛇长度增加一节。
        ++game->snakeLength;
        // 分数增加一分。
        ++game->score;
    }

    // 从蛇尾开始，让每一节身体移动到前一节的位置。
    for (index = game->snakeLength - 1; index > 0; --index)
    {
        game->snake[index] = game->snake[index - 1];
    }
    // 把新位置写入蛇头。
    game->snake[0] = newHead;

    // 吃到食物后，在新的空位置生成食物。
    if (eatsFood)
    {
        CreateFood(game);
    }
}

// 功能：初始化一局游戏并运行游戏循环。
// 传入参数说明：无，游戏数据在函数内部创建和维护。
// 以后可以把排行榜保存到文件。
// 以前的注释：程序运行的主函数。
void Nokia_Snake(void)
{
    // game 保存本局棋盘、蛇、食物和分数。
    GameView game;
    // 初始方向设置为向右。
    Point direction = {1, 0};
    // running 为 1 时继续游戏，为 0 时结束游戏。
    int running = 1;

    // 把游戏数据全部初始化为 0，避免使用未初始化的值。
    memset(&game, 0, sizeof(game));
    // 设置棋盘宽度。
    game.width = BOARD_WIDTH;
    // 设置棋盘高度。
    game.height = BOARD_HEIGHT;
    // 设置蛇的初始长度。
    game.snakeLength = 3;
    // 设置蛇头的初始位置。
    game.snake[0] = (Point){BOARD_WIDTH / 2, BOARD_HEIGHT / 2};
    // 设置第一节身体的初始位置。
    game.snake[1] = (Point){BOARD_WIDTH / 2 - 1, BOARD_HEIGHT / 2};
    // 设置第二节身体的初始位置。
    game.snake[2] = (Point){BOARD_WIDTH / 2 - 2, BOARD_HEIGHT / 2};
    // 随机生成第一个食物。
    CreateFood(&game);

    // 只要玩家没有退出并且蛇没有死亡，就持续运行游戏。
    while (running)
    {
        // 保存本次读取方向的结果。
        int inputResult;
        // 显示移动前的画面。
        Renderer_Draw(&game, 0);
        // 读取玩家输入。
        inputResult = ReadDirection(&direction);
        // 收到退出指令时离开游戏循环。
        if (inputResult < 0)
        {
            break;
        }
        // 输入无效时重新显示画面并等待下一次输入。
        if (inputResult == 0)
        {
            continue;
        }

        // 按新的方向移动蛇。
        MoveSnake(&game, direction);
        // 检查蛇头是否撞墙或撞到自己的身体。
        if (game.snake[0].x < 0 || game.snake[0].x >= game.width || game.snake[0].y < 0 ||
            game.snake[0].y >= game.height || HitsSnake(&game, game.snake[0]))
        {
            // 发生碰撞后结束游戏循环。
            running = 0;
        }
    }

    // 显示最后一帧画面和游戏结束提示。
    Renderer_Draw(&game, 1);
    // 等待玩家确认后退出程序。
    Utils_Pause();
}
