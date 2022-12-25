#include "nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <queue>

const int timeLimit = 1000;
using namespace std;
using json = nlohmann::json;


struct Game
{
public:
	enum part
	{
		air,
		black,
		white
	};

	struct pos
	{
		int x;
		int y;
		pos(int i = 0, int j = 0) { x = i; y = j; }
	};

	array<array<part, 9>, 9> board;

	array<array<bool, 9>, 9> availableQ;

	part next_part;

	friend part operator!(const part&);

	friend bool bfs(int i, int j, const Game& g, part who);

	Game();

	~Game() {}

	void update();

	const std::array<Game::part, 9>& operator[](int row) const;

	Game& place(const pos& where);
};


bool bfs(int i, int j, const Game& g, Game::part who)
{
	if (i < 0 || j < 0 || i >= 9 || j >= 9 || g.board[i][j] != who) return true;
	std::queue<Game::pos> q;
	bool vis[9][9]{};
	q.push({ i,j });

	bool ok = false;
	while (!q.empty())
	{
		Game::pos where = q.front();
		q.pop();

		vis[where.x][where.y] = true;

		if (where.x - 1 >= 0)
		{
			if (g.board[where.x - 1][where.y] == Game::air)
			{
				ok = true;
				break;
			}
			else if (g.board[where.x - 1][where.y] == who && !vis[where.x - 1][where.y])
			{
				q.push({ where.x - 1, where.y });
			}
		}

		if (where.x + 1 < 9)
		{
			if (g.board[where.x + 1][where.y] == Game::air)
			{
				ok = true;
				break;
			}
			else if (g.board[where.x + 1][where.y] == who && !vis[where.x + 1][where.y])
			{
				q.push({ where.x + 1, where.y });
			}
		}

		if (where.y - 1 >= 0)
		{
			if (g.board[where.x][where.y - 1] == Game::air)
			{
				ok = true;
				break;
			}
			else if (g.board[where.x][where.y - 1] == who && !vis[where.x][where.y - 1])
			{
				q.push({ where.x, where.y - 1 });
			}
		}

		if (where.y + 1 < 9)
		{
			if (g.board[where.x][where.y + 1] == Game::air)
			{
				ok = true;
				break;
			}
			else if (g.board[where.x][where.y + 1] == who && !vis[where.x][where.y + 1])
			{
				q.push({ where.x, where.y + 1 });
			}
		}
	}
	return ok;
}

void Game::update()
{
	int cnt = 0; //total available places
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] != air) //i.e. there is a piece at that place
			{
				availableQ[i][j] = false;
			}
			else
			{
				//place first , if no eating ,then ok
				board[i][j] = next_part;

				bool ok[5]{};
				ok[0] = bfs(i, j, *this, next_part);
				ok[1] = bfs(i - 1, j, *this, !next_part);
				ok[2] = bfs(i + 1, j, *this, !next_part);
				ok[3] = bfs(i, j - 1, *this, !next_part);
				ok[4] = bfs(i, j + 1, *this, !next_part);

				if (ok[0] && ok[1] && ok[2] && ok[3] && ok[4])
				{
					availableQ[i][j] = true;
					cnt++;
				}
				else
				{
					availableQ[i][j] = false;
				}
				board[i][j] = air; //recovery
			}
		}
	}
}

Game::Game()
{
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			board[i][j] = air;
		}
	}
	update();
	next_part = black;
}

const std::array<Game::part, 9>& Game::operator[](int row) const
{
	return board[row];
}

Game& Game::place(const pos& where)
{
	board[where.x][where.y] = next_part;
	next_part = !next_part;
	return *this;
}

Game::part operator!(const Game::part& p)
{
	if (p == Game::white)
	{
		return Game::black;
	}
	else if (p == Game::black)
	{
		return Game::white;
	}
	else
	{
		return Game::air;
	}
}

//try to make its rival has less place to place, depth = 1
void RandomBot1(Game& g, size_t timeLimit, Game::pos& choice)
{
	g.update();
	std::vector<Game::pos> whereAvailable;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (g.availableQ[i][j])
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
		detect.update();
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (detect.availableQ[i][j])
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
	srand(time(NULL));
	int select = rand() % choices.size();

	choice = whereAvailable[choices[select]];
	return;
}


int main()
{

	Game g;
	Game::pos choice;
	string str;
	getline(cin, str);
	json input = json::parse(str);
	json output;
	if (input["requests"][0]["x"] != -1)
	{
		g.place({ input["requests"][0]["x"] ,input["requests"][0]["y"] });
	}
	RandomBot1(g, timeLimit, choice);
	output["response"]["x"] = choice.x;
	output["response"]["y"] = choice.y;
	g.place(choice);
	std::cout << output;
	std::cout << "\n>>>BOTZONE_REQUEST_KEEP_RUNNING<<<\n";
	std::cout << flush;


	while (getline(cin, str))
	{
		input = json::parse(str);
		g.place({ input["x"] ,input["y"] });
		RandomBot1(g, timeLimit, choice);
		output["response"]["x"] = choice.x;
		output["response"]["y"] = choice.y;
		g.place(choice);
		std::cout << output;
		std::cout << "\n>>>BOTZONE_REQUEST_KEEP_RUNNING<<<\n";
		std::cout << flush;
	}

	return 0;
}