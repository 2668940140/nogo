#include "Console.h"
#include "Robots.h"
int main()
{
	//console.botPerformDelay = 500;
	console.BlackBot = Robot::RandomBot;
	console.WhiteBot = Robot::RandomBot1;
	console.StartNew();
	return 0;
}