#ifndef KING_H
#define KING_H
#include "board.h"

class King
{
public:
    static unsigned long long legalMoves(int positionId, const Board& board);
    static unsigned long long fastMoves(int positionId, const Board& board);
    static void calcMoves();
private:
    static uint64_t moves[64];
};

#endif // KING_H
