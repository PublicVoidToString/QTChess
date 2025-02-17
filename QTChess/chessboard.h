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
    void setDepth(char level);
    void botVsBot();
    ~ChessBoard();

private slots:
    // Slots are QT environment specific functions
    // that can be connected to signals
    void handleButtonClick(int buttonId);
    void reprintBoard();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    unsigned char botDepth; //Depth level of chess engine, 0=against player
    unsigned char selected; //Selected tile on the board remembered after clicked
    unsigned char clearSelected; //Previous selected tile to be cleared in next move
    unsigned long long moves; //Moves bitboard of all possible moves of figure on selected tile
    unsigned long long clearMoves; //Previous Moves bitboard to be cleared in next move
    unsigned int windowSize;
    bool gameEnded = false;
    Ui::ChessBoard* ui;
    // Array of chess tiles used for display
    QPushButton** chessTiles;
    // Reference to the current board
    Board* board;

    void initBoard();
    void printAllPieces();
    void printSelection();
    void printOutcome(QString outcome);
    void clearSelectedFromBoard();
};

#endif // CHESSBOARD_H
