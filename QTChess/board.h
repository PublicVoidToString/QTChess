#ifndef Board_H
#define Board_H

class Board
{
private:

public:
    Board();
    Board(Board* previousBoard);
    bool whiteMove;
    unsigned char selected;
    unsigned char clearSelected;
    unsigned char lastMove[2];
    unsigned long long clearMoves;
    unsigned long long moves;

    bool whiteLongCastlePossible;
    bool whiteShortCastlePossible;
    bool blackLongCastlePossible;
    bool blackShortCastlePossible;

    bool getWhiteLongCastlePossible() const;
    bool getWhiteShortCastlePossible() const;
    bool getBlackLongCastlePossible() const;
    bool getBlackShortCastlePossible() const;

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

    Board* prev;
    Board* left;
    Board* right;
    Board* next;

    bool isWhiteMove() const;

    // TODO
    bool isDraw() const;
    bool isBlackMated() const;
    bool isWhiteMated() const;

    void pressedButton(int buttonId);
    bool isOccupied(int buttonId) const;
    bool isEnemyOccupied(int buttonId) const;
    void nextMove(int from, int to);
    void move(int from, int to);

    unsigned char sumBits(unsigned long long variable) const;
    long long sumWhiteMaterial() const;
    long long sumBlackMaterial() const;

    long long evaluatePosition() const;

    bool isEnPassantEligible(int buttonId) const;
};

#endif // Board_H
