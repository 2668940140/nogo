#pragma once
#include "Game.h"
#include <QtWidgets/qgraphicsview.h>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
class Front
{
private:
	QPixmap airGrid;
	QPixmap blackGrid;
	QPixmap whiteGrid;
	QGraphicsPixmapItem* grids[81]{};
public:
	QGraphicsScene* scene;
public:
	Front();
	~Front();
	void initialize();
public:
	void draw_game(const Game& g)const;
};

