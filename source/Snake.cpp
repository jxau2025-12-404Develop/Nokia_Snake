#include "Snake.h"

/* 判断目标坐标是否已经被蛇身占用。 */
static int snake_occupies(const SnakeNode* head, int x, int y)
{
    const SnakeNode* curr = head;

    while (curr != NULL)
    {
        if (curr->x == x && curr->y == y)
        {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/* 初始化游戏状态，并创建第一节蛇身。 */
void snake_init(SnakeGame* game, int startX, int startY, int foodX, int foodY)
{
    if (game == NULL)
    {
        return;
    }
    game->head = NULL;
    game->dirX = 1;
    game->dirY = 0;
    game->foodX = foodX;
    game->foodY = foodY;
    game->score = 0;
    game->gameOver = 0;
    snake_push_front(&game->head, startX, startY);
}

/* 设置移动方向，禁止直接反向移动。 */
void snake_set_direction(SnakeGame* game, int dirX, int dirY)
{
    if (game == NULL || (dirX == 0 && dirY == 0))
    {
        return;
    }
    if (dirX == -game->dirX && dirY == -game->dirY)
    {
        return;
    }
    game->dirX = dirX;
    game->dirY = dirY;
}

/* 处理 W/A/S/D 和方向键第二码，空格返回暂停请求。 */
int snake_handle_input(SnakeGame* game, int key)
{
    if (key == 'w' || key == 'W' || key == 72)
    {
        snake_set_direction(game, 0, -1);
    }
    else if (key == 's' || key == 'S' || key == 80)
    {
        snake_set_direction(game, 0, 1);
    }
    else if (key == 'a' || key == 'A' || key == 75)
    {
        snake_set_direction(game, -1, 0);
    }
    else if (key == 'd' || key == 'D' || key == 77)
    {
        snake_set_direction(game, 1, 0);
    }
    else if (key == ' ')
    {
        return 1;
    }
    return 0;
}

/* 按当前方向移动：头插新节点，未吃到食物时删除蛇尾。 */
int snake_move(SnakeGame* game)
{
    SnakeNode* head;
    int nextX;
    int nextY;
    int foodEaten;

    if (game == NULL || game->head == NULL || game->gameOver)
    {
        return 0;
    }
    head = game->head;
    nextX = head->x + game->dirX;
    nextY = head->y + game->dirY;
    if (snake_occupies(game->head, nextX, nextY))
    {
        game->gameOver = 1;
        return 0;
    }
    if (snake_push_front(&game->head, nextX, nextY) != 0)
    {
        game->gameOver = 1;
        return 0;
    }
    foodEaten = nextX == game->foodX && nextY == game->foodY;
    if (foodEaten)
    {
        game->score++;
    }
    else
    {
        snake_pop_back(&game->head);
    }
    return 1;
}

/* 释放游戏中的整条蛇。 */
void snake_destroy_game(SnakeGame* game)
{
    if (game == NULL)
    {
        return;
    }
    snake_destory(&game->head);
    game->gameOver = 1;
}