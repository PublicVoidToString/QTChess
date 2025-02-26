#ifndef PAWN_H
#define PAWN_H
#include "board.h"

class Pawn
{
public:
    static unsigned long long allMoves(int positionId, const Board& board, bool isWhite);
    static unsigned long long fastMoves(int positionId, const Board& board);
    static void calcMoves();
private:
    static uint64_t movesWhite[64];
    static uint64_t movesBlack[64];
    static uint64_t attackWhite[64];
    static uint64_t attackBlack[64];
};

#endif // PAWN_H
