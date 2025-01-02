#ifndef PAWN_H
#define PAWN_H
#include "Board.h"

class Pawn
{
public:
    static unsigned long long legalMoves(int positionId, const Board& board);
};

#endif // PAWN_H
