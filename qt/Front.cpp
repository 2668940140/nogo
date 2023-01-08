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
	airGrid.load(":/mianWindow/resources/airGrid.png");
	blackGrid.load(":/mianWindow/resources/blackGrid.png");
	whiteGrid.load(":/mianWindow/resources/whiteGrid.png");
	QBrush backGroundBrush;
	backGroundBrush.setTextureImage(QImage(":/mianWindow/resources/board.png"));
	scene->setBackgroundBrush(backGroundBrush);

	//player = new QMediaPlayer;

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
		view->viewport()->repaint();
		switch (g)
		{
		case Game::error:
			if (label != nullptr)
				label->setText(R"(<html><head/><body><p><span style=" font-size:14pt;">这样不可以~</span></p></body></html>)");
			break;
		case Game::black_win:
			if (label != nullptr)
				label->setText(R"(<html><head/><body><p><span style=" font-size:14pt;">黑棋赢了！</span></p></body></html>)");
			break;
		case Game::white_win:
			if (label != nullptr)
				label->setText(R"(<html><head/><body><p><span style=" font-size:14pt;">白棋赢了！</span></p></body></html>)");
			break;
		case Game::progressing:
			if (label != nullptr)
				switch (g.show_next_part())
				{
				case Game::black:
					label->setText(R"(<html><head/><body><p><span style=" font-size:14pt;">黑棋思考中......</span></p></body></html>)");
					break;
				case Game::white:
					label->setText(R"(<html><head/><body><p><span style=" font-size:14pt;">白棋思考中......</span></p></body></html>)");
					break;
				default:
					break;
				}
			break;
		default:
			break;
		}
	}
	return;
}