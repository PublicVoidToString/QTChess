#ifndef BOARD_H
#define BOARD_H

class board
{
private:
    unsigned long long whitePawns;
    unsigned long long whiteKnights;
    unsigned long long whiteRooks;
    unsigned long long whiteBishops;
    unsigned long long whiteQueens;
    unsigned long long whiteKings;
    unsigned long long blackPawns;
    unsigned long long blackKnights;
    unsigned long long blackRooks;
    unsigned long long blackBishops;
    unsigned long long blackQueens;
    unsigned long long blackKings;
public:
    board();
};

#endif // BOARD_H
