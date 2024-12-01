#ifndef PAWN_H
#define PAWN_H
#import "board.h"

class Pawn
{
public:
    static unsigned long long legalMoves(int positionId, const board& board);
};

#endif // PAWN_H
