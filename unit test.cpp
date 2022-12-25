#include "Console.h"
#include "Robots.h"
#include <iostream>

using namespace std;
int main() {

	console.botTimeLimit = 1000;
	console.setBlackBot(Robot::c_mctsBotGreedy);
	console.setWhiteBot(Robot::RandomBot1);
	console.StartNew();

	/*
	int limit;
	cin >> limit;
	console.botTimeLimit = limit;
	int part;
	int r = 0, c = 0;
	while (cin >> part)
	{
		if (part)
		{
			console.setBlackBot(Robot::c_mctsBotGreedy);
		}
		else
		{
			console.setWhiteBot(Robot::c_mctsBotGreedy);
		}
		console.StartNew();
		
		while (cin >> r >> c && console.gameNow() == Game::progressing)
		{
			console.place({ r,c });

		}
	}
	return 0;
	*/
}
