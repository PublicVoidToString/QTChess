#ifndef KING_H
#define KING_H
#include "Board.h"

class King
{
public:
    static unsigned long long legalMoves(int positionId, const Board& board);
};

#endif // KING_H
