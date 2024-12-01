#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "board.h"
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

namespace Ui {
class ChessBoard;
}

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = nullptr);
    ~ChessBoard();

private slots:
    void initBoard();  // Slot, który będzie uruchamiał ChessBoard
    void handleButtonClick(int buttonId);
    void printPieces();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ChessBoard *ui;
    QPushButton **chessTiles;
    board **board;
};

#endif // CHESSBOARD_H
