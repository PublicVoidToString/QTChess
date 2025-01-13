#include "king.h"
#include "bishop.h"
#include "pawn.h"
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
        legalMovesBitmap |= (1ULL << newPosition);
    }

    if (board.getWhiteLongCastlePossible() && positionId == 4) {
        if (!board.isOccupied(1) && !board.isOccupied(2) && !board.isOccupied(3)) {
            legalMovesBitmap |= (1ULL << 2); // Długa roszada białego
        }
    }

    if (board.getWhiteShortCastlePossible() && positionId == 4) {
        if (!board.isOccupied(5) && !board.isOccupied(6)) {
            legalMovesBitmap |= (1ULL << 6); // Krótka roszada białego
        }
    }
    if (board.getBlackLongCastlePossible() && positionId == 60) {
        if (!board.isOccupied(57) && !board.isOccupied(58) && !board.isOccupied(59)) {
            legalMovesBitmap |= (1ULL << 58);
        }
    }
    if (board.getBlackShortCastlePossible() && positionId == 60) {
        if (!board.isOccupied(61) && !board.isOccupied(62)) {
            legalMovesBitmap |= (1ULL << 62);
        }
    }


    return legalMovesBitmap;

}

bool King::isInCheck(int positionId, const Board& board, bool isWhite){
    long long moves;

    moves = Rook::legalMoves(positionId,board);
    if((moves & (isWhite ? (board.getBlackRooks()|board.getBlackQueens()) : (board.getWhiteRooks()|board.getWhiteQueens()))) != 0) return true;

    moves = Bishop::legalMoves(positionId,board);
    if((moves & (isWhite ? (board.getBlackBishops()|board.getBlackQueens()) : (board.getWhiteBishops()|board.getWhiteQueens()))) != 0) return true;

    moves = Knight::legalMoves(positionId,board);
    if((moves & (isWhite ? board.getBlackKnights() : board.getWhiteKnights())) != 0) return true;

    long long pawnAttacks = isWhite
                               ? ((1ULL << positionId >> 7) & ~0x0101010101010101ULL) |  ((1ULL << positionId >> 9) & ~0x8080808080808080ULL)
                               : ((1ULL << positionId << 7) & ~0x8080808080808080ULL) |  ((1ULL << positionId << 9) & ~0x0101010101010101ULL);
    if ((pawnAttacks & (isWhite ? board.getBlackPawns() : board.getWhitePawns())) != 0) {
        return true;
    }

    return false;
}
