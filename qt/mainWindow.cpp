#include "mainWindow.h"
#include "gameWindow.h"
#include <QMessageBox>
#include <qfiledialog.h>
mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.newGameButton, SIGNAL(released()), this, SLOT(show_gameWindow()));
    connect(ui.help, SIGNAL(released()), this, SLOT(rule()));
    connect(ui.loadGameButton, SIGNAL(released()), this, SLOT(readGame()));
}

mainWindow::~mainWindow()
{
}

void mainWindow::readGame()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("读取存档"), "/home/", tr("JSON Files (*.json)"));
    if (!fileName.isEmpty())
    {
        gameWindow* window1 = new gameWindow(fileName);
        window1->exec();
    }
}

void mainWindow::rule()
{
    QMessageBox msgBox;
    msgBox.setText(R"(落子先后：黑子先手，双方轮流落子，落子后棋子不可移动。
判负条件：不围棋没有平局。一方输掉比赛，当且仅当以下至少一条发生：
1）如果一方落子后吃掉了对方的棋子，则落子一方判负；
2）对弈禁止自杀，落子自杀一方判负；
3）对弈禁止空手(pass)，空手一方判负。)");
    msgBox.setWindowTitle("规则");
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.exec();
    return;
}

void mainWindow::show_gameWindow()
{
    gameWindow* window1 = new gameWindow;
    window1->exec();
}