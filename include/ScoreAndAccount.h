#pragma once

#include <string>
#include <vector>

namespace Account
{
    // 保存一条玩家成绩记录。
    struct ScoreRecord
    {
        std::string nickname; // 玩家昵称。
        int score;            // 本局分数。
    };

    // 封装玩家账号、计分、难度、存档和排行榜功能。
    class ScoreAccount
    {
    public:
        // 创建账号对象，只初始化玩家昵称和密码，不自动写入文件。
        explicit ScoreAccount(const std::string& nickname = "Player", const std::string& token = "", const char LS = 's');
        // 销毁对象时，根据保存开关决定是否写入账号信息。
        ~ScoreAccount();
        // 将当前对象的昵称和密码写入账号文件。
        bool WriteAccount() const;
        // 设置对象销毁时是否自动写入昵称和密码。
        void SetSaveOnDestroy(bool shouldSave);
        // 是否成功
        bool CheckToken();

        // 从控制台输入玩家昵称。
        void InputNickname();
        // 设置玩家昵称。
        void SetNickname(const std::string& nickname);
        // 返回玩家昵称。
        const std::string& GetNickname() const;
        // 返回当前分数。
        int GetScore() const;
        // 吃到食物增加 10 分。
        int AddFoodScore();
        // 碰撞扣除 10 分，最低为 0 分。
        int ApplyCollisionPenalty();
        // 根据分数返回难度等级。
        int GetDifficultyLevel() const;
        // 根据分数返回移动间隔。
        int GetMoveDelay() const;
        // 展示昵称、分数、难度和速度。
        void ShowScoreState() const;
        // 读取并排序历史成绩。
        std::vector<ScoreRecord> LoadScores() const;
        // 保存当前玩家的指定成绩。
        bool SaveScore(int score) const;
        // 游戏结束时保存当前成绩并展示排行榜。
        bool SaveGameResult();
        // 设置一个全区score
        void SetScore(int score)
        {
            score_ = score;
        }
        // 查询当前玩家的历史最高分。
        int PersonalBest(const std::vector<ScoreRecord>& scores) const;

    private:
        std::string nickname_; // 保存玩家昵称。
        int score_;            // 保存当前分数。
        std::string token;     // 保存玩家密码。
        bool saveOnDestroy_;   // 控制对象销毁时是否写入账号信息。
        bool checkToken_;      // 保存注册或登录是否成功的状态。
    };

    // 展示历史成绩前十名，不属于 ScoreAccount 类。
    void ShowRanking(const std::vector<ScoreRecord>& scores);
} // namespace Account