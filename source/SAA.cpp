#include "ScoreAndAccount.h" // 引入成绩账号类的声明。
#include "Utils.h"           // 引入成员一提供的文件写入工具。

#include <algorithm> // 提供排序和分数限制函数。
#include <fstream>   // 提供本地存档读取功能。
#include <iostream>  // 提供控制台输入输出功能。
#include <sstream>   // 提供成绩行解析功能。

namespace
{
    const std::string scoreFile = "scores.log"; // 保存历史成绩排行榜的文件名。
    const std::string accountFile = "user.txt"; // 保存玩家账号（昵称 分数 密码）的文件名。
    constexpr int foodScore = 10;               // 吃到食物增加的分数。
    constexpr int collisionPenalty = 10;        // 碰撞时扣除的分数。
    constexpr int scorePerLevel = 50;           // 每增加 50 分提升一个难度等级。
    constexpr int firstLevelDelay = 220;        // 初始移动间隔，单位为毫秒。
    constexpr int delayDecreasePerLevel = 20;   // 每提升一级减少的移动间隔。
    constexpr int fastestDelay = 60;            // 允许的最短移动间隔。
} // namespace

namespace Account
{
    ScoreAccount::ScoreAccount(const std::string& username, const std::string& accountToken,
                               const char LS) // 构造玩家登录账号。
        : nickname_(username.empty() ? "Player" : username), score_(0),
          token(accountToken) // 保存昵称、初始分数和登录凭证。
    {
        if (LS == 's') // 只有状态字符为 s 时才认为其为注册。
        {
            // 新账号分数固定为 0，格式为：昵称 分数 密码
            const std::string record = nickname_ + " " + std::to_string(0) + " " + token;
            // 追加写入账号文件，保存成功即视为注册成功。
            checktoken = Utils::File::Add(record, accountFile);
        }
        else if (LS == 'l')
        {
            checktoken = CheckToken(); // 登录时从文件逐行比对昵称和密码。
        }
    }

    bool ScoreAccount::CheckToken() // 读取 score.txt 逐行比对昵称和密码，判断登录是否成功。
    {
        if (nickname_.empty() || token.empty()) // 先检查输入是否为空。
        {
            std::cout << "登录失败：用户名或凭证不能为空。\n"; // 输出失败原因。
            return false;                                      // 输入为空直接登录失败。
        }

        std::ifstream input(accountFile); // 打开保存账号信息的 score.txt。
        if (!input)                       // 文件不存在或打开失败时无法比对。
        {
            std::cout << "登录失败：账号文件不存在。\n"; // 提示账号文件缺失。
            return false;                                // 没有账号文件即登录失败。
        }

        std::string line;                 // 用来保存逐行读取的文本。
        while (std::getline(input, line)) // 逐行读取每个账号记录。
        {
            if (line.empty()) // 跳过空行，避免无效记录干扰比对。
            {
                continue; // 继续读取下一行。
            }
            std::istringstream record(line);                         // 将一行文本转为输入流，便于按空格拆字段。
            std::string fileNickname;                                // 保存文件中的昵称。
            int fileScore = 0;                                       // 保存文件中的分数。
            std::string fileToken;                                   // 保存文件中的密码。
            if (!(record >> fileNickname >> fileScore >> fileToken)) // 按“昵称 分数 密码”格式解析，缺字段视为无效行。
            {
                continue; // 格式错误时跳过该行。
            }
            if (fileNickname == nickname_ && fileToken == token) // 昵称和密码都匹配才算登录成功。
            {
                score_ = fileScore; // 登录成功后把文件中保存的分数恢复到当前分数。
                std::cout << "登录成功！欢迎回来，" << nickname_ << "。\n"; // 输出登录成功的提示信息。
                return true;                                                // 返回登录成功结果。
            }
        }
        std::cout << "登录失败：用户名或密码不正确。\n"; // 遍历完所有记录后仍无匹配时输出失败提示。
        return false;                                    // 返回登录失败结果。
    }

    void ScoreAccount::InputNickname() // 从控制台读取玩家昵称。
    {
        std::string nickname; // 创建变量保存控制台输入内容。
        do
        {
            std::cout << "请输入玩家昵称：";       // 提示玩家输入昵称。
            if (!std::getline(std::cin, nickname)) // 读取一整行昵称并检查输入状态。
            {
                nickname = "Player"; // 输入流结束时使用默认昵称，避免循环卡住。
                break;               // 结束输入循环。
            }
        } while (nickname.empty()); // 输入为空时继续读取，直到昵称有效。

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
            if (line.empty()) // 跳过空行，避免无效记录影响排行榜。
            {
                continue; // 继续读取下一条成绩。
            }
            std::istringstream record(line); // 将文本行转换为输入流。
            ScoreRecord item{};              // 创建一条临时成绩记录。
            if (std::getline(record, item.nickname, '\t') && record >> item.score && item.score >= 0 &&
                !item.nickname.empty()) // 校验昵称和非负分数格式。
            {
                scores.push_back(item); // 保存格式正确的成绩。
            }
        }
        std::sort(scores.begin(), scores.end(), [](const ScoreRecord& left, const ScoreRecord& right) { // 按分数从高到低排序。
            if (left.score != right.score) // 分数不同时优先比较分数。
            {
                return left.score > right.score; // 让高分排在前面。
            }
            return left.nickname < right.nickname; // 分数相同时按昵称排序，保证排行稳定。
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
