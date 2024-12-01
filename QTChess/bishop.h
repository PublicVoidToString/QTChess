#ifndef BISHOP_H
#define BISHOP_H
#import "board.h"

class Bishop
{
public:
    static unsigned long long legalMoves(int positionId, const board& board);
};

#endif // BISHOP_H
