#include "Console.h"
#include "Robots.h"
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

//This is used as a test.

const Game::pos RandomBot(const Game& g, size_t timeLimit)
{
	std::vector<Game::pos> whereAvailable;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (g.show_available()[i][j])
			{
				whereAvailable.push_back({ i,j });
			}
		}
	}

	int size = whereAvailable.size();
	srand(time(NULL));
	int select = rand() % size;

	return whereAvailable[select];
}
