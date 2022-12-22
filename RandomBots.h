#pragma once
#include "Game.h"

namespace Robot {
	const Game::pos RandomBot(const Game& g, size_t timeLimit, json& info);

	const Game::pos RandomBot1(const Game& g, size_t timeLimit, json& info);
}