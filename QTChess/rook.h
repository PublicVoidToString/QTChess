#ifndef ROOK_H
#define ROOK_H
#include "board.h"

class Rook
{
public:
    static unsigned long long allMoves(int positionId, const Board& board);
};

#endif // ROOK_H
