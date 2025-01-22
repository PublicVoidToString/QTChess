#include "Board.h"
#include "engine.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "king.h"
//pieces

// Getters
bool Board::isWhiteMove() const { return turnNumber%2==0; }

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
unsigned int Board::getTurnNumber() const { return turnNumber; }
void Board::setTurnNumber(unsigned int n) {turnNumber = n; }


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
    lastMove &= ~(0x3F << 26);
    lastMove |= (from & 0x3F) << 26;
}
void Board::setLastMoveTo(char to) {
    lastMove &= ~(0x3F << 20);
    lastMove |= (to & 0x3F) << 20;
}

void Board::setLastMoveEnPassant() {
    lastMove|=0x100; //En Passant true
    lastMove&=0xFFFF87FF; //Captured figure pawn
}
void Board::blockWhiteShortCastle() { lastMove&=0b11111111111111111111111101111111; }
void Board::blockWhiteLongCastle() { lastMove&= 0b11111111111111111111111110111111; }
void Board::blockBlackShortCastle() { lastMove&=0b11111111111111111111111111011111; }
void Board::blockBlackLongCastle() { lastMove&= 0b11111111111111111111111111101111; }

char Board::getLastMoveFrom() const { return static_cast<char>((lastMove >> 26) & 0x3F); }
char Board::getLastMoveTo() const { return static_cast<char>((lastMove >> 20) & 0x3F); }
bool Board::getLastMoveEnPassant() const { return lastMove&0x100; }
bool Board::getWhiteShortCastlePossible() const { return lastMove&0x80; }
bool Board::getWhiteLongCastlePossible() const { return lastMove&0x40; }
bool Board::getBlackShortCastlePossible() const { return lastMove&0x20; }
bool Board::getBlackLongCastlePossible() const { return lastMove&0x10; }

short Board::getLastMovePromotion() const {
    if (lastMove & 0b10000000000000000000) return 1;
    if (lastMove & 0b1000000000000000000)  return 2;
    if (lastMove & 0b100000000000000000)   return 3;
    if (lastMove & 0b10000000000000000)    return 4;
    return 0;
}

void Board::setLastMovePromotion(short promotion) {
    lastMove &= ~(0b11110000000000000000);
    switch (promotion) {
    case 1:
        lastMove |= 0b10000000000000000000;
        break;
    case 2:
        lastMove |= 0b1000000000000000000;
        break;
    case 3:
        lastMove |= 0b100000000000000000;
        break;
    case 4:
        lastMove |= 0b10000000000000000;
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
    lastMove&=0xFFFFFFF3 ;
    white ? lastMove|=0b1000 : lastMove|=0x100;
}

//TODO let's just check if checkmate by calculating it, i'm done
bool Board::getWhiteCheckmate() {
    if(!isWhiteMove()) return false;
    short kingPosition = __builtin_ctzll(this->getWhiteKings());
    if( isAttacked(kingPosition,true)){
        Engine::buildFutureGameTree(this,1);
        if(this->next==nullptr)
            return true;
    }
    return false;
}

bool Board::getBlackCheckmate() {
    if(isWhiteMove()) return false;
    short kingPosition = __builtin_ctzll(this->getBlackKings());
    if( isAttacked(kingPosition,false)) {
        Engine::buildFutureGameTree(this,1);
        if(this->next==nullptr)
            return true;
    }
    return false;
}



void Board::printLastMove() const {
    qWarning() << "From: " << static_cast<int>(getLastMoveFrom())
    << " To: " << static_cast<int>(getLastMoveTo())
    << " Castles: " << getWhiteShortCastlePossible()
    << " " << getWhiteLongCastlePossible()
    << " " << getBlackShortCastlePossible()
    << " " << getBlackLongCastlePossible();
}
