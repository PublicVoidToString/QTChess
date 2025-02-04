#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "chessboard.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainMenu;
}
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void openChessBoard();
    void openChessBoardBot();

private:
    char level;
    Ui::MainMenu *ui;
    QPushButton *pushButton;
    ChessBoard *chessBoard;

};

#endif // MAINMENU_H
