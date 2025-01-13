#ifndef KING_H
#define KING_H
#include "board.h"

class King
{
public:
    static unsigned long long legalMoves(int positionId, const Board& board);
    static  bool isInCheck(int positionId, const Board& board, bool isWhite);
};

#endif // KING_H
