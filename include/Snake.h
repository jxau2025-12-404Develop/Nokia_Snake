#pragma once

#include <deque>

namespace SnakeGame
{
    struct Position
    {
        int x = 0;
        int y = 0;

        bool operator==(const Position& other) const;
        bool operator!=(const Position& other) const;
    };

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
        explicit Snake(Position start = {10, 10}, Direction direction = Direction::Right);

        void Reset(Position start = {10, 10}, Direction direction = Direction::Right);
        // Returns true when the key requests pause/resume.
        bool HandleInput(char key);
        bool HandleInput(int key);
        void SetDirection(Direction direction);
        bool Move();
        bool Move(const Position& food);
        bool Occupies(const Position& position) const;
        void Grow();

        const std::deque<Position>& Body() const;
        Position Head() const;
        Direction GetDirection() const;
        bool IsAlive() const;

    private:
        static bool IsOpposite(Direction first, Direction second);
        bool MoveTo(const Position& nextHead, bool foodEaten);

        std::deque<Position> body_;
        Direction direction_;
        Direction next_direction_;
        bool grow_pending_ = false;
        bool alive_ = true;
    };
} // namespace SnakeGame