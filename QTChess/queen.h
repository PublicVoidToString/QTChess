#ifndef QUEEN_H
#define QUEEN_H
#import "board.h"

class Queen
{
public:
    static unsigned long long legalMoves(int positionId, const board& board);
};

#endif // QUEEN_H
