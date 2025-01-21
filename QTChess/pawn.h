#ifndef PAWN_H
#define PAWN_H
#include "board.h"

class Pawn
{
public:
    static unsigned long long allMoves(int positionId, const Board& board, bool isWhite);
};

#endif // PAWN_H
