#include "Console.h"
#include "Robots.h"
int main()
{
	console.botPerformDelay = 500;
	console.BlackBot = console.WhiteBot = RandomBot;
	console.StartNew();
	return 0;
}