#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "knight.h"


unsigned long long King::legalMoves(int positionId, const Board& board) {

    unsigned long long legalMovesBitmap = 0;

    // 8 możliwych ruchów króla (bez roszady)
    const int directions[8][2] = {
        {0, -1},
        {0, 1},
        {1, 0},
        {-1, 0},
        {1, 1},
        {-1, 1},
        {1, -1},
        {-1, -1}
    };

    // Oblicz współrzędne z pozycji ID
    int x = positionId % 8; // Kolumna (0-7)
    int y = positionId / 8; // Wiersz (0-7)


    for (const auto& direction : directions) {
        int nx = x+direction[0];
        int ny = y+direction[1];

        // Jeśli wychodzi poza planszę
        if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) {
            continue;
        }

        int newPosition = ny * 8 + nx;
        // Pole okupuje przyjacielska figura
        if (board.isOccupied(newPosition) && !board.isEnemyOccupied(newPosition)) {
            continue;
        }

        // Pole jest wolne
        if (!isInCheck(newPosition,board,board.isWhiteMove()))
            legalMovesBitmap |= (1ULL << newPosition);
    }

    if (board.getWhiteLongCastlePossible() && positionId == 4) {
        if (!board.isOccupied(1) && !board.isOccupied(2) && !board.isOccupied(3)) {
            if(!isInCheck(2,board,true) && !isInCheck(3,board,true) && !isInCheck(4,board,true))
                legalMovesBitmap |= (1ULL << 2);
        }
    }

    if (board.getWhiteShortCastlePossible() && positionId == 4) {
        if (!board.isOccupied(5) && !board.isOccupied(6)) {
            if(!isInCheck(4,board,true) && !isInCheck(5,board,true) && !isInCheck(6,board,true))
                legalMovesBitmap |= (1ULL << 6);
        }
    }
    if (board.getBlackLongCastlePossible() && positionId == 60) {
        if (!board.isOccupied(57) && !board.isOccupied(58) && !board.isOccupied(59)) {
            if(!isInCheck(58,board,false) && !isInCheck(59,board,false) && !isInCheck(60,board,false))
                legalMovesBitmap |= (1ULL << 58);
        }
    }
    if (board.getBlackShortCastlePossible() && positionId == 60) {
        if (!board.isOccupied(61) && !board.isOccupied(62)) {
            if(!isInCheck(60,board,false) && !isInCheck(61,board,false) && !isInCheck(62,board,false))
                legalMovesBitmap |= (1ULL << 62);
        }
    }


    return legalMovesBitmap;

}

bool King::isInCheck(int positionId, const Board& board, bool isWhite) {
    return false;
    /*
    Board* noKingBoard = new Board(board);
    noKingBoard->removeKing(isWhite);
    noKingBoard->setTurnNumber(board.getTurnNumber());
    long long moves;

    moves = Rook::allMoves(positionId, *noKingBoard);
    if ((moves & (isWhite ? (noKingBoard->getBlackRooks() | noKingBoard->getBlackQueens())
                          : (noKingBoard->getWhiteRooks() | noKingBoard->getWhiteQueens()))) != 0) {
        delete noKingBoard;
        return true;
    }

    moves = Bishop::allMoves(positionId, *noKingBoard);
    if ((moves & (isWhite ? (noKingBoard->getBlackBishops() | noKingBoard->getBlackQueens())
                          : (noKingBoard->getWhiteBishops() | noKingBoard->getWhiteQueens()))) != 0) {
        delete noKingBoard;
        return true;
    }

    moves = Knight::allMoves(positionId, *noKingBoard);
    if ((moves & (isWhite ? noKingBoard->getBlackKnights() : noKingBoard->getWhiteKnights())) != 0) {
        delete noKingBoard;
        return true;
    }

    long long kingPos = 1ULL << positionId;
    if (isWhite) {
        if ((noKingBoard->getBlackPawns() & (kingPos << 7)) != 0 || (noKingBoard->getBlackPawns() & (kingPos << 9)) != 0) {
            delete noKingBoard;
            return true;
        }
    } else {
        if ((noKingBoard->getWhitePawns() & (kingPos >> 7)) != 0 || (noKingBoard->getWhitePawns() & (kingPos >> 9)) != 0) {
            delete noKingBoard;
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

    if ((kingAttacks & (isWhite ? noKingBoard->getBlackKings() : noKingBoard->getWhiteKings())) != 0) {
        delete noKingBoard;
        return true;
    }

    delete noKingBoard;
    return false;
    */
}

