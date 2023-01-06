#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gameWindow.h"
#include "console.h"
class gameWindow : public QDialog
{
    Q_OBJECT
public:
    gameWindow(QWidget* parent = nullptr);
    ~gameWindow();
private:
    Ui::gameWindow ui;
    Console console;
};
