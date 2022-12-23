#include "C_mcts.h"
#include <vector>
#include <queue>
#include <map>
#include <cstring>
#include <ctime>
#include <cmath>
#include <utility>
#include <cstdlib>

/*
Choose:UCT Root ---> Leaf(has unexplored son)
Expand:Choose an unexplored son, called C
Stimulate:Rollout
Backpropagation:C ----> Root
node++ each time

choice: max n

Data reuse:
-Leaf judge
-Single game continuity
-air counting in one roll out
*/
void Robot::C_mcts::dfs1(int r, int c) 
{
    if (r < 0 || r >= 9 || c < 0 || c >= 9 || vis[r][c])
        return;
    else
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
        for (int i = 0; i < 4; i++)
        {
            dfs1(r + dr[i], c + dc[i]);
        }
    }
}

void Robot::C_mcts::dfs2(int r, int c)
{
    if (r < 0 || r >= 9 || c < 0 || c >= 9 || avaState[r][c])
        return;
    if (!board[r][c])
    {
        avaState[r][c] = 1;
        for (int i = 0; i < 4; i++)
        {
            dfs2(r + dr[i], c + dc[i]);
        }
    }
}

void Robot::C_mcts::dfs3(int r, int c)
{
    if (r < 0 || r >= 9 || c < 0 || c >= 9 || vis[r][c])
        return;
    else
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
        dpAvaState[0][r][c] = dpAvaState[1][r][c] = 2;
        for (int i = 0; i < 4; i++)
        {
            dfs3(r + dr[i], c + dc[i]);
        }
    }
}

void Robot::C_mcts::dfs4(int r, int c)
{
    if (r < 0 || r >= 9 || c < 0 || c >= 9 || (dpAvaState[0][r][c] && dpAvaState[1][r][c]))
        return;
    if (!board[r][c])
    {
        if (!dpAvaState[0][r][c])dpAvaState[0][r][c] = 1;
        if (!dpAvaState[1][r][c])dpAvaState[1][r][c] = 1;
        for (int i = 0; i < 4; i++)
        {
            dfs4(r + dr[i], c + dc[i]);
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
                memset(vis, 0, sizeof(vis));
                color = board[i][j];
                counterColor = board[i][j] % 2 + 1; //1->2 2->1
                airCnt = 0;
                dfs1(i, j);
                if (airCnt == 1) avaState[junctionAir[0]][junctionAir[1]] = 2;
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
                    if (board[x_][y_]==enemy)
                    {
                        enemyCnt++;
                    }
                    else
                    {
                        break;
                    }
                }
                if (enemyCnt==4)
                {
                    avaState[i][j] = 2;
                }
                else
                {
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
                root->sonsIndex[root->unexploredSonsCnt] = 9 * i + j;
                root->totalSonsCnt++;
            }
        }
    }
    root->unexploredSonsCnt = root->totalSonsCnt;
}

void Robot::C_mcts::expand2(int part, node*)
{
    memset(dpAvaState, 0, sizeof(dpAvaState));
    
    int enemy = part % 2 + 1;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (dpAvaState[0][i][j]) continue; //updated
            if (board[i][j]) //not air
            {
                memset(vis, 0, sizeof(vis));
                color = board[i][j];
                counterColor = board[i][j] % 2 + 1; //1->2 2->1
                airCnt = 0;
                dfs3(i, j);
                if (airCnt == 1) dpAvaState[0][junctionAir[0]][junctionAir[1]]
                    = dpAvaState[1][junctionAir[0]][junctionAir[1]]
                    = 2;
            }
            else
            {
                int partCnt = 0;
                int enemyCnt = 0;
                int x_, y_;
                for (int k = 0; k < 4; k++)
                {
                    x_ = i + dr[k]; y_ = j + dr[k];
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
                    dpAvaState[part - 1][i][j] = 1;
                }
                else if (enemyCnt == 4)
                {
                    dpAvaState[part - 1][i][j] = 2;
                    dpAvaState[enemy - 1][i][j] = 1;
                }
                else
                    dfs4(i, j);
            }
        }
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (dpAvaState[part-1][i][j] == 1)
            {
                root->sonsIndex[root->unexploredSonsCnt] = 9 * i + j;
                root->totalSonsCnt++;
            }
        }
    }
    root->unexploredSonsCnt = root->totalSonsCnt;
}

void Robot::C_mcts::expand3(int part, int r, int c)
{
}

const Game::pos Robot::C_mcts::solve(const Game& g, size_t timeLimit, json& info)
{
    clock_t startTime = clock();
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            board[i][j] = g[i][j]; //0air 1black 2white
        }
    }
    root = new node;
    expand1(g.show_next_part(),root);
    //iteration
    
    int path[80]{};
    int pathLen = 0; //for backpropagation
    do
    {
        node* now = root;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                board[i][j] = g[i][j]; //0air 1black 2white
            }
        }
        int next_part = g.show_next_part();
        int counter_part = next_part % 2 + 1;

        //go to leaf:
        while (now->unexploredSonsCnt!=0)
        {
            double maxUCT = 0;
            int index = 0;
            for (int i = 0; i < now->totalSonsCnt; i++)
            {
                double UCT = double(now->sons[now->sonsIndex[i]]->w) / now->sons[now->sonsIndex[i]]->t 
                    + exploreFactor * sqrt(log(now->t) / now->sons[now->sonsIndex[i]]->t);
                if (UCT > maxUCT)
                {
                    maxUCT = UCT;
                    index = i; //refer to sonsIndex subscript
                }
            }
            board[now->sonsIndex[index] / 9][now->sonsIndex[index] % 9] = next_part;
            std::swap(next_part, counter_part);
            path[pathLen] = index;
            pathLen++;
        }

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

        //attention, dpAvaPlace is ordered,but soon unodered -_- ...
        while (avaPlaceCnt[next_part-1])
        {
            srand(clock());
            int choice = rand() % avaPlaceCnt[next_part - 1];
            expand3(next_part, dpAvaPlace[next_part - 1][choice] / 9, dpAvaPlace[next_part - 1][choice] % 9);
;           std::swap(next_part, counter_part);
        }

    } while ((clock()-startTime)<=timeLimit);

    delete root;
    return Game::pos();
}
