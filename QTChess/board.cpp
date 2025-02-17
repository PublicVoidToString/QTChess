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
    lastMove=previousBoard->lastMove & ~(0b1111111111111111111111111111000000000000010000000000000000000000);
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
    calculateOrderingScore();

    whitePieces = (whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKings);
    blackPieces = (blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKings);
    allPieces = (whitePieces | blackPieces);

    prev = previousBoard;
    right = nullptr;
    left = nullptr;
    next = nullptr;
    if(getGameState()==0){
        checkThreeRule();
    }
    checkFiftyRule(getGameState()==0);
    checkInSufficientMaterial();
}

void Board::makeMove(unsigned char from, unsigned char to, unsigned short promotion) // Making move on board without checking it's legality
{
    setLastMovePromotion(promotion);
    setLastMoveFrom(from);
    setLastMoveTo(to);

    // clearing last move - move, capture, en passant
    lastMove&=0xFFFF000FFFFFFFFF;

    uint64_t* bitboardTO = getBitboard(to);

    if(bitboardTO!=nullptr) {
        updateCapturePiece(*bitboardTO);
        // update last move
        *bitboardTO &= ~(1ULL<<to);
        resetFiftyRule();
        // removes the captured piece

        // rook capture disableing castling
        if((*bitboardTO)==whiteRooks){
            if (to == 0) {
                blockWhiteLongCastle();
            } else if (to == 7) {
                blockWhiteShortCastle();
            }
        }else if((*bitboardTO)==blackRooks) {
            if (to == 56) {
                blockBlackLongCastle();
            } else if (to == 63) {
                blockBlackShortCastle();
            }
        }
    }

    uint64_t* bitboardFROM = getBitboard(from);
    if (bitboardFROM == nullptr) {
        qWarning() << "Invalid move: no bitboard found for tile: " << to << " (ERROR BK01)";
        return;
    }

    updateMovingPiece(*bitboardFROM);
    *bitboardFROM ^= ((1ULL<<from)|(1ULL<<to));
    // moves the piece

    // special cases: promotion, en passant, castling, rook/king move disableing castling
    if((*bitboardFROM)==whitePawns){
        resetFiftyRule();
        if(promotion > 0) promote(to, promotion);
        if((to-from==9 || to-from==7) && to<=47 && to>=40 && !isOccupied(to))  {
            setLastMoveEnPassant();
            blackPawns &= ~(1ULL<<(to-8));
        }
    }else if((*bitboardFROM)==blackPawns){
        resetFiftyRule();
        if(promotion > 0) promote(to, promotion);
        if((from-to==9 || from-to==7) && to<=23 && to>=16 && !isOccupied(to))  {
            setLastMoveEnPassant();
            whitePawns &= ~(1ULL<<(to+8));
        }
    }
    else if((*bitboardFROM)==whiteKings){
        if (from == 4 && to == 6) {
            whiteRooks &= ~(1ULL << 7);
            whiteRooks |= (1ULL << 5);
        } else if (from == 4 && to == 2) {
            whiteRooks &= ~(1ULL << 0);
            whiteRooks |= (1ULL << 3);
        }
        blockWhiteLongCastle();
        blockWhiteShortCastle();

    }else if((*bitboardFROM)==blackKings){
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
    }else if((*bitboardFROM)==whiteRooks){
        if (from == 0) {
            blockWhiteLongCastle();
        } else if (from == 7) {
            blockWhiteShortCastle();
        }
    }else if((*bitboardFROM)==blackRooks) {
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

void Board::updateMovingPiece(uint64_t movingPieceBitmap) {

    if (isWhiteMove()) {
        if (movingPieceBitmap == blackPawns) {
            lastMove|=0x0000040000000000;
        }
        else if (movingPieceBitmap == blackKnights) {
            lastMove|=0x0000020000000000;
        }
        else if (movingPieceBitmap == blackBishops) {
            lastMove|=0x0000010000000000;
        }
        else if (movingPieceBitmap == blackRooks) {
            lastMove|=0x0000008000000000;
        }
        else if (movingPieceBitmap == blackQueens) {
            lastMove|=0x0000004000000000;
        }
        else {
            lastMove|=0x0000002000000000;
        }
    } else {
        if (movingPieceBitmap == whitePawns) {
            lastMove|=0x0000040000000000;
        }
        else if (movingPieceBitmap == whiteKnights) {
            lastMove|=0x0000020000000000;
        }
        else if (movingPieceBitmap == whiteBishops) {
            lastMove|=0x0000010000000000;
        }
        else if (movingPieceBitmap == whiteRooks) {
            lastMove|=0x0000008000000000;
        }
        else if (movingPieceBitmap == whiteQueens) {
            lastMove|=0x0000004000000000;
        } else {
            lastMove|=0x0000002000000000;
        }
    }
}

void Board::updateCapturePiece(uint64_t capturedPieceBitmap) {

    if (!isWhiteMove()) {
        if (capturedPieceBitmap == blackPawns) {
            lastMove|=0x0000080000000000;
        }
        else if (capturedPieceBitmap == blackKnights) {
            lastMove|=0x0000100000000000;
        }
        else if (capturedPieceBitmap == blackBishops) {
            lastMove|=0x0000200000000000;
        }
        else if (capturedPieceBitmap == blackRooks) {
            lastMove|=0x0000400000000000;
        }
        else if (capturedPieceBitmap == blackQueens) {
            lastMove|=0x0000800000000000;
        }
    } else {
        if (capturedPieceBitmap == whitePawns) {
            lastMove|=0x0000080000000000;
        }
        else if (capturedPieceBitmap == whiteKnights) {
            lastMove|=0x0000100000000000;
        }
        else if (capturedPieceBitmap == whiteBishops) {
            lastMove|=0x0000200000000000;
        }
        else if (capturedPieceBitmap == whiteRooks) {
            lastMove|=0x0000400000000000;
        }
        else if (capturedPieceBitmap == whiteQueens) {
            lastMove|=0x0000800000000000;
        }
    }
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

void Board::calculateOrderingScore() {
    // Queen promotion - most points
    if(getLastMovePromotion() == 1) {
        orderingScore = 100;
        return;
    } else if(getLastMoveEnPassant() != 0) {     // underpromotion = negative points (checked last)
        orderingScore = -1;
        return;
    }

    // WHERE captures occured -> most valuable victim, then least valuable attacker
    if(getCapturedPieceScore() != 0) {
        orderingScore += getCapturedPieceScore() + getMovingPieceScore();
        return;
    }


    // quiet moves - where no promotion or capture happens receive score 0
    orderingScore = 0;
}

bool Board::checkInSufficientMaterial(){
    if(__builtin_popcountll(allPieces) > 4) return false;
    if(__builtin_popcountll(allPieces) == 4){
        if(__builtin_popcountll(whiteKnights) == 2) {
            lastMove |= 0xC0000000; // Set state to draw
            return true;
        }
        if(__builtin_popcountll(blackKnights) == 2) {
            lastMove |= 0xC0000000; // Set state to draw
            return true;
        }
    }
    else if(__builtin_popcountll(allPieces) == 3) {
        if(whiteKnights | blackKnights | whiteBishops | blackBishops) {
            lastMove |= 0xC0000000; // Set state to draw
            return true;
        }
    }
    return false;
}

Board::~Board() {
    existingBoards-=1;
}

