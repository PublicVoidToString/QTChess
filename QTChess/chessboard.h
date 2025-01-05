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
    ~ChessBoard();

private slots:
    // Slots are QT environment specific functions
    // that can be connected to signals
    void undoMove();
    void handleButtonClick(int buttonId);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ChessBoard* ui;
    // Array of chess tiles used for display
    QPushButton** chessTiles;
    // Reference to the current board
    Board* board;

    void initBoard();
    void printAllPieces();
    void printSelection();
    void clearSelectedFromBoard();
};

#endif // CHESSBOARD_H
