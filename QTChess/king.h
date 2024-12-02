#ifndef KING_H
#define KING_H
#include "board.h"

class King
{
public:
    static unsigned long long legalMoves(int positionId, const board& board);
};

#endif // KING_H
