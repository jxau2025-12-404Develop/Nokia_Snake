#include "ScoreAndAccount.h" // 引入成绩记录和账号功能的声明。
#include "Utils.h" // 引入成员一提供的文件写入工具。
#include <algorithm> // 提供成绩排序函数。
#include <fstream> // 提供本地文件读写功能。
#include <iostream> // 提供控制台输出功能。
#include <sstream> // 提供单行成绩记录的解析功能。

namespace // 使用匿名命名空间，避免存档文件名暴露到其他源文件。
{
	const std::string scoreFile = "scores.log"; // 保存历史成绩的本地文件名。
}

namespace Account // 账号和成绩功能所在的命名空间。
{
	std::vector<ScoreRecord> LoadScores() // 从本地文件读取全部历史成绩。
	{
		std::vector<ScoreRecord> scores; // 创建成绩列表，用于保存读取结果。
		std::ifstream input(scoreFile); // 以只读方式打开成绩存档。
		std::string line; // 保存当前读取到的一整行文本。
		while (std::getline(input, line)) // 逐行读取成绩存档直到文件结束。
		{
			std::istringstream record(line); // 将当前行包装成输入流，方便拆分字段。
			ScoreRecord score; // 创建一条临时成绩记录。
			if (std::getline(record, score.nickname, '\t') && record >> score.score && !score.nickname.empty()) // 按“昵称+制表符+分数”格式校验数据。
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
		if (nickname.empty() || score < 0) // 拒绝空昵称和负数成绩。
		{
			return false; // 告知调用者本次保存参数无效。
		}
		return Utils::File::File_add(nickname + "\t" + std::to_string(score), scoreFile); // 调用 FileHelper 将昵称和分数追加写入文件。
	}

	int PersonalBest(const std::string& nickname, const std::vector<ScoreRecord>& scores) // 查询指定玩家的历史最高分。
	{
		int best = 0; // 默认最高分为零，适用于没有历史记录的玩家。
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
		if (scores.empty()) // 判断本地是否还没有任何历史成绩。
		{
			std::cout << "暂无历史成绩\n"; // 提示玩家当前没有可展示的记录。
			return; // 没有记录时直接结束展示函数。
		}
		const std::size_t count = std::min<std::size_t>(scores.size(), 10); // 最多展示前十名，避免结束界面过长。
		for (std::size_t index = 0; index < count; ++index) // 依次输出排行中的每一条记录。
		{
			std::cout << index + 1 << ". " << scores[index].nickname << " : " << scores[index].score << " 分\n"; // 输出名次、昵称和分数。
		}
	}
} // namespace Account
