#ifndef QUEEN_H
#define QUEEN_H
#include "board.h"

class Queen
{
public:
    static unsigned long long legalMoves(int positionId, const board& board);
};

#endif // QUEEN_H
