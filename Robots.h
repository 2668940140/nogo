#pragma once
#include "RandomBots.h"
#include "B_mcts.h"
namespace Robot
{
	const Game::pos b_mctsBot(const Game& g, size_t timeLimit, json& info)
	{
		B_mcts b_mcts(RandomBot);
		return b_mcts.solve(g, timeLimit, info);
	}
}
#include "C_mcts.h"
namespace Robot
{
	const Game::pos c_mctsBotNoGreedy(const Game& g, size_t timeLimit, json& info)
	{
		C_mcts c_mcts(1.414,0);
		return c_mcts.solve(g, timeLimit, info);
	}
	const Game::pos c_mctsBotGreedy(const Game& g, size_t timeLimit, json& info)
	{
		C_mcts c_mcts(1.414, 1.0);
		return c_mcts.solve1(g, timeLimit, info);
	}
}

