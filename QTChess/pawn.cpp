#include "pawn.h"

uint64_t Pawn::movesWhite[]={};
uint64_t Pawn::movesBlack[]={};
uint64_t Pawn::attackWhite[]={};
uint64_t Pawn::attackBlack[]={};

unsigned long long Pawn::allMoves(int positionId, const Board& board, bool isWhite) {

    unsigned long long legalMovesBitmap = 0;

    // TODO - promotion, en passant
    if (isWhite) {
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

unsigned long long Pawn::fastMoves(int positionId, const Board& board) {
    uint64_t moves = 0;
    if(board.isWhiteMove()) {
        if(!board.isOccupied(positionId+8)) moves = movesWhite[positionId] & ~(board.getPieces());
        moves |= (attackWhite[positionId] & board.getBlackPieces());
        if (board.isEnPassantEligible(positionId - 1)) {
            moves |= (1ULL << (positionId + 7));
        }
        if (board.isEnPassantEligible(positionId + 1)) {
            moves |= (1ULL << (positionId + 9));
        }
    }
    else{
        if(!board.isOccupied(positionId-8)) moves = movesBlack[positionId] & ~(board.getPieces());
        moves |= (attackBlack[positionId] & board.getWhitePieces());
        if (board.isEnPassantEligible(positionId - 1)) {
            moves |= (1ULL << (positionId - 9));
        }
        if (board.isEnPassantEligible(positionId + 1)) {
            moves |= (1ULL << (positionId - 7));
        }
    }
    return moves;
}

void Pawn::calcMoves() {
    for (int clear = 0; clear < 64; ++clear) {
        movesWhite[clear] = 0;
        movesBlack[clear] = 0;
        attackWhite[clear] = 0;
        attackBlack[clear] = 0;
    }
    for(int tile =0; tile<64;tile++){
        if(tile<56) movesWhite[tile] |= 1ULL << (tile+8);
        if(tile>7) movesBlack[tile] |= 1ULL << (tile-8);
        if(tile>=8 && tile<=15) movesWhite[tile] |= 1ULL << (tile+16);
        if(tile>=48 && tile<=55) movesBlack[tile] |= 1ULL << (tile-16);


        if(tile%8>0){
            if(tile<56) attackWhite[tile] |= 1ULL << (tile+7);
            if(tile>7) attackBlack[tile] |= 1ULL << (tile-9);
        }
        if(tile%8<7){
            if(tile<56) attackWhite[tile] |= 1ULL << (tile+9);
            if(tile>7) attackBlack[tile] |= 1ULL << (tile-7);
        }
    }
}

