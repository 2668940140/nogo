#include "Console.h"
#include "Robots.h"
#include <iostream>

using namespace std;
int main() {
	console.botTimeLimit = 100;
	console.setBlackBot(Robot::C_mctsBot);
	console.setWhiteBot(Robot::RandomBot1);
	console.StartNew();
	return 0;
}
