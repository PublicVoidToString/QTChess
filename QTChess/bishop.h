#ifndef BISHOP_H
#define BISHOP_H
#include "board.h"

class Bishop
{
public:
    static unsigned long long legalMoves(int positionId, const Board& board);
};

#endif // BISHOP_H
