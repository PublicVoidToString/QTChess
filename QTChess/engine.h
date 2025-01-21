#ifndef ENGINE_H
#define ENGINE_H
#include "board.h"
#include <QMessageBox>
#include <QCoreApplication>

class Engine
{
private:
    // Engine Functions
    static void nextMove(Board* board, unsigned char from, unsigned char to); // Function creating new Board instance on next and perfoming move on it

    static Board* getBestMove(Board* startingBoard);
public:
    Engine();
    // Main Functions
    static void pressedButton(Board** board, int buttonId, unsigned char* selected, unsigned char* clearSelected, unsigned long long* moves, unsigned long long* clearMoves, char botDepth );

    static Board* engineNextMove(Board* board,char botDepth); // Function playing the move calculated as best by the engine

    static void buildFutureGameTree(Board* startingBoard, int n); // Function to calculate all possible moves for a n depth in the game tree.

    static unsigned long long getMoves(short from, Board* startingBoard);

    static unsigned long long getLegalMoves(short from, Board* startingBoard);

    static int printPossibleMoveCount(Board* startingBoard, int count=-1);

    static void printMoveDebug(Board* startingBoard);
};

#endif // ENGINE_H
