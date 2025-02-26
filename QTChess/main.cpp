#include "mainmenu.h"
#include "Board.h"
#include "engine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Engine::calcMoves();
    QApplication a(argc, argv);
    MainMenu w;
    w.show();
    return a.exec();
}

