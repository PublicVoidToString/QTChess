#ifndef ROOK_H
#define ROOK_H
#include "board.h"

class Rook
{
public:
    static unsigned long long legalMoves(int positionId, const board& board);
};

#endif // ROOK_H
