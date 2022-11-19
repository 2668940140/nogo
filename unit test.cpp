#include "Console.h"
#include "Robots.h"
#include <iostream>

using namespace std;
int main() {
	/*
	console.WhiteBot = console.BlackBot = Robot::RandomBot;
	console.performQ = false;
	console.StartNew();
	console.save("save");
	*/

	console.read("save");
	int step;
	while (cin >> step)
	{
		console.takeBack(step);
	}
	return 0;
}