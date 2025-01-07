#ifndef PAWN_H
#define PAWN_H
#include "Board.h"

class Pawn
{
public:
    static unsigned long long legalMoves(int positionId, const Board& board);
    static unsigned long long legalMoves(int positionId, const Board& board, bool isWhite);
};

#endif // PAWN_H
