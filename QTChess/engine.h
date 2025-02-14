#ifndef ENGINE_H
#define ENGINE_H
#include "board.h"
#include <QMessageBox>
#include <QCoreApplication>

class Engine
{
private:
    // Engine Functions
    static Board* getBestMove(Board* startingBoard);

public:
    Engine();
    // Main Functions
    static bool isPromotion(Board* board, int buttonId, unsigned char selected, unsigned long long moves);

    static void pressedButton(Board** board, int buttonId, unsigned char* selected, unsigned long long* moves, char botDepth, unsigned short promotion=0);

    static bool hasLegalMoves(Board* board);

    static unsigned long long getLegalMoves(short from, Board* startingBoard);

    static int printPossibleMoveCount(Board* startingBoard, int count=-1);

    static void printMoveDebug(Board* startingBoard);

    // diffrent variations of minimax algorithm
    static void minimaxTreeSearch(Board* board, int n);
    static void alphaBetaTreeSearch(Board* board, int n, double alpha, double beta);

    static short isGameEnded(Board* startingBoard);
    static bool setIfGameEnded(Board* startingBoard);
};

#endif // ENGINE_H
