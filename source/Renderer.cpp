#include "Renderer.h"

#include <algorithm>
#include <thread>

#include "Utils.h"

namespace Renderer
{
    namespace
    {
        constexpr char Wall = '#';
        constexpr char SnakeHead = '@';
        constexpr char SnakeBody = 'o';
        constexpr char Food = '*';

        bool IsInside(const Point& point, int width, int height)
        {
            return point.x >= 0 && point.x < width && point.y >= 0 && point.y < height;
        }
    } // namespace

    Renderer::Renderer(unsigned int framesPerSecond) : frameDuration_(0), lastFrame_(std::chrono::steady_clock::now())
    {
        SetFrameRate(framesPerSecond);
    }

    void Renderer::Render(const GameView& view, ScreenState state)
    {
        Utils::System::ClearScreen();

        switch (state)
        {
        case ScreenState::StartMenu:
            RenderStartMenu();
            break;
        case ScreenState::Playing:
            RenderPlaying(view);
            break;
        case ScreenState::Paused:
            RenderPaused(view);
            break;
        case ScreenState::GameOver:
            RenderGameOver(view);
            break;
        }
    }

    void Renderer::RenderStartMenu() const
    {
        RenderLines({"==============================", "          NOKIA SNAKE", "==============================", "",
                     "          [ ENTER ] 开始游戏", "          [ Q     ] 退出游戏", "",
                     "        W A S D / 方向键移动"});
    }

    void Renderer::RenderPlaying(const GameView& view) const
    {
        RenderLines(BuildBoard(view));
    }

    void Renderer::RenderPaused(const GameView& view) const
    {
        auto lines = BuildBoard(view);
        lines.push_back("");
        lines.push_back("              游戏已暂停");
        lines.push_back("          按空格键继续游戏");
        RenderLines(lines);
    }

    void Renderer::RenderGameOver(const GameView& view) const
    {
        std::vector<std::string> lines = {"==============================",
                                          "          游戏结束",
                                          "",
                                          "             得分: " + std::to_string(view.score),
                                          "",
                                          "             排行榜"};

        if (view.ranking.empty())
        {
            lines.push_back("             暂无记录");
        }
        else
        {
            for (std::size_t index = 0; index < view.ranking.size(); ++index)
            {
                lines.push_back("             " + std::to_string(index + 1) + ". " +
                                std::to_string(view.ranking[index]));
            }
        }

        lines.push_back("");
        lines.push_back("          按 Enter 重新开始");
        lines.push_back("          按 Q     退出游戏");
        lines.push_back("==============================");
        RenderLines(lines);
    }

    void Renderer::SetFrameRate(unsigned int framesPerSecond)
    {
        frameDuration_ = std::chrono::milliseconds(1000 / std::max(1u, framesPerSecond));
    }

    void Renderer::WaitForNextFrame()
    {
        const auto now = std::chrono::steady_clock::now();
        const auto elapsed = now - lastFrame_;
        if (elapsed < frameDuration_)
        {
            std::this_thread::sleep_for(frameDuration_ - elapsed);
        }
        lastFrame_ = std::chrono::steady_clock::now();
    }

    void Renderer::RenderLines(const std::vector<std::string>& lines) const
    {
        for (const auto& line : lines)
        {
            Utils::Out::Out(line);
        }
    }

    std::vector<std::string> Renderer::BuildBoard(const GameView& view) const
    {
        const int width = std::max(1, view.width);
        const int height = std::max(1, view.height);
        std::vector<std::string> board(static_cast<std::size_t>(height + 2),
                                       std::string(static_cast<std::size_t>(width + 2), ' '));

        for (auto& row : board)
        {
            row.front() = Wall;
            row.back() = Wall;
        }
        std::fill(board.front().begin(), board.front().end(), Wall);
        std::fill(board.back().begin(), board.back().end(), Wall);

        if (IsInside(view.food, width, height))
        {
            board[static_cast<std::size_t>(view.food.y + 1)][static_cast<std::size_t>(view.food.x + 1)] = Food;
        }

        for (std::size_t index = view.snake.size(); index > 0; --index)
        {
            const auto& segment = view.snake[index - 1];
            if (IsInside(segment, width, height))
            {
                board[static_cast<std::size_t>(segment.y + 1)][static_cast<std::size_t>(segment.x + 1)] =
                    index == 1 ? SnakeHead : SnakeBody;
            }
        }

        std::vector<std::string> lines = {"NOKIA SNAKE    得分: " + std::to_string(view.score)};
        lines.insert(lines.end(), board.begin(), board.end());
        return lines;
    }
} // namespace Renderer