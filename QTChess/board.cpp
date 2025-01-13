#include "Board.h"
//pieces
#include <cstddef>
#include <QMessageBox>
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "king.h"

// PRIVATE:

// Functions
// PUBLIC:

unsigned long Board::existingBranches=0;

bool Board::operator == (const Board &b)
{
    if(whitePawns != b.whitePawns) return false;
    if(whiteRooks != b.whiteRooks) return false;
    if(whiteKnights != b.whiteKnights) return false;
    if(whiteBishops != b.whiteBishops) return false;
    if(whiteQueens != b.whiteQueens) return false;
    if(whiteKings != b.whiteKings) return false;

    if(blackPawns != b.blackPawns) return false;
    if(blackRooks != b.blackRooks) return false;
    if(blackKnights != b.blackKnights) return false;
    if(blackBishops != b.blackBishops) return false;
    if(blackQueens != b.blackQueens) return false;
    if(blackKings != b.blackKings) return false;
    return true;
}

Board::Board() {
    existingBranches+=1;
    turnNumber = 0;
    boardEval=0;
    lastMove[0] = 64; // initiated out of the chessboard
    lastMove[1] = 64;

    whiteLongCastlePossible = true;
    whiteShortCastlePossible = true;
    blackLongCastlePossible = true;
    blackShortCastlePossible = true;

    whitePawns   = 0b0000000000000000000000000000000000000000000000001111111100000000;
    whiteRooks   = 0b0000000000000000000000000000000000000000000000000000000010000001;
    whiteKnights = 0b0000000000000000000000000000000000000000000000000000000001000010;
    whiteBishops = 0b0000000000000000000000000000000000000000000000000000000000100100;
    whiteQueens  = 0b0000000000000000000000000000000000000000000000000000000000001000;
    whiteKings   = 0b0000000000000000000000000000000000000000000000000000000000010000;

    blackPawns   = 0b0000000011111111000000000000000000000000000000000000000000000000;
    blackRooks   = 0b1000000100000000000000000000000000000000000000000000000000000000;
    blackKnights = 0b0100001000000000000000000000000000000000000000000000000000000000;
    blackBishops = 0b0010010000000000000000000000000000000000000000000000000000000000;
    blackQueens  = 0b0000100000000000000000000000000000000000000000000000000000000000;
    blackKings   = 0b0001000000000000000000000000000000000000000000000000000000000000;

    prev = NULL;
    right = NULL;
    left = NULL;
    next = NULL;
}
Board::Board(Board* previousBoard) {
    existingBranches+=1;
    turnNumber = previousBoard->turnNumber+1;

    whiteShortCastlePossible = previousBoard->whiteShortCastlePossible;
    whiteLongCastlePossible = previousBoard->whiteLongCastlePossible;
    blackShortCastlePossible = previousBoard->blackShortCastlePossible;
    blackLongCastlePossible = previousBoard->blackLongCastlePossible;

    whitePawns   = previousBoard->whitePawns;
    whiteRooks   = previousBoard->whiteRooks;
    whiteKnights = previousBoard->whiteKnights;
    whiteBishops = previousBoard->whiteBishops;
    whiteQueens  = previousBoard->whiteQueens;
    whiteKings   = previousBoard->whiteKings;

    blackPawns   = previousBoard->blackPawns;
    blackRooks   = previousBoard->blackRooks;
    blackKnights = previousBoard->blackKnights;
    blackBishops = previousBoard->blackBishops;
    blackQueens  = previousBoard->blackQueens;
    blackKings   = previousBoard->blackKings;



    prev = previousBoard;
    right = NULL;
    left = NULL;
    next = NULL;
}

void Board::move(unsigned char from, unsigned char to) // Function making moving figure from->to on current board
{
    bool isWhitePiece = false;
    bool isEnPassant = false;
    unsigned char capturedPawnPosition = -1;

    if (whitePawns & (1ULL << from)) {
        whitePawns &= ~(1ULL << from);
        isWhitePiece = true;
        if ((to - from) == 9 || (to - from) == 7) {
            // Stay focused here;
            // isEnemyOccupied when searching for move on the current board - correct color;
            // when executing a move - done on a new board (uses the wrong color
            if (!isOccupied(to)) {
                isEnPassant = true;
                capturedPawnPosition = to - 8;
            }
        }
        whitePawns |= (1ULL << to);
    } else if (whiteRooks & (1ULL << from)) {

        if (from == 0) {
            blockWhiteLongCastle();
        } else if (from == 7) {
            blockWhiteShortCastle();
        }
        whiteRooks &= ~(1ULL << from);
        whiteRooks |= (1ULL << to);
        isWhitePiece = true;
    } else if (whiteKnights & (1ULL << from)) {
        whiteKnights &= ~(1ULL << from);
        whiteKnights |= (1ULL << to);
        isWhitePiece = true;
    } else if (whiteBishops & (1ULL << from)) {
        whiteBishops &= ~(1ULL << from);
        whiteBishops |= (1ULL << to);
        isWhitePiece = true;
    } else if (whiteQueens & (1ULL << from)) {
        whiteQueens &= ~(1ULL << from);
        whiteQueens |= (1ULL << to);
        isWhitePiece = true;
    } else if (whiteKings & (1ULL << from)) {
        whiteKings &= ~(1ULL << from);
        whiteKings |= (1ULL << to);
        // Castles
        if (from == 4 && to == 6 && getWhiteShortCastlePossible()) {
            whiteRooks &= ~(1ULL << 7);
            whiteRooks |= (1ULL << 5);
        } else if (from == 4 && to == 2 && getWhiteLongCastlePossible()) {
            whiteRooks &= ~(1ULL << 0);
            whiteRooks |= (1ULL << 3);
        }
        // Updating castling legality after moving
        blockWhiteLongCastle();
        blockWhiteShortCastle();
        isWhitePiece = true;
    } else if (blackPawns & (1ULL << from)) {
        blackPawns &= ~(1ULL << from);
        if ((from - to) == 9 || (from - to) == 7) {
            if (!isOccupied(to)) {
                isEnPassant = true;
                capturedPawnPosition = to + 8;
            }
        }
        blackPawns |= (1ULL << to);
    } else if (blackRooks & (1ULL << from)) {

        if (from == 56) {
            blockBlackLongCastle();
        } else if (from == 63) {
            blockBlackShortCastle();
        }

        blackRooks &= ~(1ULL << from);
        blackRooks |= (1ULL << to);
    } else if (blackKnights & (1ULL << from)) {
        blackKnights &= ~(1ULL << from);
        blackKnights |= (1ULL << to);
    } else if (blackBishops & (1ULL << from)) {
        blackBishops &= ~(1ULL << from);
        blackBishops |= (1ULL << to);
    } else if (blackQueens & (1ULL << from)) {
        blackQueens &= ~(1ULL << from);
        blackQueens |= (1ULL << to);
    } else if (blackKings & (1ULL << from)) {
        blackKings &= ~(1ULL << from);
        blackKings |= (1ULL << to);
        if (from == 60 && to == 62 && getBlackShortCastlePossible()) {
            blackRooks &= ~(1ULL << 63);
            blackRooks |= (1ULL << 61);
        } else if (from == 60 && to == 58 && getBlackLongCastlePossible()) {
            blackRooks &= ~(1ULL << 56);
            blackRooks |= (1ULL << 59);
        }
        blockBlackShortCastle();
        blockBlackLongCastle();
    }
    if (isEnPassant) {
        if (isWhitePiece) {
            blackPawns &= ~(1ULL << capturedPawnPosition);
        } else {
            whitePawns &= ~(1ULL << capturedPawnPosition);
        }
    } else {
        if (isWhitePiece) {
            blackPawns &= ~(1ULL << to);
            blackRooks &= ~(1ULL << to);
            blackKnights &= ~(1ULL << to);
            blackBishops &= ~(1ULL << to);
            blackQueens &= ~(1ULL << to);
            blackKings &= ~(1ULL << to);
        } else {
            whitePawns &= ~(1ULL << to);
            whiteRooks &= ~(1ULL << to);
            whiteKnights &= ~(1ULL << to);
            whiteBishops &= ~(1ULL << to);
            whiteQueens &= ~(1ULL << to);
            whiteKings &= ~(1ULL << to);
        }
    }


    lastMove[0] = from;
    lastMove[1] = to;

}


// Logic Functions
bool Board::isOccupied(int buttonId) const { // Is any figure on buttonID tile
    return (whitePawns | whiteKnights | whiteRooks | whiteBishops | whiteQueens | whiteKings |
            blackPawns | blackKnights | blackRooks | blackBishops | blackQueens | blackKings) & (1ULL << buttonId);
}
bool Board::isEnemyOccupied(int buttonId) const { // Is enemy on buttonID tile
    if(isWhiteMove()) {
        return (blackPawns | blackKnights | blackRooks | blackBishops | blackQueens | blackKings) & (1ULL << buttonId);
    } else {
        return (whitePawns | whiteKnights | whiteRooks | whiteBishops | whiteQueens | whiteKings) & (1ULL << buttonId);
    }
}
bool Board::isEnPassantEligible(int buttonId) const {

    // checks if last move was a 2 forward advance

    if(Board::isWhiteMove()) {
        return (blackPawns & (1ULL << buttonId)) && this->prev->lastMove[0] == buttonId+16 && this->prev->lastMove[1] == buttonId;
    } else {
        return (whitePawns & (1ULL << buttonId)) && this->prev->lastMove[0] == buttonId-16 && this->prev->lastMove[1] == buttonId;
    }
}

bool Board::isAttacked(int tileId, bool isWhite) const {
    long long moves = 0;
    for (short from = 0; from < 64; from++) {
        if (isWhite) {
            if (getBlackPawns() >> from & 1) moves |= Pawn::legalMoves(from, *this, false);
            else if (getBlackRooks() >> from & 1) moves |= Rook::legalMoves(from, *this);
            else if (getBlackKnights() >> from & 1) moves |= Knight::legalMoves(from, *this);
            else if (getBlackBishops() >> from & 1) moves |= Bishop::legalMoves(from, *this);
            else if (getBlackQueens() >> from & 1) moves |= Rook::legalMoves(from, *this) | Bishop::legalMoves(from, *this);
            else if (getBlackKings() >> from & 1) moves |= King::legalMoves(from, *this);
        }
        else {
            if (getWhitePawns() >> from & 1) moves |= Pawn::legalMoves(from, *this, true);
            else if (getWhiteRooks() >> from & 1) moves |= Rook::legalMoves(from, *this);
            else if (getWhiteKnights() >> from & 1) moves |= Knight::legalMoves(from, *this);
            else if (getWhiteBishops() >> from & 1) moves |= Bishop::legalMoves(from, *this);
            else if (getWhiteQueens() >> from & 1) moves |= Rook::legalMoves(from, *this) | Bishop::legalMoves(from, *this);
            else if (getWhiteKings() >> from & 1) moves |= King::legalMoves(from, *this);
        }
    }
    if (((tileId+1) & moves) != 0) return true;
    return false;
}

//TODO
bool Board::isDraw() const {
    return false;
}
bool Board::isBlackMated() const {
    return false;
}
bool Board::isWhiteMated() const {
    return false;
}



// Getters
bool Board::isWhiteMove() const { return turnNumber%2==0; }

bool Board::getWhiteLongCastlePossible() const { return whiteLongCastlePossible; }
bool Board::getWhiteShortCastlePossible() const { return whiteShortCastlePossible; }
bool Board::getBlackLongCastlePossible() const { return blackLongCastlePossible; }
bool Board::getBlackShortCastlePossible() const { return blackShortCastlePossible; }
void Board::blockWhiteLongCastle() { whiteLongCastlePossible=false; }
void Board::blockWhiteShortCastle() { whiteLongCastlePossible=false; }
void Board::blockBlackLongCastle() { blackLongCastlePossible=false; }
void Board::blockBlackShortCastle() { blackShortCastlePossible=false;}

long long Board::getWhitePawns() const { return whitePawns; }
long long Board::getWhiteKnights() const { return whiteKnights; }
long long Board::getWhiteRooks() const { return whiteRooks; }
long long Board::getWhiteBishops() const { return whiteBishops; }
long long Board::getWhiteQueens() const { return whiteQueens; }
long long Board::getWhiteKings() const { return whiteKings; }

long long Board::getBlackPawns() const { return blackPawns; }
long long Board::getBlackKnights() const { return blackKnights; }
long long Board::getBlackRooks() const { return blackRooks; }
long long Board::getBlackBishops() const { return blackBishops; }
long long Board::getBlackQueens() const { return blackQueens; }
long long Board::getBlackKings() const { return blackKings; }

double Board::getBoardEval() const { return boardEval; }
void Board::setBoardEval(double eval) { boardEval=eval; }

void Board::printRootLength() const {
    int size = 0;
    for (const Board* cur = this; cur->prev != nullptr; cur = cur->prev) {
        size++;
    }
    qWarning() << "Root size:" << size;
}

void Board::cutSideBranches() {
    if(right!=nullptr) {
        right->cutRightBranches();
        delete right;
    }
    if(left!=nullptr) {
        left->cutLeftBranches();
        delete left;
    }
}

void Board::cutRightBranches() {
    if(right!=nullptr) {
        right->cutRightBranches();
        delete right;
    }
    if(next!=nullptr) {
        next->cutAllBranches();
        delete next;
    }
}

void Board::cutLeftBranches() {
    if(left!=nullptr) {
        left->cutLeftBranches();
        delete left;
    }
    if(next!=nullptr) {
        next->cutAllBranches();
        delete next;
    }
}

void Board::cutAllBranches() {
    if(right!=nullptr) {
        right->cutRightBranches();
        delete right;
    }
    if(left!=nullptr) {
        left->cutLeftBranches();
        delete left;
    }
    if(next!=nullptr) {
        next->cutAllBranches();
        delete next;
    }
}

Board::~Board() {
    existingBranches-=1;
}

