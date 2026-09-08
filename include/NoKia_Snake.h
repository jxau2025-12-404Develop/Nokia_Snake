// Nokia_Snake.h
#pragma once
#include <memory>

#include "raylib.h"

#include "Utils.h"
#include "ScoreAndAccount.h"

// 重置/初始化
void InitGame(GameView& GV);

// 单次游戏的函数
void Game(GameView& GV, Music backgroundMusic);

// 用户的登录和注册
void LoginSignIN(GameView& GV, std::unique_ptr<Account::ScoreAccount>& player);

// 用户的登录和注册
void LoginSignIN(GameView& GV, std::unique_ptr<Account::ScoreAccount>& player);

// 程序运行的主函数
void Nokia_Snake(Music backgroundMusic);