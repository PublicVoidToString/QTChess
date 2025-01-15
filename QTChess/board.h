#ifndef Board_H
#define Board_H
#include <stdint.h>

class Board
{
private:
    unsigned int turnNumber;
    double boardEval; // + White, - Black, Calculated when creating board, making a move, or when Engine is calculating Moves

    uint32_t lastMove;
    // Bitboards representing figure positions
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
    bool operator == (const Board &c);
    static unsigned long existingBranches;
    Board();
    Board(Board* previousBoard);
    ~Board();

    void move(unsigned char from, unsigned char to); // Function making moving figure from->to on current board
    void capture(unsigned char to, bool isWhitePiece);
    // GAME TREE (Current is stored by Chessboard class, therefore those pointers are made public)
    Board* prev; //Pointer to previous move (empty if first)
    Board* next; //Pointer to next move (can be also pointing at the first move that needs to be calculated by engine)
    // Engine pointers (If those pointers exists for current board the move has not yet been made)
    Board* right; // Next possible move
    Board* left; // Previous possible move

    // Logic Functions
    bool isOccupied(int buttonId) const; // Is any figure on buttonID tile
    bool isEnemyOccupied(int buttonId) const; // Is enemy on buttonID tile
    bool isEnPassantEligible(int buttonId) const;
    bool isAttacked(int tileId, bool isWhite) const;
    // TODO
    bool isDraw() const;
    bool isBlackMated() const;
    bool isWhiteMated() const;

    // Getters
    bool isWhiteMove() const;
    bool isOnMoveList() const;

    long long getWhitePawns() const;
    long long getWhiteKnights() const;
    long long getWhiteRooks() const;
    long long getWhiteBishops() const;
    long long getWhiteQueens() const;
    long long getWhiteKings() const;

    long long getBlackPawns() const;
    long long getBlackKnights() const;
    long long getBlackRooks() const;
    long long getBlackBishops() const;
    long long getBlackQueens() const;
    long long getBlackKings() const;
    long long getMoves(int position) const;
    double getBoardEval() const;
    void setBoardEval(double eval);
    void setTurnNumber(unsigned int n);
    unsigned int getTurnNumber() const;

    void printRootLength() const;

    void cutSideBranches(); //Recursive delete of all "next"/"right" boards
    void cutRightBranches(); //Recursive delete of all "next"/"left" boards
    void cutLeftBranches(); //Recursive delete of all "next"/"left" boards
    void cutAllBranches(); //Recursive delete of all "next"/"left" boards

    void removeKing(bool isWhite);

    void setLastMoveFrom(char from);
    void setLastMoveTo(char to);
    void setLastMoveShortCastle();
    void setLastMoveLongCastle();
    void setLastMoveEnPassant();
    void blockBlackShortCastle();
    void blockBlackLongCastle();
    void blockWhiteShortCastle();
    void blockWhiteLongCastle();
    char getLastMoveFrom() const;
    char getLastMoveTo() const;
    bool getLastMoveEnPassant() const;
    bool getBlackShortCastlePossible() const;
    bool getBlackLongCastlePossible() const;
    bool getWhiteShortCastlePossible() const;
    bool getWhiteLongCastlePossible() const;
    void printLastMove() const;
};
#endif // Board_H

