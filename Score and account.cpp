#include "ScoreAndAccount.h"

#include "Utils.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace
{
	const char* ScoreFile = "scores.dat";
}

namespace Account
{
	std::vector<ScoreRecord> LoadScores()
	{
		std::vector<ScoreRecord> scores;
		std::ifstream input(ScoreFile);
		std::string line;

		while (std::getline(input, line))
		{
			std::istringstream row(line);
			ScoreRecord record;
			if (row >> record.score && std::getline(row, record.nickname))
			{
				if (!record.nickname.empty() && record.nickname.front() == ' ')
					record.nickname.erase(record.nickname.begin());
				scores.push_back(record);
			}
		}

		return scores;
	}

	bool SaveScore(const std::string& nickname, int score)
	{
		return Utils::File::File_add(score, ScoreFile) &&
			   Utils::File::File_add(std::string("  ") + nickname, "scores.dat");
	}

	int PersonalBest(const std::string& nickname, const std::vector<ScoreRecord>& scores)
	{
		int best = 0;
		for (const ScoreRecord& record : scores)
		{
			if (record.nickname == nickname)
				best = std::max(best, record.score);
		}
		return best;
	}

	void ShowRanking(const std::vector<ScoreRecord>& scores)
	{
		std::vector<ScoreRecord> ranking = scores;
		std::sort(ranking.begin(), ranking.end(), [](const ScoreRecord& left, const ScoreRecord& right) {
			return left.score > right.score;
		});

		Utils::Out::Out("\n本地历史成绩排行（前十名）");
		if (ranking.empty())
		{
			Utils::Out::Out("暂无历史成绩");
			return;
		}

		const std::size_t count = std::min<std::size_t>(10, ranking.size());
		for (std::size_t index = 0; index < count; ++index)
			Utils::Out::Out(std::to_string(index + 1) + ". " + ranking[index].nickname + " - " +
							std::to_string(ranking[index].score));
	}
}
