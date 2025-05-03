#ifndef KNIGHT_H
#define KNIGHT_H
#include "board.h"

class Knight
{
public:
    static unsigned long long allMoves(int positionId, const Board& board);
};

#endif // KNIGHT_H
