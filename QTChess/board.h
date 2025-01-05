#ifndef Board_H
#define Board_H

class Board
{
private:
    bool whiteMove;

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

    // Other variables
    unsigned char selected; //Selected tile on the board remembered after clicked
    unsigned char clearSelected; //Previous selected tile to be cleared in next move
    unsigned long long moves; //Moves bitboard of all possible moves of figure on selected tile
    unsigned long long clearMoves; //Previous Moves bitboard to be cleared in next move
    unsigned char lastMove[2];

    // Functions
    void move(int from, int to); // Function making moving figure from->to on current board

    // Evaluation functions
    unsigned char sumBits(unsigned long long variable) const;
    long long sumWhiteMaterial() const;
    long long sumBlackMaterial() const;

public:
    Board();
    Board(Board* previousBoard);

    // GAME TREE (Current is stored by Chessboard class, therefore those pointers are made public)
    Board* prev; //Pointer to previous move (empty if first)
    Board* next; //Pointer to next move (can be also pointing at the first move that needs to be calculated by engine)
    // Engine pointers (If those pointers exists for current board the move has not yet been made)
    Board* right; // Next possible move
    Board* left; // Previous possible move

    // Main Functions
    bool pressedButton(int buttonId); // Reads input and calls all other functions
    long long evaluatePosition() const; // Main eval function, calculating based on private eval functions

    // Logic Functions
    bool isOccupied(int buttonId) const; // Is any figure on buttonID tile
    bool isEnemyOccupied(int buttonId) const; // Is enemy on buttonID tile
    bool isEnPassantEligible(int buttonId) const;
    // TODO
    bool isDraw() const;
    bool isBlackMated() const;
    bool isWhiteMated() const;

    // Engine Functions
    void nextMove(int from, int to); // Function creating new Board instance on next and perfoming move on it

    // Getters
    bool isWhiteMove() const;

    bool getWhiteLongCastlePossible() const;
    bool getWhiteShortCastlePossible() const;
    bool getBlackLongCastlePossible() const;
    bool getBlackShortCastlePossible() const;

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

    char getSelected() const;
    char getClearSelected() const;
    long long getMoves() const;
    long long getClearMoves() const;

};

#endif // Board_H
