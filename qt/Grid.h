#pragma once

#include <QGraphicsPixmapItem>

class Grid  : public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	Grid();
	~Grid();
protected:
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};
