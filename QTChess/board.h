#ifndef Board_H
#define Board_H
#include <stdint.h>
#include <QFuture>
#include <QFutureWatcher>

class Board
{
private:
    double boardEval; // + White, - Black, Calculated when creating board, making a move, or when Engine is calculating Moves

    uint64_t lastMove;
    // Bitboards representing figure positions
    uint64_t whitePawns; uint64_t blackPawns;
    uint64_t whiteKnights; uint64_t blackKnights;
    uint64_t whiteRooks; uint64_t blackRooks;
    uint64_t whiteBishops; uint64_t blackBishops;
    uint64_t whiteQueens; uint64_t blackQueens;
    uint64_t whiteKings; uint64_t blackKings;

    uint64_t blackPieces;
    uint64_t whitePieces;
    uint64_t allPieces;


public:
    bool operator == (const Board &c);
    static unsigned long existingBoards;
    Board(bool debug=false);
    Board(Board* previousBoard,bool debug=false);
    Board(Board* previousBoard,unsigned char from, unsigned char to, unsigned short promotion, bool debug=false);
    ~Board();

    // Logic Functions
    bool isOccupied(int tileId) const; // Is any figure on tileId tile
    bool isEnemyOccupied(int tileId) const; // Is enemy on tileId tile
    bool isWhite(int tileId) const;
    bool isBlack(int tileId) const;

    bool isEnPassantEligible(int tileId) const;
    bool isAttacked(int tileId, bool isWhite) const;
    uint64_t* getBitboard(int tileId);


    void makeMove(unsigned char from, unsigned char to, unsigned short promotion=0); // Function making moving figure from->to on current board
    void promote(unsigned char tile, unsigned char promotion);

    void move(unsigned char from, unsigned char to);
    // select the bitboard where move occurs; updates last move
    void movePiece(unsigned char from, unsigned char to, uint64_t& pieceBoard);
    // executes the move; if TO is occupied, then perform capture (calls capture)

    void capture(unsigned char to);
    // select the bitboard where capture occurs; updates last move
    void capturePiece(unsigned char to, uint64_t& pieceBoard);
    // executes the capture (removes piece in the correct bitmap)



    // GAME TREE (Current is stored by Chessboard class, therefore those pointers are made public)
    Board* prev; //Pointer to previous move (empty if first)
    Board* next; //Pointer to next move (can be also pointing at the first move that needs to be calculated by engine)
    // Engine pointers (If those pointers exists for current board the move has not yet been made)
    Board* right; // Next possible move
    Board* left; // Previous possible move



    // Getters
    bool isWhiteMove() const;
    bool isOnMoveList() const;

    short getKingPosition() const;
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

    void printRootLength() const;

    void cutSideBranches(); //Recursive delete of all "next"/"right" boards
    void cutRightBranches(); //Recursive delete of all "next"/"left" boards
    void cutLeftBranches(); //Recursive delete of all "next"/"left" boards
    void cutAllBranches(); //Recursive delete of all "next"/"left" boards
    void cutNextBranches(); //Recursive delete of all "next"/"left" boards

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
    void increaseTurnNumber();
    char getLastMoveFrom() const;
    char getLastMoveTo() const;
    bool getLastMoveEnPassant() const;
    bool getBlackShortCastlePossible() const;
    bool getBlackLongCastlePossible() const;
    bool getWhiteShortCastlePossible() const;
    bool getWhiteLongCastlePossible() const;
    unsigned int getTurnNumber() const;
    unsigned int getGameState() const;

    uint64_t getLastMove() const;

    short getLastMovePromotion() const;
    void setLastMovePromotion(short promotion) ;

    void setLastMoveWin(bool white);
    void setLastMoveWhiteWin();
    void setLastMoveBlackWin();
    bool getWhiteCheckmate();
    bool getBlackCheckmate();
    bool isPossibleMove();


    void printLastMove() const;
};
#endif // Board_H

