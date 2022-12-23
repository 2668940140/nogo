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
	const Game::pos c_mctsBot(const Game& g, size_t timeLimit, json& info)
	{
		C_mcts c_mcts;
		return c_mcts.solve(g, timeLimit, info);
	}
}

