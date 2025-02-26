#include "knight.h"

uint64_t Knight::moves[] = {};

unsigned long long Knight::allMoves(int positionId, const Board& board) {

    unsigned long long legalMovesBitmap = 0;

    // 8 możliwych ruchów skoczka (zapisane jako zmiana względem positionId)
    const int directions[8][2] = {
        {1, -2},
        {2,-1},
        {2,1},
        {1,2},
        {-1, 2},
        {-2,1},
        {-2,-1},
        {-1,-2}
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


    return legalMovesBitmap;
}

unsigned long long Knight::fastMoves(int positionId, const Board& board) {
    if(board.isWhiteMove()){
        return moves[positionId] & ~(board.getWhitePieces());
    } else{
        return moves[positionId] & ~(board.getBlackPieces());
    }
}

void Knight::calcMoves() {
    for (int clear = 0; clear < 64; ++clear) moves[clear] = 0;

    // All 8 possible knight moves
    int knightOffsets[] = {15, 17, 6, 10, -15, -17, -6, -10};

    for (int tile = 0; tile < 64; ++tile) {
        for (int offset : knightOffsets) {
            int newTile = tile + offset;

            // Ensure newTile is within bounds (0-63)
            if (newTile >= 0 && newTile < 64) {
                int oldCol = tile % 8;
                int newCol = newTile % 8;

                // The knight must move only 1 or 2 columns away
                if (std::abs(oldCol - newCol) == 1 || std::abs(oldCol - newCol) == 2) {
                    moves[tile] |= (1ULL << newTile);  // Use 1ULL to avoid shift overflow
                }
            }
        }
    }
}
