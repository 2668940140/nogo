#include "gameWindow.h"
#include "robots.h"
#include <QFileDialog>

gameWindow::gameWindow(QString fileName)
{
	ui.setupUi(this);
	ui.gameGraphics->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.gameGraphics->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	console.setGraph(ui.gameGraphics);
	console.setWhiteBot(Robot::RandomBot);
	console.botTimeLimit = 1000;
	console.StartNew();

	listItems[0][0] = new QListWidgetItem("人类", ui.blackList);
	listItems[0][1] = new QListWidgetItem("随机", ui.blackList);
	listItems[0][2] = new QListWidgetItem("一层贪心", ui.blackList);
	listItems[0][3] = new QListWidgetItem("mcts半贪心", ui.blackList);
	listItems[0][4] = new QListWidgetItem("mcts贪心", ui.blackList);
	listItems[1][0] = new QListWidgetItem("人类", ui.whiteList);
	listItems[1][1] = new QListWidgetItem("随机", ui.whiteList);
	listItems[1][2] = new QListWidgetItem("一层贪心", ui.whiteList);
	listItems[1][3] = new QListWidgetItem("mcts半贪心", ui.whiteList);
	listItems[1][4] = new QListWidgetItem("mcts贪心", ui.whiteList);
	ui.blackList->setCurrentItem(listItems[0][0]);
	ui.whiteList->setCurrentItem(listItems[1][1]);
	connect(ui.blackList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(blackBotChanged()));
	connect(ui.whiteList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(whiteBotChanged()));
	connect(ui.regretButton, SIGNAL(released()), this, SLOT(regret()));
	connect(ui.save, SIGNAL(released()), this, SLOT(save()));
	connect(ui.read, SIGNAL(released()), this, SLOT(read()));
	connect(ui.newGame, SIGNAL(released()), this, SLOT(newGame()));
	console.setLabel(ui.statusLabel_2);
	if (!fileName.isEmpty())
		console.read(fileName.toStdWString());
}

void gameWindow::blackBotChanged()
{
	auto current = ui.blackList->currentItem();
	if (current == listItems[0][0])
	{
		console.setBlackBot(nullptr);
		console.keepGo();
	}
	else if (current == listItems[0][1])
	{
		console.setBlackBot(Robot::RandomBot);
		console.keepGo();
	}
	else if (current == listItems[0][2])
	{
		console.setBlackBot(Robot::RandomBot1);
		console.keepGo();
	}
	else if (current == listItems[0][3])
	{
		console.setBlackBot(Robot::c_mctsBotGreedy);
		console.keepGo();
	}
	else if (current == listItems[0][4])
	{
		console.setBlackBot(Robot::c_mctsMinMax);
		console.keepGo();
	}
	return;
}
void gameWindow::regret()
{
	int human = int(ui.blackList->currentItem() == listItems[0][0]) + int(ui.whiteList->currentItem() == listItems[1][0]);
	console.takeBack(human%2+1);
}
void gameWindow::save()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("保存存档"), "/home/", tr("JSON Files (*.json)"));
	if (!fileName.isEmpty())console.save(fileName.toStdWString());
}
void gameWindow::read()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("读取存档"), "/home/", tr("JSON Files (*.json)"));
	if (!fileName.isEmpty())
		console.read(fileName.toStdWString());
}
void gameWindow::newGame()
{
	console.StartNew();
}
void gameWindow::whiteBotChanged()
{
	auto current = ui.whiteList->currentItem();
	if (current == listItems[1][0])
	{
		console.setWhiteBot(nullptr);
		console.keepGo();
	}
	else if (current == listItems[1][1])
	{
		console.setWhiteBot(Robot::RandomBot);
		console.keepGo();
	}
	else if (current == listItems[1][2])
	{
		console.setWhiteBot(Robot::RandomBot1);
		console.keepGo();
	}
	else if (current == listItems[1][3])
	{
		console.setWhiteBot(Robot::c_mctsBotGreedy);
		console.keepGo();
	}
	else if (current == listItems[1][4])
	{
		console.setWhiteBot(Robot::c_mctsMinMax);
		console.keepGo();
	}
	return;
}


gameWindow::~gameWindow()
{
}