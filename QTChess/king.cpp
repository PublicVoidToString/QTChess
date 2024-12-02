#include "king.h"

unsigned long long King::legalMoves(int positionId, const board& board) {

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

    // TODO roszada

    return legalMovesBitmap;
}
