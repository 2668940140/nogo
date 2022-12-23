#pragma once
/*
    This object tries to store the infomation of the situation of a game.
    And perform some basic judgements.

    Flag of win is set when ont part can't place his/her piece.
*/

#include "JSON for Modern C++ version 3.11.2/single_include/nlohmann/json.hpp"
#include <array>

using std::array;
using json = nlohmann::json;
//Recieve a function as a robot
//first parameter is the situation of the game
//second parameter is the time limit of the calculation
//of course ,not implemented, the bot can ignore it,but better not

class Game
{
public:
   enum part
    {
        air,
        black,
        white
    };
   
   enum flag
   {
       progressing,
       black_win,
       white_win,
       error
   };
    
   struct pos
   {
       int x;
       int y;
       pos(int i = 0, int j = 0) { x = i; y = j; }
   };

   //for map , set and others, lexicographic order, x to y
   friend bool operator<(const pos& a, const pos& b);

   friend bool operator>(const pos& a, const pos& b)
   {
       return b < a;
   }

private:
	 //the borad stores the thing on every point.
	 //0-air   1-black   2-white
	 //note that our 9x9 board is like
	 //(0,0) (1,0) ... (8,0)
	 //(0,1)              .
	 //.                  .
	 //.                  .
	 //.                  .
	 //(0,8)   ...     (8,8)
	 //[x][y] is (x,y)
    array<array<part, 9>, 9> board;

    array<array<bool, 9>, 9> availableQ;

    //the part to place a piece
    part next_part;

    flag state;

private:
    bool place_availableQ(const pos& where);

    //Check every air ,update the availableQ and
    //and decide if there is a winner, if any update the state
    void update();

    //return the counterpart, if p is air, then return air
    friend part operator!(const part&);
    
    //to find if this place has any air
    friend bool bfs(int i, int j, const Game& g, part who); 

public:
    Game();

    ~Game(){}

    //ignore existing error
    Game(const json& content);

    flag show_state() const { return state; }
    
    operator flag() const { return state; }

    part show_next_part() const { return next_part; }

    const array<array<bool, 9>, 9>& show_available() const { return availableQ; }

    //return the object in json
    //only save the board, next_part, other thing waiting to update
    //if an error has been set, then return an empty object
    json json_content() const;

    //if out of range , cracked. Yes
    const std::array<Game::part, 9>& operator[](int row) const;

    //return whether this placement is avalible.
    //IF true, the board has been changed.
    //IF false, nothing has benn changed, an error flag has been set.
    Game& place(const pos& where);

    void read_json_save(const json& content)
    {
        *this = Game(content);
    }
    
    // clear the error flag if any, and reset state to what it should be
    void clear();
    
    void restart() { *this = Game(); }
};

//2nd para: unit ms
typedef const Game::pos(*Bot)(const Game&, size_t timeLimit, json& info);