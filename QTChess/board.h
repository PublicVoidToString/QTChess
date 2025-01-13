#ifndef Board_H
#define Board_H

class Board
{
private:
    unsigned int turnNumber;
    double boardEval; // + White, - Black, Calculated when creating board, making a move, or when Engine is calculating Moves

    bool whiteLongCastlePossible;
    bool whiteShortCastlePossible;
    bool blackLongCastlePossible;
    bool blackShortCastlePossible;
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

    unsigned char lastMove[2];
    void move(unsigned char from, unsigned char to); // Function making moving figure from->to on current board
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

    bool getWhiteLongCastlePossible() const;
    bool getWhiteShortCastlePossible() const;
    bool getBlackLongCastlePossible() const;
    bool getBlackShortCastlePossible() const;
    void blockWhiteShortCastle();
    void blockWhiteLongCastle();
    void blockBlackShortCastle();
    void blockBlackLongCastle();

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
    double getBoardEval() const;
    void setBoardEval(double eval);

    void printRootLength() const;

    void cutSideBranches(); //Recursive delete of all "next"/"right" boards
    void cutRightBranches(); //Recursive delete of all "next"/"left" boards
    void cutLeftBranches(); //Recursive delete of all "next"/"left" boards
    void cutAllBranches(); //Recursive delete of all "next"/"left" boards
};
#endif // Board_H

