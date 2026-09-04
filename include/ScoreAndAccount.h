#pragma once

#include <string>
#include <vector>

namespace Account
{
    struct ScoreRecord
    {
        std::string nickname;
        int score;
    };

    std::vector<ScoreRecord> LoadScores();
    bool SaveScore(const std::string& nickname, int score);
    int PersonalBest(const std::string& nickname, const std::vector<ScoreRecord>& scores);
    void ShowRanking(const std::vector<ScoreRecord>& scores);
}
