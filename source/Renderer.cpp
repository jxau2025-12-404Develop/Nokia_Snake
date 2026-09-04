#include "Renderer.h"

#include <algorithm>
#include <thread>

#include "Utils.h"

namespace Renderer
{
    namespace
    {
        // 棋盘及游戏对象使用的字符表示。
        constexpr char Wall = '#';
        constexpr char SnakeHead = '@';
        constexpr char SnakeBody = 'o';
        constexpr char Food = '*';

        // 判断坐标是否位于有效的游戏区域内。
        bool IsInside(const Point& point, int width, int height)
        {
            return point.x >= 0 && point.x < width && point.y >= 0 && point.y < height;
        }
    } // namespace

    // 初始化计时器，并将目标帧率转换为每帧间隔。
    Renderer::Renderer(unsigned int framesPerSecond) : frameDuration_(0), lastFrame_(std::chrono::steady_clock::now())
    {
        SetFrameRate(framesPerSecond);
    }

    void Renderer::Render(const GameView& view, ScreenState state)
    {
        // 每次绘制前清屏，避免新旧画面叠加。
        Utils::System::ClearScreen();

        // 统一分发各界面的绘制逻辑。
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
        // 开始菜单只包含操作提示，不需要游戏状态数据。
        RenderLines({"==============================", "          NOKIA SNAKE", "==============================", "",
                     "          [ ENTER ] 开始游戏", "          [ Q     ] 退出游戏", "",
                     "        W A S D / 方向键移动"});
    }

    void Renderer::RenderPlaying(const GameView& view) const
    {
        // 游戏进行中只显示棋盘和当前分数。
        RenderLines(BuildBoard(view));
    }

    void Renderer::RenderPaused(const GameView& view) const
    {
        // 暂停界面保留当前棋盘，并在底部追加操作提示。
        auto lines = BuildBoard(view);
        lines.push_back("");
        lines.push_back("              游戏已暂停");
        lines.push_back("          按空格键继续游戏");
        RenderLines(lines);
    }

    void Renderer::RenderGameOver(const GameView& view) const
    {
        // 先构建结算信息，再根据排行榜是否为空补充内容。
        std::vector<std::string> lines = {"==============================",
                                          "          游戏结束",
                                          "",
                                          "             得分: " + std::to_string(view.score),
                                          "",
                                          "             排行榜"};

        if (view.ranking.empty())
        {
            // 没有历史成绩时显示占位提示。
            lines.push_back("             暂无记录");
        }
        else
        {
            // 排行榜下标从 0 开始，显示时转换为从 1 开始的名次。
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
        // 防止传入 0 导致除零；最小按 1 FPS 处理。
        frameDuration_ = std::chrono::milliseconds(1000 / std::max(1u, framesPerSecond));
    }

    void Renderer::WaitForNextFrame()
    {
        const auto now = std::chrono::steady_clock::now();
        const auto elapsed = now - lastFrame_;

        // 当前帧过早完成时休眠，保证帧率不超过目标值。
        if (elapsed < frameDuration_)
        {
            std::this_thread::sleep_for(frameDuration_ - elapsed);
        }
        // 以等待结束的时刻作为下一帧的计时起点。
        lastFrame_ = std::chrono::steady_clock::now();
    }

    void Renderer::RenderLines(const std::vector<std::string>& lines) const
    {
        // 逐行输出，保持终端画面的布局。
        for (const auto& line : lines)
        {
            Utils::Out::Out(line);
        }
    }

    std::vector<std::string> Renderer::BuildBoard(const GameView& view) const
    {
        // 将非法尺寸钳制为至少 1，确保棋盘始终可绘制。
        const int width = std::max(1, view.width);
        const int height = std::max(1, view.height);

        // 额外增加一圈边框，因此内部区域尺寸为 width * height。
        std::vector<std::string> board(static_cast<std::size_t>(height + 2),
                                       std::string(static_cast<std::size_t>(width + 2), ' '));

        // 先设置左右边界，再用整行填充上下边界。
        for (auto& row : board)
        {
            row.front() = Wall;
            row.back() = Wall;
        }
        std::fill(board.front().begin(), board.front().end(), Wall);
        std::fill(board.back().begin(), board.back().end(), Wall);

        // 只有食物位于棋盘内部时才绘制，避免越界访问。
        if (IsInside(view.food, width, height))
        {
            board[static_cast<std::size_t>(view.food.y + 1)][static_cast<std::size_t>(view.food.x + 1)] = Food;
        }

        // 从蛇尾向蛇头绘制，使蛇头最终覆盖重叠位置。
        for (std::size_t index = view.snake.size(); index > 0; --index)
        {
            const auto& segment = view.snake[index - 1];
            if (IsInside(segment, width, height))
            {
                // 坐标加 1 是因为棋盘外层预留了边框。
                board[static_cast<std::size_t>(segment.y + 1)][static_cast<std::size_t>(segment.x + 1)] =
                    index == 1 ? SnakeHead : SnakeBody;
            }
        }

        // 将分数行放在棋盘前，形成最终的终端输出内容。
        std::vector<std::string> lines = {"NOKIA SNAKE    得分: " + std::to_string(view.score)};
        lines.insert(lines.end(), board.begin(), board.end());
        return lines;
    }
} // namespace Renderer