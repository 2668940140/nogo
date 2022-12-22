#pragma once
#include "Game.h"
#include <map>
#include <stack>

const int CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000;

namespace Robot {
	/*
	Break-MCTS Bot
	implement a mcts bot without a continuous network but with break
	*rollout bot is not adapted to use the info before,but to update everything each time
	*no expanding
	*to many additional nodes
	*/
	class B_mcts
	{
	public:
		struct Node
		{
			Game circumstance;
			unsigned wins; //for the part who has just placed , i.e. !next_part
			unsigned total;
			std::map<Game::pos, Node*> next;
			Node(const Game& cir, unsigned w = 0, unsigned t = 0)
			{
				circumstance = cir;
				wins = w;
				total = t;
				for (int i = 0; i < 9; i++)
				{
					for (int j = 0; j < 9; j++)
					{
						if (circumstance.show_available()[i][j] == true)
						{
							next.insert({ { i,j }, nullptr });
						}
					}
				}
			}

			//Attention, all must be initialized by new, except for the root;
			~Node()
			{
				for (auto& p : next)
				{
					delete p.second;
				}
			}
		};

		friend bool leafQ(Node* leaf);
	public:
		//for clear vision
		const Game::pos solve(const Game& g, size_t timeLimit, json& info);

		const double exploreFactor = 1.414;

		Bot rollBot;
		B_mcts(Bot rollout, double exploreF = 1.414) : exploreFactor(exploreF)
		{
			rollBot = rollout;
		}
	};
}