#pragma once
#include "Front.h"
#include "Game.h"
#include <vector>
#include "JSON for Modern C++ version 3.11.2/single_include/nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

static class Console
{
private:
	struct stepInfo
	{
		Game g;
		json blackInfo;
		json whiteInfo;
		stepInfo(const Game &m_g = Game(), const json& m_blackInfo = json(), const json& m_whiteInfo = json())
		{
			g = m_g;
			blackInfo = m_blackInfo;
			whiteInfo = m_whiteInfo;
		}
	};

private:
	std::vector<stepInfo> onGoing;
	Front perform;
	inline Game& now() { return onGoing.back().g; }

private:
	//save the series of the games
	json jsonContent() const;

	//read a series of games by json
	void readJson(const json& content);

	//mainly used after any break
	Game::flag keepGo();

	json whiteBotInfo;
	json blackBotInfo;

	Bot BlackBot;
	Bot WhiteBot;
	
public:
	//determine whether it will print or not
	bool performQ = true;

	//recommended time, unit:ms
	size_t botTimeLimit;

	//mainly used for robot vs robot, initializeed as 0, unit:ms
	size_t botPerformDelay;

	Console();
	~Console() {}

	//receive initial info
	Game::flag StartNew(const json& b = json(), const json& w = json());

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

	//the second parameter is the initial info
	void setBlackBot(Bot,json info = json());
	void setWhiteBot(Bot,json info = json());
}console;