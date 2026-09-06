#include "ScoreAndAccount.h" // 引入成绩账号类的声明。
#include "Utils.h"           // 引入成员一提供的文件写入工具。

#include <algorithm> // 提供排序和分数限制函数。
#include <fstream>   // 提供本地存档读取功能。
#include <iostream>  // 提供控制台输入输出功能。
#include <sstream>   // 提供成绩行解析功能。

namespace
{
    const std::string scoreFile = "scores.log"; // 保存历史成绩的文件名。
    constexpr int foodScore = 10;               // 吃到食物增加的分数。
    constexpr int collisionPenalty = 10;        // 碰撞时扣除的分数。
    constexpr int scorePerLevel = 50;           // 每增加 50 分提升一个难度等级。
    constexpr int firstLevelDelay = 220;        // 初始移动间隔，单位为毫秒。
    constexpr int delayDecreasePerLevel = 20;   // 每提升一级减少的移动间隔。
    constexpr int fastestDelay = 60;            // 允许的最短移动间隔。
} // namespace

namespace Account
{
    ScoreAccount::ScoreAccount(const std::string& nickname)            // 构造玩家账号对象。
        : nickname_(nickname.empty() ? "Player" : nickname), score_(0) // 设置昵称并将初始分数设为 0。
    {
    }

    void ScoreAccount::InputNickname() // 从控制台读取玩家昵称。
    {
        std::string nickname; // 创建变量保存控制台输入内容。
        do
        {
            std::cout << "请输入玩家昵称：";  // 提示玩家输入昵称。
            std::getline(std::cin, nickname); // 读取一整行昵称。
        } while (nickname.empty() && !std::cin.eof()); // 输入为空时继续读取，直到昵称有效。

        SetNickname(nickname); // 使用成员方法保存并展示昵称。
    }

    void ScoreAccount::SetNickname(const std::string& nickname) // 设置玩家昵称。
    {
        nickname_ = nickname.empty() ? "Player" : nickname; // 空昵称使用默认名称 Player。
        std::cout << "当前玩家：" << nickname_ << "\n";     // 展示当前玩家昵称。
    }

    const std::string& ScoreAccount::GetNickname() const // 获取玩家昵称。
    {
        return nickname_; // 返回账号保存的昵称。
    }

    int ScoreAccount::GetScore() const // 获取当前分数。
    {
        return score_; // 返回账号保存的分数。
    }

    int ScoreAccount::AddFoodScore() // 处理吃到食物后的加分。
    {
        score_ += foodScore; // 吃到食物增加 10 分。
        return score_;       // 返回加分后的分数。
    }

    int ScoreAccount::ApplyCollisionPenalty() // 处理撞墙或撞身后的扣分。
    {
        score_ = std::max(0, score_ - collisionPenalty); // 扣除 10 分且保证分数不低于 0。
        return score_;                                   // 返回扣分后的分数。
    }

    int ScoreAccount::GetDifficultyLevel() const // 根据当前分数计算难度等级。
    {
        return score_ / scorePerLevel + 1; // 每 50 分提升一个难度等级。
    }

    int ScoreAccount::GetMoveDelay() const // 根据当前分数计算移动间隔。
    {
        const int delay = firstLevelDelay - (GetDifficultyLevel() - 1) * delayDecreasePerLevel; // 难度越高等待越短。
        return std::max(fastestDelay, delay); // 限制最快速度，避免游戏失控。
    }

    void ScoreAccount::ShowScoreState() const // 展示游戏中的账号状态。
    {
        std::cout << "玩家：" << nickname_                           // 输出玩家昵称。
                  << " | 分数：" << score_                           // 输出当前分数。
                  << " | 难度：" << GetDifficultyLevel()             // 输出当前难度。
                  << " | 移动间隔：" << GetMoveDelay() << " 毫秒\n"; // 输出当前速度。
    }

    std::vector<Account::ScoreRecord> ScoreAccount::LoadScores() const // 读取并排序历史成绩。
    {
        std::vector<ScoreRecord> scores;  // 创建成绩列表保存读取结果。
        std::ifstream input(scoreFile);   // 打开本地成绩文件。
        std::string line;                 // 保存当前读取的文本行。
        while (std::getline(input, line)) // 循环读取每一行成绩。
        {
            std::istringstream record(line); // 将文本行转换为输入流。
            ScoreRecord item{};              // 创建一条临时成绩记录。
            if (std::getline(record, item.nickname, '\t') && record >> item.score &&
                !item.nickname.empty()) // 校验昵称和分数格式。
            {
                scores.push_back(item); // 保存格式正确的成绩。
            }
        }
        std::sort(scores.begin(), scores.end(), [](const ScoreRecord& left, const ScoreRecord& right) { // 按分数从高到低排序。
            return left.score > right.score; // 让高分排在前面。
        });
        return scores; // 返回排序后的历史成绩。
    }

    bool ScoreAccount::SaveScore(int score) const // 将成绩追加保存到本地文件。
    {
        if (nickname_.empty() || score < 0) // 拒绝空昵称和负数成绩。
        {
            return false; // 返回保存失败。
        }
        const std::string record = nickname_ + "\t" + std::to_string(score); // 按“昵称+制表符+分数”拼接存档内容。
        return Utils::File::Add(record, scoreFile);                          // 调用 FileHelper 的 Add 方法写入成绩。
    }

    bool ScoreAccount::SaveGameResult() // 游戏结束时保存成绩并展示排行。
    {
        if (!SaveScore(score_)) // 保存当前玩家本局成绩。
        {
            std::cout << "成绩保存失败。\n"; // 保存失败时显示提示。
            return false;                    // 返回失败结果。
        }
        const std::vector<ScoreRecord> scores = LoadScores();           // 重新读取包含本局成绩的排行榜。
        std::cout << "===== 游戏结束 =====\n";                          // 输出游戏结束标题。
        std::cout << "玩家：" << nickname_ << "\n";                     // 展示结束时的玩家昵称。
        std::cout << "本局成绩：" << score_ << " 分\n";                 // 展示本局最终成绩。
        std::cout << "个人最高分：" << PersonalBest(scores) << " 分\n"; // 展示玩家历史最高分。
        Account::ShowRanking(scores);                                   // 展示历史成绩排行。
        return true;                                                    // 返回保存和展示成功。
    }

    int ScoreAccount::PersonalBest(const std::vector<ScoreRecord>& scores) const // 查询当前玩家历史最高分。
    {
        int best = 0;                          // 没有记录时最高分默认为 0。
        for (const ScoreRecord& item : scores) // 遍历所有历史成绩。
        {
            if (item.nickname == nickname_ && item.score > best) // 只比较当前玩家的更高成绩。
            {
                best = item.score; // 更新个人最高分。
            }
        }
        return best; // 返回个人最高分。
    }

    void ShowRanking(const std::vector<ScoreRecord>& scores) // 展示历史成绩排行榜。
    {
        std::cout << "\n===== 历史成绩排行 =====\n"; // 输出排行榜标题。
        if (scores.empty())                          // 判断是否存在历史成绩。
        {
            std::cout << "暂无历史成绩\n"; // 没有成绩时显示提示。
            return;                        // 结束排行榜展示。
        }
        const std::size_t count = std::min<std::size_t>(scores.size(), 10); // 最多展示前十名。
        for (std::size_t index = 0; index < count; ++index)                 // 遍历需要展示的成绩。
        {
            std::cout << index + 1 << ". " << scores[index].nickname << " : " << scores[index].score
                      << " 分\n"; // 输出名次、昵称和分数。
        }
    }
} // namespace Account
