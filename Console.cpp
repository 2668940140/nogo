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
	using std::to_string;
	for (int i = 0; i < onGoing.size(); i++)
	{
		out[to_string(i)] = onGoing[i].json_content();
	}
	return out;
}

void Console::readJson(const json& content)
{
	size_t size = content.size();
	onGoing.resize(size);

	using std::to_string;
	for (int i = 0; i < onGoing.size(); i++)
	{
		onGoing[i] = content[to_string(i)];
	}

	keepGo();
	return;
}

Game::flag Console::keepGo()
{
	if (performQ)perform.draw_game(now());
	if (now().show_state() == Game::progressing)
	{
		switch (now().show_next_part())
		{
		case Game::black:
			if (BlackBot != nullptr)
			{
				sleep(botPerformDelay);
				return place(BlackBot(now(), botTimeLimit));
			}
			break;
		case Game::white:
			if (WhiteBot != nullptr)
			{
				sleep(botPerformDelay);
				return place(WhiteBot(now(), botTimeLimit));
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

Game::flag Console::StartNew()
{
	onGoing.resize(1, Game());
	if (performQ) perform.draw_game(now());

	if (BlackBot != nullptr)
	{
		sleep(botPerformDelay);
		return place(BlackBot(now(), botTimeLimit));
	}
	return now().show_state();
}

Game::flag Console::place(const Game::pos& where)
{
	onGoing.emplace_back(now());
	onGoing.back().place(where);
	if (performQ)perform.draw_game(now());
	if (now().show_state() == Game::progressing)
	{
		switch (now().show_next_part())
		{
		case Game::black:
			if (BlackBot != nullptr)
			{
				sleep(botPerformDelay);
				return place(BlackBot(now(), botTimeLimit));
			}
			break;
		case Game::white:
			if (WhiteBot != nullptr)
			{
				sleep(botPerformDelay);
				return place(WhiteBot(now(), botTimeLimit));
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
