#include "mianWindow.h"
#include <QtWidgets/QApplication>
#include "Console.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainWindow window0;
    window0.show();
    return app.exec();
}
