#include "Front.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
Front::Front()
{	
}

Front::~Front()
{
}

void Front::initialize()
{
	airGrid.load(":/mianWindow/resources/airGrid.jpg");
	blackGrid.load(":/mianWindow/resources/blackGrid.jpg");
	whiteGrid.load(":/mianWindow/resources/whiteGrid.jpg");
	QBrush backGroundBrush;
	backGroundBrush.setTextureImage(QImage(":/mianWindow/resources/darkWood.jpg"));
	scene->setBackgroundBrush(backGroundBrush);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			grids[9 * i + j] = scene->addPixmap(airGrid);
			grids[9 * i + j]->setPos(i * 50 + 10 * (i + 1), j * 50 + 10 * (j + 1));
		}
	}
}

void Front::draw_game(const Game& g) const
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			switch (g[i][j])
			{
			case Game::air:
				grids[9 * i + j]->setPixmap(airGrid);
				break;
			case Game::black:
				grids[9 * i + j]->setPixmap(blackGrid);
				break;
			case Game::white:
				grids[9 * i + j]->setPixmap(whiteGrid);
				break;
			default:
				break;
			}
		}
	}

	return;
}