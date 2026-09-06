#pragma once
//引入字符串和动态数组容器
#include <string>
#include <vector>
//创建Account命名空间，存放分数记录相关代码
namespace Account
{
    //分数记录结构体，保存玩家昵称与单次得分
    struct ScoreRecord
    {
        //分数记录结构体，保存玩家昵称与单次得分
        std::string nickname;//玩家昵称
        int score;//本次得分
    };

    //从文件加载全部分数记录，返回记录数组
    std::vector<ScoreRecord> LoadScores();
     //保存一条玩家得分到文件，成功返回true，失败false
    bool SaveScore(const std::string& nickname, int score);
    //查询指定玩家的历史最高分，传入全部分数记录
    int PersonalBest(const std::string& nickname, const std::vector<ScoreRecord>& scores);
    //打印分数排行榜，分数从高到低展示
    void ShowRanking(const std::vector<ScoreRecord>& scores);
}