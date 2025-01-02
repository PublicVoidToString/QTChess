#include "pawn.h"

unsigned long long Pawn::legalMoves(int positionId, const Board& board) {

    unsigned long long legalMovesBitmap = 0;

    // TODO - promotion, en passant
    if (board.isWhiteMove()) {
        if (positionId + 8 < 64 && !board.isOccupied(positionId + 8)) {  // Jeden ruch do przodu
            legalMovesBitmap |= (1ULL << (positionId + 8));
        }

        if (positionId >= 8 && positionId < 16 && !board.isOccupied(positionId + 8) && !board.isOccupied(positionId + 16)) { // Dwa pola do przodu
            legalMovesBitmap |= (1ULL << (positionId + 16));
        }

        if (positionId%8 != 0 && positionId + 8 < 64 && board.isEnemyOccupied(positionId+7)) { // Bicie po lewej przekątnej (z perspektywy białego)
            legalMovesBitmap |= (1ULL << (positionId + 7));
        }

        if ((positionId+1)%8 != 0 && positionId + 8 < 64 && board.isEnemyOccupied(positionId+9)) { // Bicie po lewej przekątnej (z perspetywy białego)
            legalMovesBitmap |= (1ULL << (positionId + 9));
        }

        // **En passant capture**
        if (positionId >= 32 && positionId < 40 && board.isEnPassantEligible(positionId - 1)) { // Left en passant
            legalMovesBitmap |= (1ULL << (positionId + 7));
        }
        if (positionId >= 32 && positionId < 40 && board.isEnPassantEligible(positionId + 1)) { // Right en passant
            legalMovesBitmap |= (1ULL << (positionId + 9));
        }

    } else {
        // to samo dla czarnych figur
        if (positionId - 8 >= 0 && !board.isOccupied(positionId - 8)) {
            legalMovesBitmap |= (1ULL << (positionId - 8));
        }

        if (positionId >= 48 && positionId < 56 && !board.isOccupied(positionId - 8) && !board.isOccupied(positionId - 16)) {
            legalMovesBitmap |= (1ULL << (positionId - 16));
        }

        // Bicie po lewej przekątnej (z perspektywy białego)
        if (positionId % 8 != 0 && positionId - 8 >= 0 && board.isEnemyOccupied(positionId - 9)) {
            legalMovesBitmap |= (1ULL << (positionId - 9));
        }

        // Bicie po prawej przekątnej (z perspektywy białego)
        if ((positionId + 1) % 8 != 0 && positionId - 8 >= 0 && board.isEnemyOccupied(positionId - 7)) {
            legalMovesBitmap |= (1ULL << (positionId - 7));
        }

        // **En passant capture**
        if (positionId >= 24 && positionId < 32 && board.isEnPassantEligible(positionId - 1)) { // Left en passant
            legalMovesBitmap |= (1ULL << (positionId - 9));
        }
        if (positionId >= 24 && positionId < 32 && board.isEnPassantEligible(positionId + 1)) { // Right en passant
            legalMovesBitmap |= (1ULL << (positionId - 7));
        }

    }

    return legalMovesBitmap;
}
