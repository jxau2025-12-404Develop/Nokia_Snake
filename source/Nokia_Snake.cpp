// Nokia_Snake.cpp
#include "NoKia_Snake.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <thread>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace
{
    constexpr int boardWidth = 30;
    constexpr int boardHeight = 18;

    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    struct Position
    {
        int x;
        int y;
    };

    struct SnakeNode
    {
        Position position;
        SnakeNode* next; // 指向下一节蛇身，所有节点组成单向链表
    };

    struct Snake
    {
        SnakeNode* head; // 蛇头，移动时在这里插入新节点
        SnakeNode* tail; // 蛇尾，移动时从这里删除节点
        int length;
    };

    struct Food
    {
        Position position;
    };

    bool SamePosition(Position first, Position second)
    {
        return first.x == second.x && first.y == second.y;
    }

    bool IsSnakePosition(const Snake* snake, Position position)
    {
        for (SnakeNode* node = snake->head; node != nullptr; node = node->next)
        {
            if (SamePosition(node->position, position))
                return true;
        }
        return false;
    }

    void AddHead(Snake* snake, Position position)
    {
        // 头插法：新位置成为蛇头，并连接原来的蛇头
        SnakeNode* node = new SnakeNode{position, snake->head};
        snake->head = node;
        if (snake->tail == nullptr)
            snake->tail = node;
        ++snake->length;
    }

    void RemoveTail(Snake* snake)
    {
        if (snake->head == nullptr)
            return;

        // 找到尾节点的前一个节点，断开链表并释放原尾节点
        if (snake->head == snake->tail)
        {
            delete snake->head;
            snake->head = nullptr;
            snake->tail = nullptr;
        }
        else
        {
            SnakeNode* node = snake->head;
            while (node->next != snake->tail)
                node = node->next;
            delete snake->tail;
            node->next = nullptr;
            snake->tail = node;
        }
        --snake->length;
    }

    void ClearSnake(Snake* snake)
    {
        while (snake->head != nullptr)
        {
            SnakeNode* node = snake->head;
            snake->head = node->next;
            delete node;
        }
        snake->tail = nullptr;
        snake->length = 0;
    }

    Food CreateFood(const Snake* snake, std::mt19937& generator)
    {
        std::uniform_int_distribution<int> xDistribution(0, boardWidth - 1);
        std::uniform_int_distribution<int> yDistribution(0, boardHeight - 1);
        Food food{};
        do
        {
            food.position = {xDistribution(generator), yDistribution(generator)};
            // 食物不能生成在蛇身上，重复随机直到找到空位置
        } while (IsSnakePosition(snake, food.position));
        return food;
    }

    Position NextPosition(Position head, Direction direction)
    {
        switch (direction)
        {
        case Direction::Up:
            --head.y;
            break;
        case Direction::Down:
            ++head.y;
            break;
        case Direction::Left:
            --head.x;
            break;
        case Direction::Right:
            ++head.x;
            break;
        }
        return head;
    }

    bool IsWallCollision(Position position)
    {
        // 坐标超出棋盘范围即表示撞墙
        return position.x < 0 || position.x >= boardWidth || position.y < 0 || position.y >= boardHeight;
    }

    bool IsReverse(Direction current, Direction next)
    {
        return (current == Direction::Up && next == Direction::Down) ||
               (current == Direction::Down && next == Direction::Up) ||
               (current == Direction::Left && next == Direction::Right) ||
               (current == Direction::Right && next == Direction::Left);
    }

#ifdef _WIN32
    bool ReadDirection(Direction* direction)
    {
        if (!_kbhit())
            return false;
        int key = _getch();
        if (key == 0 || key == 224)
        {
            key = _getch();
            if (key == 72) *direction = Direction::Up;
            else if (key == 80) *direction = Direction::Down;
            else if (key == 75) *direction = Direction::Left;
            else if (key == 77) *direction = Direction::Right;
            else return false;
            return true;
        }
        if (key == 'w' || key == 'W') *direction = Direction::Up;
        else if (key == 's' || key == 'S') *direction = Direction::Down;
        else if (key == 'a' || key == 'A') *direction = Direction::Left;
        else if (key == 'd' || key == 'D') *direction = Direction::Right;
        else return false;
        return true;
    }
#else
    bool ReadDirection(Direction*) { return false; }
#endif

    void Draw(const Snake* snake, const Food& food)
    {
        std::cout << "\033[H";
        for (int y = -1; y <= boardHeight; ++y)
        {
            for (int x = -1; x <= boardWidth; ++x)
            {
                Position position{x, y};
                if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight)
                    std::cout << '#';
                else if (SamePosition(food.position, position))
                    std::cout << '*';
                else if (SamePosition(snake->head->position, position))
                    std::cout << 'O';
                else if (IsSnakePosition(snake, position))
                    std::cout << 'o';
                else
                    std::cout << ' ';
            }
            std::cout << '\n';
        }
        std::cout << "长度: " << snake->length << "  W/A/S/D 或方向键移动，Ctrl+C 退出\n" << std::flush;
    }
}

// 程序运行的主函数
void Nokia_Snake()
{
    Snake snake{nullptr, nullptr, 0};
    AddHead(&snake, {boardWidth / 2, boardHeight / 2});
    AddHead(&snake, {boardWidth / 2 - 1, boardHeight / 2});
    AddHead(&snake, {boardWidth / 2 - 2, boardHeight / 2});

    std::mt19937 generator(std::random_device{}());
    Food food = CreateFood(&snake, generator);
    Direction direction = Direction::Right;
    bool gameOver = false;
    bool hitWall = false;

    std::cout << "\033[2J";
    while (!gameOver)
    {
        Direction requestedDirection = direction;
        if (ReadDirection(&requestedDirection) && !IsReverse(direction, requestedDirection))
            direction = requestedDirection;

        Position nextHead = NextPosition(snake.head->position, direction);
        // 移动前检查下一格，撞墙或撞到任意蛇身都会结束游戏
        if (IsWallCollision(nextHead) || IsSnakePosition(&snake, nextHead))
        {
            hitWall = IsWallCollision(nextHead);
            gameOver = true;
            break;
        }

        bool ateFood = SamePosition(nextHead, food.position);
        AddHead(&snake, nextHead);
        if (ateFood)
            // 吃到食物时不删除尾节点，蛇长度增加一节
            food = CreateFood(&snake, generator);
        else
            // 没吃到食物时删除尾节点，蛇只保持移动
            RemoveTail(&snake);

        Draw(&snake, food);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    ClearSnake(&snake);
    std::cout << "游戏结束：蛇撞到了" << (hitWall ? "墙" : "自己") << "。\n";
}