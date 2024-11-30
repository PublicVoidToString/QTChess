#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>  // Dodajemy layout
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
    void openChessBoard();  // Slot, który będzie uruchamiał ChessBoard

private:
    Ui::MainMenu *ui;
    QPushButton *pushButton;  // Deklaracja przycisku
    ChessBoard *chessBoard;
};

#endif // MAINMENU_H
