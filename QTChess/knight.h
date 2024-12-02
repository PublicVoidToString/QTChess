#ifndef KNIGHT_H
#define KNIGHT_H
#include "board.h"

class Knight
{
public:
    static unsigned long long legalMoves(int positionId, const board& board);
};

#endif // KNIGHT_H
