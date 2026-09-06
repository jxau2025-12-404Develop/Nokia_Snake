#include "ScoreAndAccount.h" // 引入成绩记录和账号功能的声明。
#include "Utils.h"           // 引入成员一提供的文件写入工具。
#include <algorithm>         // 提供成绩排序函数。
#include <fstream>           // 提供本地文件读写功能。
#include <iostream>          // 提供控制台输出功能。
#include <limits>            // 提供输入流清理功能。
#include <sstream>           // 提供单行成绩记录的解析功能。

namespace // 使用匿名命名空间，避免存档文件名暴露到其他源文件。
{
    const std::string scoreFile = "scores.log"; // 保存历史成绩的本地文件名。
    constexpr int foodScore = 10;               // 吃到一个食物增加的分数。
    constexpr int collisionPenalty = 10;        // 撞墙或撞身扣除的分数。
    constexpr int scorePerLevel = 50;           // 每增加 50 分提升一个难度等级。
    constexpr int firstLevelDelay = 220;        // 第一等级的移动间隔，单位为毫秒。
    constexpr int delayDecreasePerLevel = 20;   // 每提升一级减少的移动间隔。
    constexpr int fastestDelay = 60;            // 游戏允许的最短移动间隔。
} // namespace

namespace Account // 账号和成绩功能所在的命名空间。
{
    std::string InputNickname() // 读取并展示玩家昵称。
    {
        std::string nickname;    // 创建变量保存玩家昵称。
        while (nickname.empty()) // 昵称为空时持续要求重新输入。
        {
            std::cout << "请输入玩家昵称：";  // 提示玩家输入昵称。
            std::getline(std::cin, nickname); // 读取一整行昵称，允许昵称包含空格。
            if (std::cin.eof())               // 判断输入是否已经到达文件尾。
            {
                return "Player"; // 没有输入时返回默认昵称。
            }
            if (nickname.empty()) // 判断玩家是否输入了空昵称。
            {
                std::cout << "昵称不能为空，请重新输入。\n"; // 提醒玩家输入有效昵称。
            }
        }
        std::cout << "当前玩家：" << nickname << "\n"; // 展示已经输入的玩家昵称。
        return nickname;                               // 返回昵称供游戏流程使用。
    }

    int AddFoodScore(int currentScore) // 计算吃到食物后的新分数。
    {
        return currentScore + foodScore; // 每吃到一个食物增加 10 分。
    }

    int ApplyCollisionPenalty(int currentScore) // 计算碰撞后的新分数。
    {
        return std::max(0, currentScore - collisionPenalty); // 扣除 10 分且保证分数不会小于零。
    }

    int GetDifficultyLevel(int score) // 根据当前分数计算难度等级。
    {
        return score / scorePerLevel + 1; // 每增加 50 分，难度等级增加 1。
    }

    int GetMoveDelay(int score) // 根据当前分数计算蛇的移动间隔。
    {
        const int level = GetDifficultyLevel(score);                             // 获取当前难度等级。
        const int delay = firstLevelDelay - (level - 1) * delayDecreasePerLevel; // 难度越高，等待时间越短。
        return std::max(fastestDelay, delay);                                    // 限制最快速度，避免移动过快。
    }

    void ShowScoreState(const std::string& nickname, int score) // 展示游戏中的玩家信息和实时难度。
    {
        std::cout << "玩家：" << nickname                                 // 输出当前玩家昵称。
                  << " | 分数：" << score                                 // 输出当前游戏分数。
                  << " | 难度：" << GetDifficultyLevel(score)             // 输出当前难度等级。
                  << " | 移动间隔：" << GetMoveDelay(score) << " 毫秒\n"; // 输出当前速度信息。
    }

    bool SaveGameResult(const std::string& nickname, int score) // 游戏结束时保存成绩并展示排行榜。
    {
        const bool saved = SaveScore(nickname, score); // 使用 FileHelper 将本局成绩写入本地存档。
        if (!saved)                                    // 判断成绩是否写入成功。
        {
            std::cout << "成绩保存失败。\n"; // 写入失败时向玩家显示提示。
            return false;                    // 返回失败结果。
        }
        const std::vector<ScoreRecord> scores = LoadScores(); // 重新读取存档，得到包含本局的排行榜。
        std::cout << "成绩已保存，以下是历史成绩排行：\n";    // 提示玩家进入游戏结束界面。
        ShowRanking(scores);                                  // 展示历史成绩前十名。
        return true;                                          // 返回保存和展示均成功的结果。
    }

    std::vector<ScoreRecord> LoadScores() // 从本地文件读取全部历史成绩。
    {
        std::vector<ScoreRecord> scores;  // 创建成绩列表，用于保存读取结果。
        std::ifstream input(scoreFile);   // 以只读方式打开成绩存档。
        std::string line;                 // 保存当前读取到的一整行文本。
        while (std::getline(input, line)) // 逐行读取成绩存档直到文件结束。
        {
            std::istringstream record(line); // 将当前行包装成输入流，方便拆分字段。
            ScoreRecord score;               // 创建一条临时成绩记录。
            if (std::getline(record, score.nickname, '\t') && record >> score.score &&
                !score.nickname.empty()) // 按“昵称+制表符+分数”格式校验数据。
            {
                scores.push_back(score); // 只保存格式正确且昵称非空的成绩。
            }
        }
        std::sort(scores.begin(), scores.end(), [](const ScoreRecord& left, const ScoreRecord& right) { // 按分数从高到低排序。
			return left.score > right.score; // 分数较高的记录排在前面。
		}); // 结束排序操作。
        return scores; // 返回已经排序的历史成绩。
    }

    bool SaveScore(const std::string& nickname, int score) // 将本局成绩追加到本地存档。
    {
        if (nickname.empty() || score < 0) // 拒绝空昵称和负数成绩。ss
        {
            return false; // 告知调用者本次保存参数无效。
        }
        return Utils::File::Add(nickname + "\t" + std::to_string(score),
                                scoreFile); // 调用 FileHelper 将昵称和分数追加写入文件。
    }

    int PersonalBest(const std::string& nickname, const std::vector<ScoreRecord>& scores) // 查询指定玩家的历史最高分。
    {
        int best = 0;                            // 默认最高分为零，适用于没有历史记录的玩家。
        for (const ScoreRecord& record : scores) // 遍历所有历史成绩记录。
        {
            if (record.nickname == nickname && record.score > best) // 只比较同名玩家中更高的成绩。
            {
                best = record.score; // 更新当前找到的个人最高分。
            }
        }
        return best; // 返回查询到的最高分。
    }

    void ShowRanking(const std::vector<ScoreRecord>& scores) // 在结束界面展示历史成绩排行。
    {
        std::cout << "\n===== 历史成绩排行 =====\n"; // 输出排行标题。
        if (scores.empty())                          // 判断本地是否还没有任何历史成绩。
        {
            std::cout << "暂无历史成绩\n"; // 提示玩家当前没有可展示的记录。
            return;                        // 没有记录时直接结束展示函数。
        }
        const std::size_t count = std::min<std::size_t>(scores.size(), 10); // 最多展示前十名，避免结束界面过长。
        for (std::size_t index = 0; index < count; ++index)                 // 依次输出排行中的每一条记录。
        {
            std::cout << index + 1 << ". " << scores[index].nickname << " : " << scores[index].score
                      << " 分\n"; // 输出名次、昵称和分数。
        }
    }
} // namespace Account
