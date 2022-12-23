#pragma once
#include "Game.h"
#include <cstring>
#include <list>
namespace Robot
{
	/*
	continous mcts
	everything fully adapted for it
	no deep learning policy network
	*/
	class C_mcts
	{
	private:
		/*
		leaf <=> unexporedSonsCnt > 0 || win / lose(for next part)
		lose <=> sonsCnt == unexploredSonsCnt == 0;
		*/
		struct node
		{
			int w; // win games
			int t; // total games
			int sonsCnt;
			node* sons[80];
			int unexploredSonsCnt;
			int totalSonsCnt;;
			int sonsIndex[80];
			node()
			{
				w = t = 0;
				sonsCnt = unexploredSonsCnt = 0;
				memset(sons, 0, sizeof(sons));
				memset(sonsIndex, 0, sizeof(sonsIndex));
			}
			~node()
			{
				for (int i = 0; i < sonsCnt; i++)
				{
					delete sons[i];
				}
			}
		};
	private:
		//overall
		const double exploreFactor = 1.414;
		const int dr[4] = { -1,0,1,0 };
		const int dc[4] = { 0,-1,0,1 };
		const json C_mcts_initial_Json = {};

		int board[9][9] = { 0 };

		//expand and playout:
		bool vis[9][9] = { 0 };
		int airCnt = 0;
		int color;
		int counterColor;
		int junctionAir[2];
		int avaState[9][9]; //0 not update,1 ok, 2 not ok
		node* root;

		//roll out:
		//0 not update,1 ok, 2 not ok
		int dpAvaState[2][9][9]; 
		int dpAvaPlace[2][80];
		int avaPlaceCnt[2];
	private:
		void dfs1(int r, int c); //dfs with color
		void dfs2(int r, int c); //dfs air
		void dfs3(int r, int c); //dfs with color in roll out
		void dfs4(int r, int c); //dfs air in roll out
		
		void expand1(int part, node*); //normal expand
		void expand2(int part, node*); //expand for the first step in rollout
		void expand3(int part, int r, int c); //expand a region for one step in rollout
	public:
		const Game::pos	solve(const Game& g, size_t timeLimit, json& info);
	};
	

}