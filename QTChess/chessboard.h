#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "Board.h"
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
    QString convertBitmaskToString(unsigned long long legalMovesBitmap);
    ~ChessBoard();

private slots:
    void initBoard();  // Slot, który będzie uruchamiał ChessBoard
    void undoMove();
    void handleButtonClick(int buttonId);
    void printSelection();
    void clearSelectedFromBoard();
    void printAllPieces();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ChessBoard* ui;
    QPushButton** chessTiles;
    Board* board; //TO JEST NASZ AKTUALNY BOARD
};

#endif // CHESSBOARD_H
