#include "Console.h"
#include "Robots.h"
#include <iostream>

using namespace std;
int main() {
	console.botTimeLimit = 1000;
	console.setBlackBot(Robot::c_mctsBot);
	console.setWhiteBot(Robot::RandomBot1);
	console.StartNew();
	return 0;
}
