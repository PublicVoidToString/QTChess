#include "Board.h"
#include "engine.h"
#include <QString>

uint64_t Board::getLastMove() const {
    return lastMove;
}

void Board::printLastMove() const {
    qWarning() << "From: " << static_cast<int>(getLastMoveFrom())
    << " To: " << static_cast<int>(getLastMoveTo())
    << " Castles: " << getWhiteShortCastlePossible()
    << " " << getWhiteLongCastlePossible()
    << " " << getBlackShortCastlePossible()
    << " " << getBlackLongCastlePossible()
    << " Rules:" << (getFiftyRuleNumber()+1)/2 << "/50"
    << " " << getThreeRuleNumber();
}


// Moved From (6/6)
void Board::setLastMoveFrom(char from) {
    lastMove &= ~(0xFC00000000000000ULL);
    lastMove |= (static_cast<uint64_t>(from & 0x3F) << 58);
}
char Board::getLastMoveFrom() const { return static_cast<char>((lastMove >> 58) & 0x3F); }

// Moved To (6/12)
void Board::setLastMoveTo(char to) {
    lastMove &= ~(0x03F0000000000000ULL);
    lastMove |= (static_cast<uint64_t>(to & 0x3F) << 52);
}
char Board::getLastMoveTo() const { return static_cast<char>((lastMove >> 52) & 0x3F); }


// Promotion (4/16)
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
        QMessageBox::critical(nullptr, "Error", "Wrong promotion ID passed to setLastMove!! (ERROR LM01)");
        QCoreApplication::quit();
        break;
    }
}

// Capture (5/21)


// Moving piece (6/27)


// En Passant (1/28)
void Board::setLastMoveEnPassant() {
    lastMove|=0x0000001000000000;
    lastMove&=0xFFFF0FFFFFFFFFFF; // en passant captures a pawn
}
bool Board::getLastMoveEnPassant() const { return lastMove&0xFFFF0FFFFFFFFFFF; }

// Castle (4/32)
void Board::blockWhiteShortCastle() { lastMove&=0b1111111111111111111111111111011111111111111111111111111111111111; }
void Board::blockWhiteLongCastle() { lastMove&= 0b1111111111111111111111111111101111111111111111111111111111111111; }
void Board::blockBlackShortCastle() { lastMove&=0b1111111111111111111111111111110111111111111111111111111111111111; }
void Board::blockBlackLongCastle() { lastMove&= 0b1111111111111111111111111111111011111111111111111111111111111111; }

bool Board::getWhiteShortCastlePossible() const { return lastMove&0b100000000000000000000000000000000000; }
bool Board::getWhiteLongCastlePossible() const { return lastMove&0b10000000000000000000000000000000000; }
bool Board::getBlackShortCastlePossible() const { return lastMove&0b1000000000000000000000000000000000; }
bool Board::getBlackLongCastlePossible() const { return lastMove&0b100000000000000000000000000000000; }


// Win/Draw Logic (2/34)
void Board::setLastMoveWin(bool white) {
    lastMove&=0xFFFFFFFF3FFFFFFF ;
    white ? lastMove|=0x80000000 : lastMove|=0x40000000;
}
unsigned int Board::getGameState() const { return (lastMove&0b11000000000000000000000000000000) >> 30; }


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

// 50 Repetition Rule (7/41)

void Board::resetFiftyRule(){
    lastMove &= ~(0x3F800000);
}

bool Board::checkFiftyRule(bool checkMate){
    lastMove += 0x800000;
    if ((lastMove & 0x30000000) == 0x30000000 && (lastMove & 0x0E000000)) {
        if (checkMate) lastMove |= 0xC0000000;
        return true;
    }
    return false;
}

unsigned int Board::getFiftyRuleNumber() const{
    return lastMove>>23 & 0b1111111;
}

// 3 repeating position counter (1/42)
bool Board::checkThreeRule(){
    for(Board* temp=this->prev;temp!=nullptr;temp=temp->prev){
        if(temp->whitePawns==this->whitePieces
            && temp->whiteRooks==this->whiteRooks
            && temp->whiteKnights==this->whiteKnights
            && temp->whiteBishops==this->whiteBishops
            && temp->whiteQueens==this->whiteQueens
            && temp->whiteKings==this->whiteKings
            && temp->blackPawns==this->blackPawns
            && temp->blackRooks==this->blackRooks
            && temp->blackKnights==this->blackKnights
            && temp->blackBishops==this->blackBishops
            && temp->blackQueens==this->blackQueens
            && temp->blackKings==this->blackKings){
                if(lastMove & 0x400000) {
                    lastMove |= 0xC0000000;
                    return true;
                }
                else {
                    lastMove |= 0x400000;
                    return false;
                }
        } else
        if(!(temp->lastMove & 0x3F000000)) return false;
    }
    return false;
}

unsigned int Board::getThreeRuleNumber() const{
    return lastMove>>22 & 1;
}

// Move counter (22/64)
unsigned int Board::getTurnNumber() const { return lastMove&0x3FFFFF; }
void Board::increaseTurnNumber() { lastMove++; }
