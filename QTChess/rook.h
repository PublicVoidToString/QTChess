#ifndef ROOK_H
#define ROOK_H
#include "Board.h"

class Rook
{
public:
    static unsigned long long legalMoves(int positionId, const Board& board);
};

#endif // ROOK_H
