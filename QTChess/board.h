#ifndef BOARD_H
#define BOARD_H

class board
{
private:

public:
    board();
    bool whiteMove;
    unsigned char selected;
    unsigned char clearSelected;
    unsigned char lastMove[2];
    unsigned long long clearMoves;
    unsigned long long moves;

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

    bool isWhiteMove() const;

    void pressedButton(int buttonId);
    bool isOccupied(int buttonId) const;
    bool isEnemyOccupied(int buttonId) const;
    void move(int from, int to);

    bool isEnPassantEligible(int buttonId) const;
};

#endif // BOARD_H
