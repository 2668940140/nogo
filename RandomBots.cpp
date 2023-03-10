#include "RandomBots.h"
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

//This is used as a test. random, gaurantee not suicide
const Game::pos Robot::RandomBot(const Game& g, size_t timeLimit, json& info)
{
	clock_t beginTime = clock();
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

	size_t size = whereAvailable.size();
	srand(clock());
	size_t select = rand() % size;

	while (clock() - beginTime<double(timeLimit) * 1000 / CLOCKS_PER_SEC);

	return whereAvailable[select];
}

//try to make its rival has less place to place, depth = 1
const Game::pos Robot::RandomBot1(const Game& g, size_t timeLimit, json& info)
{
	clock_t beginTime = clock();
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

	size_t size = whereAvailable.size();
	std::vector<int> cnts(size);
	int MinPlaces = INT32_MAX;
	for (size_t l = 0; l < size; l++)
	{
		Game detect(g);
		detect.place(whereAvailable[l]);
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (detect.show_available()[i][j])
				{
					cnts[l]++;
				}
			}
		}
		if (cnts[l] < MinPlaces)
		{
			MinPlaces = cnts[l];
		}
	}

	std::vector<int> choices;
	
	for (size_t i = 0; i < cnts.size(); i++)
	{
		if (cnts[i] == MinPlaces) choices.push_back(i);
	}
	srand(clock());
	int select = rand() % choices.size();

	while (clock() - beginTime<double(timeLimit) * 1000 / CLOCKS_PER_SEC);
	return whereAvailable[choices[select]];
}
