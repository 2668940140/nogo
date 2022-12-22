#pragma once
#include "Game.h"
namespace Robot
{
	/*
	continous mcts bot
	everything fully adapted for it
	no deep learning policy network
	*/
	const Game::pos C_mctsBot(const Game& g, size_t timeLimit, json& info);
}