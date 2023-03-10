#include "nlohmann/json.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <ctime>
#include <cmath>
#include <utility>
#include <cstdlib>
using json = nlohmann::json;
int choice[2];
int root_next_Part = 0;
int g[9][9]{};
namespace Robot
{
	/*
	continous mcts
	everything fully adapted for it
	no deep learning policy network
	*/
	class C_mcts
	{
    public:

        /*
        leaf <=> unexporedSonsCnt > 0 || win / lose(for next part)
        lose <=> sonsCnt == unexploredSonsCnt == 0;
        */
        struct node
        {
            long long w; // about value
            long long t; // total games
            node* sons[81];
            int unexploredSonsCnt;
            int totalSonsCnt;
            int sonsIndex[81];
            int next_part_ava_cnt;
            int counter_part_ava_cnt;
            node()
            {
                w = t = 0;
                next_part_ava_cnt = counter_part_ava_cnt = 0;
                unexploredSonsCnt = totalSonsCnt = 0;
                memset(sons, 0, sizeof(sons));
                memset(sonsIndex, 0, sizeof(sonsIndex));
            }
            ~node()
            {
                for (int i = unexploredSonsCnt; i < totalSonsCnt; i++)
                {
                    delete sons[sonsIndex[i]];
                }
            }
        };
        node* root = new node;
    private:
        //overall
        const double exploreFactor = 1.414;
        const double greedyFactor = 1.414;
        const json C_mcts_initial_Json = {};
        const int dr[4] = { -1,0,1,0 };
        const int dc[4] = { 0,-1,0,1 };

        int board[9][9] = { 0 };

        //expand and playout:
        bool vis[9][9] = { 0 };
        int airCnt = 0;
        int color;
        int counterColor;
        int junctionAir[2] = { 0 };
        int avaState[9][9] = { 0 }; //0 not update,1 ok, 2 not ok

        //roll out:
        //0 not update,1 ok, 2 not ok
        int dpAvaState[2][9][9] = { 0 };
        int dpAvaPlace[2][80] = { 0 };
        int avaPlaceCnt[2] = { 0 };

        bool ok = false;
    private:
        //dfs with color
        void dfs1(int r, int c);

        //dfs air
        void dfs2(int r, int c);

        //dfs with color in roll out
        //void dfs3(int r, int c); // same as dfs1

        //dfs air in roll out
        void dfs4(int r, int c);

        //dfs if the part can place in this air, which is the only air of one block
        void dfs5(int r, int c, bool& ok);

        void expand1(int part, node*); //normal expand
        void expand2(int part, node*); //expand for the first step in rollout
        inline void eraseAva(int partSubscript, int r, int c);
        void expand3(int, int r, int c); //expand a region for one step in rollout
	public:
		C_mcts(double m_expore = 1.414, double m_greedy = 1.0) : exploreFactor(m_expore), greedyFactor(m_greedy)
		{}
	    void solve(size_t timeLimit, json& info); //with linear down greedy
	};


}
Robot::C_mcts c_mcts(1.414,1.0);
int main()
{
	std::string str;
	getline(std::cin, str);
	json input = json::parse(str);
	json output;
    int x = 0, y = 0;
    if (input["requests"][0]["x"] != -1)
    {
        x = input["requests"][0]["x"];
        y = input["requests"][0]["y"];
        g[x][y] = 1;
        root_next_Part = 2;
    }
    else
    {
        root_next_Part = 1;
    }

    int clock_per_second = (CLOCKS_PER_SEC * 0.95);
    c_mcts.solve(clock_per_second*2,input);

	output["response"]["x"] = choice[0];
	output["response"]["y"] = choice[1];
	std::cout << output;
	std::cout << "\n>>>BOTZONE_REQUEST_KEEP_RUNNING<<<\n";
	std::cout << std::flush;


	while (getline(std::cin, str))
	{
		input = json::parse(str);
        x = input["x"];
        y = input["y"];
        g[x][y] = root_next_Part % 2 + 1;
        int index = x * 9 + y;
        if (c_mcts.root->sons[index] != nullptr)
            c_mcts.root = c_mcts.root->sons[index];
        else
            c_mcts.root = new Robot::C_mcts::node;
        c_mcts.solve(clock_per_second, input);
        std::cout << "{\"response\":{\"x\":" << choice[0] << ", \"y\":" << choice[1] << " } }";
		std::cout << "\n>>>BOTZONE_REQUEST_KEEP_RUNNING<<<\n";
		std::cout << std::flush;
	}
	return 0;
}

void Robot::C_mcts::dfs1(int r, int c)
{
    vis[r][c] = true;

    if (board[r][c] == counterColor)
        return;
    else if (!board[r][c]) //air
    {
        airCnt++;
        junctionAir[0] = r;
        junctionAir[1] = c;
    }
    else //same color
    {
        avaState[r][c] = 2;
        int x_, y_;
        for (int i = 0; i < 4; i++)
        {
            x_ = r + dr[i]; y_ = c + dc[i];
            if (x_ < 0 || x_ >= 9 || y_ < 0 || y_ >= 9 || vis[x_][y_])
                continue;
            dfs1(x_, y_);
        }
    }
}

void Robot::C_mcts::dfs2(int r, int c)
{
    vis[r][c] = 1;
    if (!board[r][c])
    {
        if (!avaState[r][c])
            avaState[r][c] = 1;
        int x_, y_;
        for (int k = 0; k < 4; k++)
        {
            x_ = r + dr[k]; y_ = c + dc[k];
            if (x_ < 0 || x_ >= 9 || y_ < 0 || y_ >= 9 || vis[x_][y_])continue;
            dfs2(x_, y_);
        }
    }

}

void Robot::C_mcts::dfs4(int r, int c)
{
    vis[r][c] = 1;
    if (!board[r][c])
    {
        if (!dpAvaState[0][r][c])
            dpAvaState[0][r][c] = 1;
        if (!dpAvaState[1][r][c])
            dpAvaState[1][r][c] = 1;
        int x_, y_;
        for (int k = 0; k < 4; k++)
        {
            x_ = r + dr[k]; y_ = c + dc[k];
            if (x_ < 0 || x_ >= 9 || y_ < 0 || y_ >= 9 || vis[x_][y_])continue;
            dfs4(x_, y_);
        }
    }
}

void Robot::C_mcts::dfs5(int r, int c, bool& ok)
{
    if (ok) return;
    vis[r][c] = 1;
    if (!board[r][c])
    {
        ok = true;
        return;
    }
    else if (board[r][c] == color)
    {
        int x_, y_;
        for (int k = 0; k < 4; k++)
        {
            x_ = r + dr[k]; y_ = c + dc[k];
            if (x_ < 0 || x_ >= 9 || y_ < 0 || y_ >= 9 || vis[x_][y_])continue;
            dfs5(x_, y_, ok);
        }
    }

}

void Robot::C_mcts::expand1(int part, node* root) //from a board to update the unexplored sons
{
    memset(avaState, 0, sizeof(avaState));
    int enemy = part % 2 + 1;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (avaState[i][j]) continue; //updated
            if (board[i][j]) //not air
            {
                avaState[i][j] = 2;
                memset(vis, 0, sizeof(vis));
                color = board[i][j];
                counterColor = board[i][j] % 2 + 1; //1->2 2->1
                airCnt = 0;
                dfs1(i, j);
                if (airCnt == 1)
                {
                    if (color == enemy)
                        avaState[junctionAir[0]][junctionAir[1]] = 2;
                    else
                    {
                        board[junctionAir[0]][junctionAir[1]] = part;
                        bool ok = false;
                        dfs5(junctionAir[0], junctionAir[1], ok);
                        if (!ok)
                            avaState[junctionAir[0]][junctionAir[1]] = 2;
                        board[junctionAir[0]][junctionAir[1]] = 0;
                    }
                }
            }
            else
            {
                int enemyCnt = 0;
                int x_ = 0, y_ = 0;
                for (int k = 0; k < 4; k++)
                {
                    x_ = i + dr[k];
                    y_ = j + dc[k];
                    if (x_ < 0 || x_ >= 9 || y_ < 0 || y_ >= 9)
                    {
                        enemyCnt++;
                        continue;
                    }
                    if (board[x_][y_] == enemy)
                    {
                        enemyCnt++;
                    }
                    else
                    {
                        break;
                    }
                }
                if (enemyCnt == 4)
                {
                    avaState[i][j] = 2;
                }
                else
                {
                    memset(vis, 0, sizeof(vis));
                    dfs2(i, j);
                }
            }
        }
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (avaState[i][j] == 1)
            {
                root->sonsIndex[root->totalSonsCnt] = 9 * i + j;
                root->totalSonsCnt++;
            }
        }
    }
    root->unexploredSonsCnt = root->totalSonsCnt;
}

void Robot::C_mcts::expand2(int part, node* root)
{
    memset(dpAvaState, 0, sizeof(dpAvaState));

    int enemy = part % 2 + 1;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (dpAvaState[0][i][j] && dpAvaState[1][i][j]) continue; //updated
            if (board[i][j]) //not air
            {
                dpAvaState[0][i][j] = dpAvaState[1][i][j] = 2;
                memset(vis, 0, sizeof(vis));
                color = board[i][j];
                counterColor = board[i][j] % 2 + 1; //1->2 2->1
                airCnt = 0;
                dfs1(i, j);
                if (airCnt == 1)
                {
                    if (color == enemy)
                    {
                        dpAvaState[part - 1][junctionAir[0]][junctionAir[1]] = 2;
                        board[junctionAir[0]][junctionAir[1]] = enemy;
                        bool ok = false;
                        dfs5(junctionAir[0], junctionAir[1], ok);
                        if (!ok)
                            dpAvaState[enemy - 1][junctionAir[0]][junctionAir[1]] = 2;
                        board[junctionAir[0]][junctionAir[1]] = 0;
                    }
                    else
                    {
                        dpAvaState[enemy - 1][junctionAir[0]][junctionAir[1]] = 2;
                        board[junctionAir[0]][junctionAir[1]] = part;
                        bool ok = false;
                        dfs5(junctionAir[0], junctionAir[1], ok);
                        if (!ok)
                            dpAvaState[part - 1][junctionAir[0]][junctionAir[1]] = 2;
                        board[junctionAir[0]][junctionAir[1]] = 0;
                    }
                }
            }
            else
            {
                int partCnt = 0;
                int enemyCnt = 0;
                int x_, y_;
                for (int k = 0; k < 4; k++)
                {
                    x_ = i + dr[k]; y_ = j + dc[k];
                    if (x_ < 0 || x_ >= 9 || y_ < 0 || y_ >= 9)
                    {
                        partCnt++; enemyCnt++;
                        continue;
                    }
                    else if (!board[x_][y_])
                        break;
                    else if (board[x_][y_] == part)
                        partCnt++;
                    else
                        enemyCnt++;
                }
                if (partCnt == 4)
                {
                    dpAvaState[enemy - 1][i][j] = 2;
                }
                else if (enemyCnt == 4)
                {
                    dpAvaState[part - 1][i][j] = 2;
                }
                memset(vis, 0, sizeof(vis));
                dfs4(i, j);
            }
        }
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (dpAvaState[part - 1][i][j] == 1)
            {
                root->sonsIndex[root->totalSonsCnt] = 9 * i + j;
                root->totalSonsCnt++;
            }
        }
    }
    root->unexploredSonsCnt = root->totalSonsCnt;
}

inline void Robot::C_mcts::eraseAva(int partSubscript, int r, int c)
{
    if (dpAvaState[partSubscript][r][c] == 2)return;
    dpAvaState[partSubscript][r][c] = 2;
    avaPlaceCnt[partSubscript]--;
    int choice = 9 * r + c;
    for (int i = 0; i < avaPlaceCnt[partSubscript]; i++)
    {
        if (dpAvaPlace[partSubscript][i] == choice)
        {
            std::swap(dpAvaPlace[partSubscript][i], dpAvaPlace[partSubscript][avaPlaceCnt[partSubscript]]);
            return;
        }
    }
}

void Robot::C_mcts::expand3(int partSubscript, int r, int c)
{
    board[r][c] = partSubscript + 1;
    int enemyScript = partSubscript ^ 1;
    //more unavailable only:
    //1. new block with 1 air
    //2. junction air become has no adjacent air
    eraseAva(partSubscript, r, c);
    eraseAva(enemyScript, r, c);
    memset(vis, 0, sizeof(vis));
    color = board[r][c];
    counterColor = color % 2 + 1;
    airCnt = 0;
    dfs1(r, c);
    if (airCnt == 1)
    {
        eraseAva(enemyScript, junctionAir[0], junctionAir[1]);
        board[junctionAir[0]][junctionAir[1]] = color;
        bool ok = false;
        dfs5(junctionAir[0], junctionAir[1], ok);
        if (!ok)
            eraseAva(partSubscript, junctionAir[0], junctionAir[1]);
        board[junctionAir[0]][junctionAir[1]] = 0;
    }
    int x_, y_;
    for (int k = 0; k < 4; k++)
    {
        x_ = r + dr[k]; y_ = c + dc[k];
        if (x_ < 0 || x_ >= 9 || y_ < 0 || y_ >= 9) continue;
        if (!board[x_][y_])
        {
            int partCnt = 0;
            int enemyCnt = 0;
            int x__, y__;
            for (int l = 0; l < 4; l++)
            {
                x__ = x_ + dr[l]; y__ = y_ + dc[l];
                if (x__ < 0 || x__ >= 9 || y__ < 0 || y__ >= 9)
                {
                    partCnt++; enemyCnt++;
                    continue;
                }
                else if (!board[x__][y__])
                    break;
                else if (board[x__][y__] == partSubscript + 1)
                    partCnt++;
                else
                    enemyCnt++;
            }
            if (partCnt == 4)
            {
                eraseAva(enemyScript, x_, y_);
            }
            else if (enemyCnt == 4)
            {
                eraseAva(partSubscript, x_, y_);
            }
        }
        else
        {
            memset(vis, 0, sizeof(vis));
            color = board[x_][y_];
            counterColor = color % 2 + 1;
            airCnt = 0;
            dfs1(x_, y_);
            if (airCnt == 1)
            {
                eraseAva(counterColor - 1, junctionAir[0], junctionAir[1]);
                board[junctionAir[0]][junctionAir[1]] = color;
                bool ok = false;
                dfs5(junctionAir[0], junctionAir[1], ok);
                if (!ok)
                    eraseAva(color - 1, junctionAir[0], junctionAir[1]);
                board[junctionAir[0]][junctionAir[1]] = 0;
            }
        }
    }

}
inline double sigmoid(double x)
{
	return 1.0 / (1 + exp(-x));
}


void Robot::C_mcts::solve(size_t timeLimit, json& info)
{
	clock_t startTime = clock();
	int oriPiecesCnt = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			board[i][j] = g[i][j]; //0air 1black 2white
			if (board[i][j])
				oriPiecesCnt++;
		}
	}
	root = new node;
	expand1(root_next_Part, root);
	//iteration
	int path[80]{};

	do
	{
		node* now = root;
		int piecesCnt = oriPiecesCnt;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				board[i][j] = g[i][j]; //0air 1black 2white
			}
		}
		int next_part = root_next_Part;
		int counter_part = next_part % 2 + 1;

		//go to leaf:
		int pathLen = 0;

		while (now->unexploredSonsCnt == 0 && now->totalSonsCnt > 0)
		{
			double maxUCT = INT32_MIN;
			int index = 0;
			for (int i = 0; i < now->totalSonsCnt; i++)
			{
				double UCT = double(now->sons[now->sonsIndex[i]]->w) / now->sons[now->sonsIndex[i]]->t
					+ exploreFactor * sqrt(log(now->t) / now->sons[now->sonsIndex[i]]->t)
					+ greedyFactor * (1 - piecesCnt / 81.0) *
					(now->sons[now->sonsIndex[i]]->counter_part_ava_cnt
						- now->sons[now->sonsIndex[i]]->next_part_ava_cnt);
				if (UCT > maxUCT)
				{
					maxUCT = UCT;
					index = i; //refer to sonsIndex subscript
				}
			}
			board[now->sonsIndex[index] / 9][now->sonsIndex[index] % 9] = next_part;
			std::swap(next_part, counter_part);
			now = now->sons[now->sonsIndex[index]];
			path[pathLen] = index;
			pathLen++;
			piecesCnt++;
		}
		if (now->totalSonsCnt == 0) continue;
		//expand:
		now->unexploredSonsCnt--;
		board[now->sonsIndex[now->unexploredSonsCnt] / 9][now->sonsIndex[now->unexploredSonsCnt] % 9] = next_part;
		path[pathLen] = now->unexploredSonsCnt;
		pathLen++;
		std::swap(next_part, counter_part);
		now = now->sons[now->sonsIndex[now->unexploredSonsCnt]] = new node;
		expand2(next_part, now);

		//roll out
		//attention: avaPlaceCnt only decreases
		avaPlaceCnt[0] = avaPlaceCnt[1] = 0;
		for (int part = 0; part < 2; part++)
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					if (dpAvaState[part][i][j] == 1)
					{
						dpAvaPlace[part][avaPlaceCnt[part]] = 9 * i + j;
						avaPlaceCnt[part]++;
					}
				}
			}
		}
		now->next_part_ava_cnt = avaPlaceCnt[next_part - 1];
		now->counter_part_ava_cnt = avaPlaceCnt[(next_part - 1) ^ 1];

		//attention, dpAvaPlace is ordered,but soon unodered -_- ...
		while (avaPlaceCnt[next_part - 1] > 0)
		{
			srand(clock());
			int choice = rand() % avaPlaceCnt[next_part - 1];
			expand3(next_part - 1, dpAvaPlace[next_part - 1][choice] / 9, dpAvaPlace[next_part - 1][choice] % 9);
			;           std::swap(next_part, counter_part);
		}

		//Backpropagation:C ----> Root
		//loser is next_part
		now = root;
		int winAddition = 0;
		if (root_next_Part == next_part) winAddition = 1;
		now->w += winAddition;
		now->t++;
		for (int i = 0; i < pathLen; i++)
		{
			now = now->sons[now->sonsIndex[path[i]]];
			winAddition ^= 1;
			now->w += winAddition;
			now->t++;
		}

	} while ((clock() - startTime) <= timeLimit);

	int index = 0;
	int max_t = 0;
	for (int i = root->unexploredSonsCnt; i < root->totalSonsCnt; i++)
	{
		if (root->sons[root->sonsIndex[i]]->t > max_t)
		{
			max_t = root->sons[root->sonsIndex[i]]->t;
			index = root->sonsIndex[i];
		}
	}

    choice[0] = index / 9;
    choice[1] = index % 9;
    g[choice[0]][choice[1]] = root_next_Part;
    root = root->sons[index];
}
