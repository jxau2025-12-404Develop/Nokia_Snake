#include "Snake.h"

#include <cctype>

namespace SnakeGame
{
    bool Position::operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }

    bool Position::operator!=(const Position& other) const
    {
        return !(*this == other);
    }

    Snake::Snake(Position start, Direction direction)
    {
        Reset(start, direction);
    }

    void Snake::Reset(Position start, Direction direction)
    {
        body_.clear();
        body_.push_back(start);
        direction_ = direction;
        next_direction_ = direction;
        grow_pending_ = false;
        alive_ = true;
    }

    bool Snake::HandleInput(char key)
    {
        switch (std::tolower(static_cast<unsigned char>(key)))
        {
        case 'w':
            SetDirection(Direction::Up);
            return false;
        case 's':
            SetDirection(Direction::Down);
            return false;
        case 'a':
            SetDirection(Direction::Left);
            return false;
        case 'd':
            SetDirection(Direction::Right);
            return false;
        case ' ':
            return true;
        default:
            return false;
        }
    }

    bool Snake::HandleInput(int key)
    {
        switch (key)
        {
        case 72:
            SetDirection(Direction::Up);
            return false;
        case 80:
            SetDirection(Direction::Down);
            return false;
        case 75:
            SetDirection(Direction::Left);
            return false;
        case 77:
            SetDirection(Direction::Right);
            return false;
        case ' ':
            return true;
        default:
            return false;
        }
    }

    void Snake::SetDirection(Direction direction)
    {
        if (!IsOpposite(next_direction_, direction))
            next_direction_ = direction;
    }

    bool Snake::Move()
    {
        Position nextHead = Head();
        direction_ = next_direction_;

        switch (direction_)
        {
        case Direction::Up:
            --nextHead.y;
            break;
        case Direction::Down:
            ++nextHead.y;
            break;
        case Direction::Left:
            --nextHead.x;
            break;
        case Direction::Right:
            ++nextHead.x;
            break;
        }

        return MoveTo(nextHead, false);
    }

    bool Snake::Move(const Position& food)
    {
        Position nextHead = Head();
        direction_ = next_direction_;

        switch (direction_)
        {
        case Direction::Up:
            --nextHead.y;
            break;
        case Direction::Down:
            ++nextHead.y;
            break;
        case Direction::Left:
            --nextHead.x;
            break;
        case Direction::Right:
            ++nextHead.x;
            break;
        }

        return MoveTo(nextHead, nextHead == food);
    }

    bool Snake::MoveTo(const Position& nextHead, bool foodEaten)
    {
        if (!alive_ || Occupies(nextHead))
        {
            alive_ = false;
            return false;
        }

        body_.push_front(nextHead);
        if (foodEaten || grow_pending_)
            grow_pending_ = false;
        else
            body_.pop_back();

        return true;
    }

    bool Snake::Occupies(const Position& position) const
    {
        for (const Position& segment : body_)
        {
            if (segment == position)
                return true;
        }
        return false;
    }

    void Snake::Grow()
    {
        grow_pending_ = true;
    }

    const std::deque<Position>& Snake::Body() const
    {
        return body_;
    }

    Position Snake::Head() const
    {
        return body_.front();
    }

    Direction Snake::GetDirection() const
    {
        return direction_;
    }

    bool Snake::IsAlive() const
    {
        return alive_;
    }

    bool Snake::IsOpposite(Direction first, Direction second)
    {
        return (first == Direction::Up && second == Direction::Down) ||
               (first == Direction::Down && second == Direction::Up) ||
               (first == Direction::Left && second == Direction::Right) ||
               (first == Direction::Right && second == Direction::Left);
    }
} // namespace SnakeGame