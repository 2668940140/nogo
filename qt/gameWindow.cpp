#include "gameWindow.h"
#include "robots.h"

gameWindow::gameWindow(QWidget* parent)
{
	ui.setupUi(this);
	ui.gameGraphics->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.gameGraphics->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	console.setGraph(ui.gameGraphics);
	console.setWhiteBot(Robot::RandomBot);
	console.StartNew();
}

gameWindow::~gameWindow()
{
}