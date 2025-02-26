#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "knight.h"

uint64_t King::moves[]={};

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
        if (!board.isAttacked(newPosition, board.isWhiteMove()))
            legalMovesBitmap |= (1ULL << newPosition);
    }

    if (board.getWhiteLongCastlePossible() && positionId == 4) {
        if (!board.isOccupied(1) && !board.isOccupied(2) && !board.isOccupied(3)) {
            if(!board.isAttacked(2,true) && !board.isAttacked(3,true) && !board.isAttacked(4,true))
                legalMovesBitmap |= (1ULL << 2);
        }
    }

    if (board.getWhiteShortCastlePossible() && positionId == 4) {
        if (!board.isOccupied(5) && !board.isOccupied(6)) {
            if(!board.isAttacked(4,true) && !board.isAttacked(5,true) && !board.isAttacked(6,true))
                legalMovesBitmap |= (1ULL << 6);
        }
    }
    if (board.getBlackLongCastlePossible() && positionId == 60) {
        if (!board.isOccupied(57) && !board.isOccupied(58) && !board.isOccupied(59)) {
            if(!board.isAttacked(58,false) && !board.isAttacked(59,false) && !board.isAttacked(60,false))
                legalMovesBitmap |= (1ULL << 58);
        }
    }
    if (board.getBlackShortCastlePossible() && positionId == 60) {
        if (!board.isOccupied(61) && !board.isOccupied(62)) {
            if(!board.isAttacked(60,false) && !board.isAttacked(61,false) && !board.isAttacked(62,false))
                legalMovesBitmap |= (1ULL << 62);
        }
    }


    return legalMovesBitmap;

}


unsigned long long King::fastMoves(int positionId, const Board& board) {
    if(board.isWhiteMove()){
        return moves[positionId] & ~(board.getWhitePieces());
    } else{
        return moves[positionId] & ~(board.getBlackPieces());
    }
}

void King::calcMoves() {
    for (int clear = 0; clear < 64; ++clear) moves[clear] = 0;

    for (int tile = 0; tile < 64; ++tile) {
        int file = tile % 8;

        int moveOffsets[] = {-9, -8, -7, -1, 1, 7, 8, 9};

        for (int offset : moveOffsets) {
            int target = tile + offset;
            int targetFile = target % 8;

            if (target >= 0 && target < 64) {
                if (abs(targetFile - file) <= 1) {
                    moves[tile] |= 1ULL << target;
                }
            }
        }
    }
}
