#include "Board.h"
#include "engine.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "king.h"
#include <bitset>
#include <QString>
//pieces

// Getters

uint64_t* Board::getBitboard(int tileId) {
    uint64_t mask = 1ULL << tileId;
    if (whitePawns & mask) return &whitePawns;
    if (whiteRooks & mask) return &whiteRooks;
    if (whiteKnights & mask) return &whiteKnights;
    if (whiteBishops & mask) return &whiteBishops;
    if (whiteQueens & mask) return &whiteQueens;
    if (whiteKings & mask) return &whiteKings;
    if (blackPawns & mask) return &blackPawns;
    if (blackRooks & mask) return &blackRooks;
    if (blackKnights & mask) return &blackKnights;
    if (blackBishops & mask) return &blackBishops;
    if (blackQueens & mask) return &blackQueens;
    if (blackKings & mask) return &blackKings;
    return nullptr;
}

bool Board::isWhiteMove() const { return getTurnNumber()%2==0; }

short Board::getKingPosition() const {
    return  isWhiteMove()
    ? __builtin_ctzll(getWhiteKings())
    : __builtin_ctzll(getBlackKings());
}

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

//TODO Fix
long long Board::getMoves(int position) const {
    long long figureMoves=0;
    if(isWhiteMove()){
        if (getWhitePawns()  & 1ULL <<  position)    figureMoves = Pawn::allMoves(position, *this, isWhiteMove());
        else if (getWhiteRooks()  & 1ULL <<  position)    figureMoves = Rook::allMoves(position, *this);
        else if (getWhiteKnights()  & 1ULL <<  position)  figureMoves = Knight::allMoves(position, *this);
        else if (getWhiteBishops()  & 1ULL <<  position)  figureMoves = Bishop::allMoves(position, *this);
        else if (getWhiteQueens()  & 1ULL <<  position)   figureMoves = Bishop::allMoves(position, *this) | Rook::allMoves(position, *this);
        else if (getWhiteKings()  & 1ULL <<  position)    figureMoves = King::legalMoves(position, *this);
    }
    else {
        if (getBlackPawns() & 1ULL <<  position)    figureMoves = Pawn::allMoves(position, *this, isWhiteMove());
        else if (getBlackRooks() & 1ULL <<  position)    figureMoves = Rook::allMoves(position, *this);
        else if (getBlackKnights() & 1ULL <<  position)  figureMoves = Knight::allMoves(position, *this);
        else if (getBlackBishops() & 1ULL <<  position)  figureMoves = Bishop::allMoves(position, *this);
        else if (getBlackQueens() & 1ULL <<  position)   figureMoves = Bishop::allMoves(position, *this) | Rook::allMoves(position, *this);
        else if (getBlackKings() & 1ULL <<  position)    figureMoves = King::legalMoves(position, *this);
    }
    return figureMoves;
}

double Board::getBoardEval() const { return boardEval; }
void Board::setBoardEval(double eval) { boardEval=eval; }
unsigned int Board::getTurnNumber() const { return lastMove&0x3FFFFF; }
void Board::increaseTurnNumber() { lastMove++; }


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
        right=nullptr;
    }
    if(left!=nullptr) {
        left->cutLeftBranches();
        delete left;
        left=nullptr;
    }
}

void Board::cutRightBranches() {
    if(right!=nullptr) {
        right->cutRightBranches();
        delete right;
        right=nullptr;
    }
    if(next!=nullptr) {
        next->cutAllBranches();
        delete next;
        next=nullptr;
    }
}

void Board::cutLeftBranches() {
    if(left!=nullptr) {
        left->cutLeftBranches();
        delete left;
        left=nullptr;
    }
    if(next!=nullptr) {
        next->cutAllBranches();
        delete next;
        next=nullptr;
    }
}

void Board::cutAllBranches() {
    if(right!=nullptr) {
        right->cutRightBranches();
        delete right;
        right=nullptr;
    }
    if(left!=nullptr) {
        left->cutLeftBranches();
        delete left;
        left=nullptr;
    }
    if(next!=nullptr) {
        next->cutAllBranches();
        delete next;
        next=nullptr;
    }
}

void Board::cutNextBranches(){
    if(next!=nullptr) {
        next->cutAllBranches();
        delete next;
        next=nullptr;
    }
}

void Board::removeKing(bool isWhite){
    if(isWhite) whiteKings=0;
    else blackKings=0;
}

void Board::setLastMoveFrom(char from) {
    lastMove &= ~(0xFC00000000000000ULL);
    lastMove |= (static_cast<uint64_t>(from & 0x3F) << 58);
}

void Board::setLastMoveTo(char to) {
    lastMove &= ~(0x03F0000000000000ULL);
    lastMove |= (static_cast<uint64_t>(to & 0x3F) << 52);
}

void Board::setLastMoveEnPassant() {
    lastMove|=0x0000001000000000;
    lastMove&=0xFFFF0FFFFFFFFFFF; // en passant captures a pawn
}
void Board::blockWhiteShortCastle() { lastMove&=0b1111111111111111111111111111011111111111111111111111111111111111; }
void Board::blockWhiteLongCastle() { lastMove&= 0b1111111111111111111111111111101111111111111111111111111111111111; }
void Board::blockBlackShortCastle() { lastMove&=0b1111111111111111111111111111110111111111111111111111111111111111; }
void Board::blockBlackLongCastle() { lastMove&= 0b1111111111111111111111111111111011111111111111111111111111111111; }

char Board::getLastMoveFrom() const { return static_cast<char>((lastMove >> 58) & 0x3F); }
char Board::getLastMoveTo() const { return static_cast<char>((lastMove >> 52) & 0x3F); }
bool Board::getLastMoveEnPassant() const { return lastMove&0xFFFF0FFFFFFFFFFF; }
bool Board::getWhiteShortCastlePossible() const { return lastMove&0b100000000000000000000000000000000000; }
bool Board::getWhiteLongCastlePossible() const { return lastMove&0b10000000000000000000000000000000000; }
bool Board::getBlackShortCastlePossible() const { return lastMove&0b1000000000000000000000000000000000; }
bool Board::getBlackLongCastlePossible() const { return lastMove&0b100000000000000000000000000000000; }
unsigned int Board::getGameState() const { return (lastMove&0b0000000000000000000000000000000011000000000000000000000000000000) >> 30; }

short Board::getLastMovePromotion() const {
    if (lastMove & 0x0008000000000000) return 1;
    if (lastMove & 0x0006000000000000)  return 2;
    if (lastMove & 0x0004000000000000)   return 3;
    if (lastMove & 0x0002000000000000)    return 4;
    return 0;
}

void Board::setLastMovePromotion(short promotion) {
    lastMove &= ~(0x000F000000000000);
    switch (promotion) {
    case 1:
        lastMove |= 0x0008000000000000;
        break;
    case 2:
        lastMove |= 0x0006000000000000;
        break;
    case 3:
        lastMove |= 0x0004000000000000;
        break;
    case 4:
        lastMove |= 0x0002000000000000;
        break;
    case 0:
        break;
    default:
        QMessageBox::critical(nullptr, "Error", "Wrong promotion ID passed to setLastMove!! (ERROR EBGS01)");
        QCoreApplication::quit();
        break;
    }
}

void Board::setLastMoveWin(bool white) {
    lastMove&=0xFFFFFFFF3FFFFFFF ;
    white ? lastMove|=0x80000000 : lastMove|=0x40000000;
}

//TODO let's just check if checkmate by calculating it, i'm done
bool Board::getWhiteCheckmate() {
    if(!isWhiteMove()) return false;
    short kingPosition = __builtin_ctzll(this->getWhiteKings());
    if(isAttacked(kingPosition,true)){
        if(!Engine::hasLegalMoves(this)) {
            lastMove|=0x40000000;
            return true;
        }
    }
    return false;
}

bool Board::getBlackCheckmate() {
    if(isWhiteMove()) return false;
    short kingPosition = __builtin_ctzll(this->getBlackKings());
    if( isAttacked(kingPosition,false)) {
        if(!Engine::hasLegalMoves(this)){
            lastMove|=0x80000000;
            return true;
        }
    }
    return false;
}

bool Board::isPossibleMove() {
    if(!Engine::hasLegalMoves(this)){
        lastMove|=0xC0000000;
        return false;
    }
    return true;
}

uint64_t Board::getLastMove() const {
    return lastMove;
}

void Board::printLastMove() const {
    qWarning() << "From: " << static_cast<int>(getLastMoveFrom())
    << " To: " << static_cast<int>(getLastMoveTo())
    << " Castles: " << getWhiteShortCastlePossible()
    << " " << getWhiteLongCastlePossible()
    << " " << getBlackShortCastlePossible()
    << " " << getBlackLongCastlePossible();
}
