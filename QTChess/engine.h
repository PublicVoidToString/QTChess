#ifndef ENGINE_H
#define ENGINE_H
#include "board.h"
#include "evaluation.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "king.h"
#include <QMessageBox>

class Engine
{
private:

    // Engine Functions
    static void nextMove(Board* board, unsigned char from, unsigned char to); // Function creating new Board instance on next and perfoming move on it

    // TODO: Finish + Maybe move to private after it's working
    static void engineNextMove(Board* board); // Function playing the move calculated as best by the engine
    static void buildFutureGameTree(Board* startingBoard, int n); // Function to calculate all possible moves for a n depth in the game tree.
public:
    Engine();
    // Main Functions
    static bool pressedButton(Board* board, int buttonId, unsigned char* selected, unsigned char* clearSelected, unsigned long long* moves, unsigned long long* clearMoves );
};

#endif // ENGINE_H
