#include "Board.h"
//pieces
#include <cstddef>
#include <QMessageBox>
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "king.h"
#include "engine.h"

// PRIVATE:

// Functions
// PUBLIC:

unsigned long Board::existingBoards=0;

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

Board::Board(bool debug) {
    if(!debug) existingBoards+=1;
    boardEval=0;
    lastMove = 0xF00000000; //castles are possible

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

    whitePieces = (whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKings);
    blackPieces = (blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKings);
    allPieces = (whitePieces | blackPieces);
    prev = nullptr;
    right = nullptr;
    left = nullptr;
    next = nullptr;
}

Board::Board(Board* previousBoard,unsigned char from, unsigned char to, unsigned short promotion, bool debug) {
    if(!debug) existingBoards+=1;
    lastMove=previousBoard->lastMove;
    increaseTurnNumber();

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

    makeMove(from, to, promotion);

    whitePieces = (whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKings);
    blackPieces = (blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKings);
    allPieces = (whitePieces | blackPieces);

    prev = previousBoard;
    right = nullptr;
    left = nullptr;
    next = nullptr;
}

void Board::makeMove(unsigned char from, unsigned char to, unsigned short promotion) // Making move on board without checking it's legality
{
    setLastMovePromotion(promotion);
    setLastMoveFrom(from);
    setLastMoveTo(to);

    uint64_t* bitboard = getBitboard(to);
    if(bitboard!=nullptr) *bitboard &= ~(1ULL<<to);
    bitboard = getBitboard(from);
    if (bitboard == nullptr) {
        qWarning() << "Invalid move: no bitboard found for tile: " << to << " (ERROR BK01)";
        return;
    }
    *bitboard ^= ((1ULL<<from)|(1ULL<<to));
    if((*bitboard)==whitePawns){
        if(promotion > 0) promote(to, promotion);
        if((to-from==9 || to-from==7) && to<=47 && to>=40 && !isOccupied(to))  {
            setLastMoveEnPassant();
            blackPawns &= ~(1ULL<<(to-8));
        }
    }else if((*bitboard)==blackPawns){
        if(promotion > 0) promote(to, promotion);
        if((from-to==9 || from-to==7) && to<=23 && to>=16 && !isOccupied(to))  {
            whitePawns &= ~(1ULL<<(to+8));
        }
    }
    else if((*bitboard)==whiteKings){
        if (from == 4 && to == 6) {
            whiteRooks &= ~(1ULL << 7);
            whiteRooks |= (1ULL << 5);
        } else if (from == 4 && to == 2) {
            whiteRooks &= ~(1ULL << 0);
            whiteRooks |= (1ULL << 3);
        }
        blockWhiteLongCastle();
        blockWhiteShortCastle();

    }else if((*bitboard)==blackKings){
        blackKings &= ~(1ULL << from);
        blackKings |= (1ULL << to);
        if (from == 60 && to == 62) {
            blackRooks &= ~(1ULL << 63);
            blackRooks |= (1ULL << 61);
        } else if (from == 60 && to == 58) {
            blackRooks &= ~(1ULL << 56);
            blackRooks |= (1ULL << 59);
        }
        blockBlackShortCastle();
        blockBlackLongCastle();
    }else if((*bitboard)==whiteRooks){
        if (from == 0) {
            blockWhiteLongCastle();
        } else if (from == 7) {
            blockWhiteShortCastle();
        }
    }else if((*bitboard)==blackRooks) {
        if (from == 56) {
            blockBlackLongCastle();
        } else if (from == 63) {
            blockBlackShortCastle();
        }
    }
}

void Board::promote(unsigned char tile, unsigned char promotion) {
    uint64_t tileMask = 1ULL << tile;

    bool isWhite = (getWhitePawns() & tileMask) != 0;
    bool isBlack = (getBlackPawns() & tileMask) != 0;

    if (!isWhite && !isBlack) {
        QMessageBox::critical(nullptr, "Error", "Wrong tile passed to promote!! (ERROR EB01)");
        QCoreApplication::quit();
        return;
    }

    if (isWhite) whitePawns &= ~tileMask;
    else blackPawns &= ~tileMask;

    uint64_t* promotionTarget = nullptr;
    switch (promotion) {
    case 1: promotionTarget = isWhite ? &whiteQueens : &blackQueens; break;
    case 2: promotionTarget = isWhite ? &whiteRooks : &blackRooks; break;
    case 3: promotionTarget = isWhite ? &whiteBishops : &blackBishops; break;
    case 4: promotionTarget = isWhite ? &whiteKnights : &blackKnights; break;
    default:
        QMessageBox::critical(nullptr, "Error", "Wrong promotion ID passed to promote!! (ERROR EB02)");
        QCoreApplication::quit();
        return;
    }

    *promotionTarget |= tileMask;
}

void Board::move(unsigned char from, unsigned char to) {

    // clearing last move - both move and capture
    lastMove&=0xFFFF000FFFFFFFFF;

    // this means capture occurs
    if(isOccupied(to)) {
        capture(to);
    }

    if (isWhiteMove()) {
        if (blackPawns & (1ULL << from)) {
            movePiece(from, to, blackPawns);
            lastMove|=0x0000040000000000;
        }
        else if (blackKnights & (1ULL << from)) {
            movePiece(from, to, blackKnights);
            lastMove|=0x0000020000000000;
        }
        else if (blackBishops & (1ULL << to)) {
            movePiece(from, to, blackBishops);
            lastMove|=0x0000010000000000;
        }
        else if (blackRooks & (1ULL << to)) {
            movePiece(from, to, blackRooks);
            lastMove|=0x0000008000000000;
        }
        else if (blackQueens & (1ULL << to)) {
            movePiece(from, to, blackQueens);
            lastMove|=0x0000004000000000;
        }
        else {
            movePiece(from, to, blackKings);
            lastMove|=0x0000002000000000;
        }
    } else {
        if (whitePawns & (1ULL << to)) {
            movePiece(from, to, whitePawns);
            lastMove|=0x0000040000000000;
        }
        else if (whiteKnights & (1ULL << to)) {
            movePiece(from, to, whiteKnights);
            lastMove|=0x0000020000000000;
        }
        else if (whiteBishops & (1ULL << to)) {
            movePiece(from, to, whiteBishops);
            lastMove|=0x0000010000000000;
        }
        else if (whiteRooks & (1ULL << to)) {
            movePiece(from, to, whiteRooks);
            lastMove|=0x0000008000000000;
        }
        else if (whiteQueens & (1ULL << to)) {
            movePiece(from, to, whiteQueens);
            lastMove|=0x0000004000000000;
        } else {
            movePiece(from, to, whiteKings);
            lastMove|=0x0000002000000000;
        }
    }
}

void Board::movePiece(unsigned char from, unsigned char to, uint64_t& pieceBoard) {
    pieceBoard &= ~(1ULL << from);
    pieceBoard |= 1ULL << to;
}

void Board::capture(unsigned char to) {

    if (isWhiteMove()) {
        if (blackPawns & (1ULL << to)) {
            capturePiece(to, blackPawns);
            lastMove|=0x0000080000000000;
        }
        else if (blackKnights & (1ULL << to)) {
            capturePiece(to, blackKnights);
            lastMove|=0x0000100000000000;
        }
        else if (blackBishops & (1ULL << to)) {
            capturePiece(to, blackBishops);
            lastMove&=0x0000200000000000;
        }
        else if (blackRooks & (1ULL << to)) {
            capturePiece(to, blackRooks);
            lastMove&=0x0000400000000000;
        }
        else if (blackQueens & (1ULL << to)) {
            capturePiece(to, blackQueens);
            lastMove|=0x0000800000000000;
        }
    } else {
        if (whitePawns & (1ULL << to)) {
            capturePiece(to, whitePawns);
            lastMove|=0x0000080000000000;
        }
        else if (whiteKnights & (1ULL << to)) {
            capturePiece(to, whiteKnights);
            lastMove|=0x0000100000000000;
        }
        else if (whiteBishops & (1ULL << to)) {
            capturePiece(to, whiteBishops);
            lastMove|=0x0000200000000000;
        }
        else if (whiteRooks & (1ULL << to)) {
            capturePiece(to, whiteRooks);
            lastMove|=0x0000400000000000;
        }
        else if (whiteQueens & (1ULL << to)) {
            capturePiece(to, whiteQueens);
            lastMove|=0x0000800000000000;
        }
    }
}

void Board::capturePiece(unsigned char to, uint64_t& pieceBoard) {
    pieceBoard &= ~(1ULL << to);
}

// Logic Functions
bool Board::isOccupied(int tileId) const {
    return (allPieces & (1ULL << tileId));
}

bool Board::isEnemyOccupied(int tileId) const { // Is enemy on tileId tile
    if(isWhiteMove()) {
        return isBlack(tileId);
    } else {
        return isWhite(tileId);
    }
}

bool Board::isWhite(int tileId) const { return whitePieces & (1ULL << tileId); }
bool Board::isBlack(int tileId) const { return blackPieces & (1ULL << tileId); }

bool Board::isEnPassantEligible(int tileId) const {

    // checks if last move was a 2 forward advance
    if(Board::isWhiteMove()) {
        return (blackPawns & (1ULL << tileId)) && this->getLastMoveFrom() == tileId+16
               && this->getLastMoveTo() == tileId;
    } else {
        return (whitePawns & (1ULL << tileId)) && this->getLastMoveFrom() == tileId-16
               && this->getLastMoveTo() == tileId;
    }
}

bool Board::isAttacked(int tileId, bool isWhite) const {
    long long moves;

    //qWarning() << "IsWhite: " << isWhite << " Pos " << tileId;
    moves = Rook::allMoves(tileId, *this);
    if ((moves & (isWhite ? (this->getBlackRooks() | this->getBlackQueens())
                          : (this->getWhiteRooks() | this->getWhiteQueens()))) != 0) {
        //qWarning() << "Attacker: Rook";
        return true;
    }

    moves = Bishop::allMoves(tileId, *this);
    if ((moves & (isWhite ? (this->getBlackBishops() | this->getBlackQueens())
                          : (this->getWhiteBishops() | this->getWhiteQueens()))) != 0) {
        //qWarning() << "Attacker: Bishop";
        return true;
    }

    moves = Knight::allMoves(tileId, *this);
    if ((moves & (isWhite ? this->getBlackKnights() : this->getWhiteKnights())) != 0) {
        //qWarning() << "Attacker: Knight";
        return true;
    }

    long long kingPos = 1ULL << tileId;
    if (isWhite) {
        if ((this->getBlackPawns() & (kingPos << 7)) != 0 || (this->getBlackPawns() & (kingPos << 9)) != 0) {
            //qWarning() << "Attacker: BlackPawn";
            return true;
        }
    } else {
        if ((this->getWhitePawns() & (kingPos >> 7)) != 0 || (this->getWhitePawns() & (kingPos >> 9)) != 0) {
            //qWarning() << "Attacker: WhitePawn";
            return true;
        }
    }

    long long kingAttacks = 0;
    kingAttacks |= (kingPos << 8);
    kingAttacks |= (kingPos >> 8);
    kingAttacks |= (kingPos << 1) & ~0x0101010101010101ULL;
    kingAttacks |= (kingPos >> 1) & ~0x8080808080808080ULL;
    kingAttacks |= (kingPos << 9) & ~0x0101010101010101ULL;
    kingAttacks |= (kingPos << 7) & ~0x8080808080808080ULL;
    kingAttacks |= (kingPos >> 9) & ~0x8080808080808080ULL;
    kingAttacks |= (kingPos >> 7) & ~0x0101010101010101ULL;

    if ((kingAttacks & (isWhite ? this->getBlackKings() : this->getWhiteKings())) != 0) {
        return true;
    }

    return false;
}

Board::~Board() {
    existingBoards-=1;
}

