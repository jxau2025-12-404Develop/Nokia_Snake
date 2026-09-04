#pragma once

#include <deque>

namespace SnakeGame
{
    // 游戏棋盘上的一个网格坐标。x 表示水平方向，y 表示垂直方向。
    struct Position
    {
        int x = 0;
        int y = 0;

        // 判断两个坐标是否指向同一个网格。
        bool operator==(const Position& other) const;

        // 判断两个坐标是否指向不同网格。
        bool operator!=(const Position& other) const;
    };

    // 蛇头下一次移动的方向。
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    class Snake
    {
    public:
        // 创建一条只有一个蛇头的蛇。
        // start 是蛇头初始位置，direction 是初始移动方向。
        explicit Snake(Position start = {10, 10}, Direction direction = Direction::Right);

        // 清空当前身体并恢复为初始状态，可用于开始新游戏。
        void Reset(Position start = {10, 10}, Direction direction = Direction::Right);

        // 处理单字节输入：支持 W/A/S/D 和空格键。
        // 返回 true 表示检测到空格，调用者应切换暂停状态。
        bool HandleInput(char key);

        // 处理方向键的第二个按键码：上 72、下 80、左 75、右 77。
        // 同样支持用空格键请求暂停或继续。
        bool HandleInput(int key);

        // 设置待执行方向。相反方向会被忽略，避免蛇头立即撞上自己的身体。
        void SetDirection(Direction direction);

        // 按当前方向前进一格，不进行食物检测。
        // 返回 false 表示蛇已经死亡或撞到自身。
        bool Move();

        // 按当前方向前进一格，并判断新蛇头是否到达 food。
        // 吃到食物时身体增长一节；返回 false 表示移动失败或撞到自身。
        bool Move(const Position& food);

        // 判断指定坐标是否被蛇身（包括蛇头）占据。
        bool Occupies(const Position& position) const;

        // 标记蛇在下一次移动时增长一节。
        void Grow();

        // 返回蛇身，从蛇头到蛇尾排列。返回的是只读引用，不应修改容器内容。
        const std::deque<Position>& Body() const;

        // 返回当前蛇头坐标。
        Position Head() const;

        // 返回当前已经生效的移动方向。
        Direction GetDirection() const;

        // 返回蛇当前是否仍可继续移动。
        bool IsAlive() const;

    private:
        // 判断两个方向是否互为相反方向。
        static bool IsOpposite(Direction first, Direction second);

        // 执行实际移动、增长和碰撞后的状态更新。
        bool MoveTo(const Position& nextHead, bool foodEaten);

        // 蛇身队列，队首为蛇头，队尾为蛇尾。
        std::deque<Position> body_;

        // 当前正在移动的方向。
        Direction direction_;

        // 已接收但尚未移动的方向，用于处理下一帧输入。
        Direction next_direction_;

        // 是否应在下一次移动时保留蛇尾，从而使蛇增长一节。
        bool grow_pending_ = false;

        // 蛇是否仍处于存活状态。
        bool alive_ = true;
    };
} // namespace SnakeGame