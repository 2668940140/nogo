#include "Console.h"
#include <ctime>

void sleep(size_t t)
{
	clock_t from = clock();
	while (size_t(clock() - from) * 1000 / CLOCKS_PER_SEC < t);
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