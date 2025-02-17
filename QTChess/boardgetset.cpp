#include "Board.h"
#include "engine.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "king.h"
#include <bitset>
#include <QString>
#include <QMessageBox>

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
int Board::getOrderingScore() const { return orderingScore; }

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


short Board::getCapturedPieceScore() const {
    if (lastMove & 0x0000800000000000) return 40; // queen - most valuable victim
    if (lastMove & 0x0000400000000000) return 30; // rook
    if (lastMove & 0x0000300000000000) return 20; // bishop or knight - same order
    if (lastMove & 0x0000080000000000) return 10; // pawn
    return 0;
}

short Board::getMovingPieceScore() const {
    if (lastMove & 0x0000040000000000) return 5; // pawn - least valuable attacker
    if (lastMove & 0x0000030000000000) return 4; // bishop or knight
    if (lastMove & 0x0000008000000000) return 3; // rook
    if (lastMove & 0x0000004000000000) return 2; // queen
    if (lastMove & 0x0000002000000000) return 1; // king
    return 0;
}
