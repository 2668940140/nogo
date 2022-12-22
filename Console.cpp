#include "Console.h"
#include <fstream>
#include <string>
#include <ctime>
#include "JSON for Modern C++ version 3.11.2/single_include/nlohmann/json.hpp"

void sleep(size_t t)
{
	clock_t from = clock();
	while (size_t(clock() - from) * 1000 / CLOCKS_PER_SEC < t);
}

json Console::jsonContent() const
{
	json out;
	std::string index;
	using std::to_string;
	for (int i = 0; i < onGoing.size(); i++)
	{
		index = to_string(i);
		out[index] = onGoing[i].g.json_content();
		out[index]["Black"] = onGoing[i].blackInfo;
		out[index]["White"] = onGoing[i].blackInfo;
	}
	return out;
}

void Console::readJson(const json& content)
{
	size_t size = content.size();
	std::string index;
	onGoing.resize(size);

	using std::to_string;
	for (int i = 0; i < onGoing.size(); i++)
	{
		index = to_string(i);
		onGoing[i].g = content[index];
		onGoing[i].blackInfo = content[index]["Black"];
		onGoing[i].whiteInfo = content[index]["White"];
	}
	keepGo();
	return;
}

Game::flag Console::keepGo()
{
	if (performQ)perform.draw_game(now());
	blackBotInfo = onGoing.back().blackInfo;
	whiteBotInfo = onGoing.back().whiteInfo;
	if (now().show_state() == Game::progressing)
	{
		switch (now().show_next_part())
		{
		case Game::black:
			if (BlackBot != nullptr)
			{
				sleep(botPerformDelay);
				return place(BlackBot(now(), botTimeLimit, blackBotInfo));
			}
			break;
		case Game::white:
			if (WhiteBot != nullptr)
			{
				sleep(botPerformDelay);
				return place(WhiteBot(now(), botTimeLimit, whiteBotInfo));
			}
			break;
		default:
			return now().show_state();
		}
	}
	return now().show_state();
}

Console::Console()
{
	BlackBot = nullptr;
	WhiteBot = nullptr;
	botTimeLimit = INT32_MAX;
	botPerformDelay = 0;
}

Game::flag Console::StartNew(const json& b, const json& w)
{
	blackBotInfo = b;
	whiteBotInfo = w;
	onGoing.assign(1, { Game(),blackBotInfo,whiteBotInfo });
	if (performQ) perform.draw_game(now());
	if (BlackBot != nullptr)
	{
		sleep(botPerformDelay);
		return place(BlackBot(now(), botTimeLimit, blackBotInfo));
	}
	return now().show_state();
}

Game::flag Console::place(const Game::pos& where)
{
	onGoing.emplace_back(now());
	onGoing.back().g.place(where);
	onGoing.back().blackInfo = blackBotInfo;
	onGoing.back().whiteInfo = whiteBotInfo;
	if (performQ)perform.draw_game(now());
	if (now().show_state() == Game::progressing)
	{
		switch (now().show_next_part())
		{
		case Game::black:
			if (BlackBot != nullptr)
			{
				sleep(botPerformDelay);
				return place(BlackBot(now(), botTimeLimit, blackBotInfo));
			}
			break;
		case Game::white:
			if (WhiteBot != nullptr)
			{
				sleep(botPerformDelay);
				return place(WhiteBot(now(), botTimeLimit, whiteBotInfo));
			}
			break;
		default:
			break;
		}
	}

	//here means there's not robot for the next step
	return now().show_state();
}

Game::flag Console::takeBack(unsigned step)
{
	size_t size = onGoing.size();
	if (step >= size)
	{
		return Game::error;
	}
	else
	{
		onGoing.resize(size - step);
		blackBotInfo = whiteBotInfo = json();
		return keepGo();
	}
}

void Console::save(const std::string& name)
{
	std::fstream fs;
	fs.open(name, std::ios_base::out);
	fs << jsonContent();
	fs.close();
	return;
}

void Console::read(const std::string& name)
{
	using json = nlohmann::json;
	std::fstream fs;
	fs.open(name, std::ios_base::in);
	readJson(json::parse(fs));
	fs.close();
	return;
}

void Console::setBlackBot(Bot bot, json info)
{
	BlackBot = bot;
	blackBotInfo = info;
	return;
}

void Console::setWhiteBot(Bot bot, json info)
{
	WhiteBot = bot;
	whiteBotInfo = info;
	return;
}
