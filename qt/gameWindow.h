#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gameWindow.h"

class gameWindow : public QMainWindow
{
    Q_OBJECT

public:
    gameWindow(QWidget* parent = nullptr);
    ~gameWindow();
private:
    QWidget* father;
    Ui::gameWindow ui;
private slots:
    void showMain();
};
