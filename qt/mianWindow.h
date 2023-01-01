#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mianWindow.h"

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private:
    Ui::window0 ui;
private slots:
    void show_gameWindow();
};
