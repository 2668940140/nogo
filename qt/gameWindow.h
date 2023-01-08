#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_gameWindow.h"
#include "console.h"
#include <QStringList>
#include <QListWidgetItem>
class gameWindow : public QDialog
{
    Q_OBJECT
public:
    gameWindow(QString = "");
    ~gameWindow();
private:
    Ui::gameWindow ui;
    Console console;
    QListWidgetItem* listItems[2][5]{0};
private slots:
    void whiteBotChanged();
    void blackBotChanged();
    void regret();
    void save();
    void read();
    void newGame();
};
