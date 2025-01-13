#ifndef KNIGHT_H
#define KNIGHT_H
#include "board.h"

class Knight
{
public:
    static unsigned long long legalMoves(int positionId, const Board& board);
};

#endif // KNIGHT_H
