#include "B_mcts.h"
#include "Game.h"
#include <map>
#include <ctime>
#include <cmath>
#include <stack>
#include <cstdlib>

using std::stack;

bool Robot::leafQ(B_mcts::Node* leaf)
{
	if (leaf->next.empty()) return true;
	for (auto& p : leaf->next)
	{
		if (p.second == nullptr)
		{
			return true;
		}
	}
	return false;
}

const Game::pos Robot::B_mcts::solve(const Game& g, size_t timeLimit, json& info)
{
	clock_t startTime = clock();

	Node *root = new Node(g);

	do
	{
		srand(clock());
		Node* leaf = root;

		//to be changed after one rollout
		stack<Node*> waitingStack;
		waitingStack.push(leaf);

		//first find leaf
		while (!leafQ(leaf))
		{
			double N = leaf->total;
			double heaviestWeight = -1;
			Node* nextLeaf = nullptr;
			for (auto &son : leaf->next)
			{
				auto &p = *son.second;
				double weight = double(p.wins) / p.total + exploreFactor* sqrt(log(N) / p.total);

				if (weight > heaviestWeight)
				{
					heaviestWeight = weight;
					nextLeaf = son.second;
				}
			}
			leaf = nextLeaf;
			waitingStack.push(leaf);
		}

		//now leaf has son who is not detected
		//begin to rollout
		Game::part loser = Game::air;
		Game tmpGame(leaf->circumstance);
		json info; //if no initial info
		while (tmpGame.show_state() == Game::progressing) 
		{
			Game::pos tmpPos = rollBot(tmpGame, timeLimit, info);
			tmpGame.place(tmpPos);
			Node* nextLeaf = leaf->next[tmpPos];
			if (nextLeaf == nullptr)
			{
				nextLeaf = new Node(tmpGame);
				leaf->next[tmpPos] = nextLeaf;
			}
			leaf = nextLeaf;
			waitingStack.push(leaf);
		}

		loser = tmpGame.show_next_part();
		while (!waitingStack.empty())
		{
			Node* now = waitingStack.top();
			waitingStack.pop();

			now->total++;
			if (now->circumstance.show_next_part() == loser) //next is loser, i.e. this is the winner
			{
				now->wins++;
			}
		}
	}while (size_t(clock() - startTime) / CLOCKS_PER_MS < timeLimit);

	unsigned BiggestValue = 0;
	Game::pos ans;
	for (auto &p: root->next)
	{
		if (p.second != nullptr && p.second->total > BiggestValue)
		{
			BiggestValue = p.second->total;
			ans = p.first;
		}
	}
	int wins =  root->next[ans]->wins;
	int total = root->next[ans]->total;
	delete root;
	return ans;
}

