#include "rook.h"

unsigned long long Rook::legalMoves(int positionId, const board& board) {
    unsigned long long legalMovesBitmap = 0; // Bitboard dla możliwych ruchów

    // Kierunki ruchu wieży: w prawo, w lewo, w górę, w dół
    const int directions[4][2] = {
        {1, 0},  // W prawo
        {-1, 0}, // W lewo
        {0, 1},  // W górę
        {0, -1}  // W dół
    };

    // Oblicz współrzędne z pozycji ID
    int x = positionId % 8; // Kolumna (0-7)
    int y = positionId / 8; // Wiersz (0-7)

    // Iteracja przez kierunki ruchu
    for (const auto& direction : directions) {
        int nx = x;
        int ny = y;

        // Idziemy w danym kierunku, dopóki nie trafimy na przeszkodę
        while (true) {
            nx += direction[0]; // Nowa kolumna
            ny += direction[1]; // Nowy wiersz

            // Sprawdź, czy nowa pozycja jest w granicach planszy
            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) {
                break; // Wyjście poza planszę
            }

            int newPosition = ny * 8 + nx; // Przelicz na ID pozycji (0-63)

            if (board.isOccupied(newPosition)) {
                // Jeśli pole jest zajęte, sprawdź czy to przeciwnik
                if (board.isEnemyOccupied(newPosition)) {
                    legalMovesBitmap |= (1ULL << newPosition); // Można bić przeciwnika
                }
                break; // Blokada ruchu w tym kierunku
            }

            // Pole jest wolne
            legalMovesBitmap |= (1ULL << newPosition);
        }
    }

    return legalMovesBitmap; // Zwróć bitboard możliwych ruchów
}
