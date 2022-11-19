#pragma once
#include "Front.h"
#include "Game.h"
#include <vector>

//Recieve a function as a robot
//first parameter is the situation of the game
//second parameter is the time limit of the calculation
//of course ,not implemented, the bot can ignore it,but better not
typedef const Game::pos (*Bot)(const Game&, size_t timeLimit) ;

static class Console
{
private:
	std::vector<Game> onGoing;
	Front perform;
	inline Game& now() { return onGoing.back(); }
	
public:
	bool performQ = true;

	//recommended time, unit:ms
	size_t botTimeLimit;

	//mainly used for robot vs robot, initializeed as 0, unit:ms
	size_t botPerformDelay;

	Bot BlackBot;
	Bot WhiteBot;

	Console();
	~Console() {}

	Game::flag StartNew();

	Game::flag place(const Game::pos&);

	Game::flag operator<<(const Game::pos& where) { return place(where); }

	//Return the game on running
	const Game& gameNow() { return now(); }

}console;