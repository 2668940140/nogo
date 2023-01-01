#include "gameWindow.h"

gameWindow::gameWindow(QWidget* parent)
{
	ui.setupUi(this);
	father = parent;
	connect(this, SIGNAL(destroyed(QObject*)), SLOT(showMain()));
}

gameWindow::~gameWindow()
{

}
void gameWindow::showMain()
{

}