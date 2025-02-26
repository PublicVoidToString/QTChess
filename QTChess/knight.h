#ifndef KNIGHT_H
#define KNIGHT_H
#include "board.h"

class Knight
{
public:
    static unsigned long long allMoves(int positionId, const Board& board);
    static unsigned long long fastMoves(int positionId, const Board& board);
    static void calcMoves();
private:
    static uint64_t moves[64];
};

#endif // KNIGHT_H
