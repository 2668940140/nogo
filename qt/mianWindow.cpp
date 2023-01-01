#include "mianWindow.h"
#include "gameWindow.h"


mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.newGameButton, SIGNAL(pressed()),this, SLOT(show_gameWindow()));
}

mainWindow::~mainWindow()
{
}

void mainWindow::show_gameWindow()
{
    gameWindow *window1 = new gameWindow(this);
    window1->show();
}