#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace Renderer
{
    struct Point
    {
        int x = 0;
        int y = 0;

        bool operator==(const Point& other) const
        {
            return x == other.x && y == other.y;
        }
    };

    enum class ScreenState
    {
        StartMenu,
        Playing,
        Paused,
        GameOver
    };

    struct GameView
    {
        int width = 30;
        int height = 15;
        std::vector<Point> snake;
        Point food;
        int score = 0;
        std::vector<int> ranking;
    };

    class Renderer
    {
    public:
        explicit Renderer(unsigned int framesPerSecond = 10);

        void Render(const GameView& view, ScreenState state);
        void RenderStartMenu() const;
        void RenderPlaying(const GameView& view) const;
        void RenderPaused(const GameView& view) const;
        void RenderGameOver(const GameView& view) const;

        void SetFrameRate(unsigned int framesPerSecond);
        void WaitForNextFrame();

    private:
        void RenderLines(const std::vector<std::string>& lines) const;
        std::vector<std::string> BuildBoard(const GameView& view) const;

        std::chrono::milliseconds frameDuration_;
        std::chrono::steady_clock::time_point lastFrame_;
    };
} // namespace Renderer