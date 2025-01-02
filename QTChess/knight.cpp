#include "knight.h"

unsigned long long Knight::legalMoves(int positionId, const Board& board) {

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
