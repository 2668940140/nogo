#pragma once
#include "Front.h"
#include "Game.h"
#include <vector>
#include "JSON for Modern C++ version 3.11.2/single_include/nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

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

private:
	//save the series of the games
	json jsonContent() const;

	//read a series of games by json
	void readJson(const json& content);

	//mainly used after any break
	Game::flag keepGo();
	
public:
	//determine whether it will print or not
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

	//note that if steps == 1, and the opponent is a bot, it means bot will replace
	//so steps should be 2 in such circumstance
	//if steps is out of range, do nothing and return false, else true
	Game::flag takeBack(unsigned step);

	//to be updated for path and other config
	void save(const std::string& name);

	//to be updated for path and other config
	void read(const std::string& name);

}console;