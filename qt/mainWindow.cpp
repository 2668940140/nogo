#include "mainWindow.h"
#include "gameWindow.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.newGameButton, SIGNAL(released()), this, SLOT(show_gameWindow()));
}

mainWindow::~mainWindow()
{
}

void mainWindow::show_gameWindow()
{
    gameWindow* window1 = new gameWindow;
    window1->exec();
}