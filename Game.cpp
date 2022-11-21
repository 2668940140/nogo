#include "Game.h"
#include "JSON for Modern C++ version 3.11.2/single_include/nlohmann/json.hpp"
#include <cstring>
#include <queue>

bool Game::place_availableQ(const pos& where)
{
    if (state != progressing) return false;
    return availableQ[where.x][where.y];
}

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
    if (state == error) return;

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

    if (cnt == 0) //which means next_part has nowhere to place his/her piece. i.e. the other part has won 
    {
        switch (!next_part)
        {
        case black:
            state = black_win;
            break;
        case white:
            state = white_win;
            break;
        }
    }
    
    return;
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
    next_part = black;
    state = progressing;
    update();
}

Game::Game(const json& content)
{
    state = progressing;
    char row[2]{};

    next_part = content["next_part"];
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            row[0] = i + '0';
            board[i][j] = content[row][j];
        }
    }
    update();
}


json Game::json_content() const
{
    if (state == error) return json();
    
    json output;
    output["next_part"] = next_part;
    char row[2]{ 0 };
    for (int i = 0; i < 9; i++)
    {
        row[0] = i + '0';
        output[row] = json(board[i]);
    }
 
    return output;
}

const std::array<Game::part, 9>& Game::operator[](int row) const
{
    return board[row];
}

Game& Game::place(const pos& where)
{
    if (state != progressing) return *this;

    if (where.x < 0 || where.x >= 9 || where.y < 0 || where.y >= 9)
    {
        state = error;
        return *this;
    }

    if (place_availableQ(where)) //can be placed
    {
        board[where.x][where.y] = next_part;
        next_part = !next_part;
        update();
        return *this;
    }
    else //can't be placed
    {
        if (board[where.x][where.y] != air)
        {
            state = error;
            return *this;
        }
        board[where.x][where.y] = next_part;
        next_part = !next_part;  //now it's who has won
        update();
        switch (next_part)
        {
            case Game::air:
                state = error;
                break;
            case Game::black:
                state = black_win;
                break;
            case Game::white:
                state = white_win;
                break;
            default:
                state = error;
                break;
        }
        return *this;
    }
}

void Game::clear()
{
    if (state == error)
    {
        state = progressing;
        update();
    }
    else
    {
        return;
    }
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

