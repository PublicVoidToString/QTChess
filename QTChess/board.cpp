#include "board.h"
#include "pawn.h"
#include <iostream>
#include <bitset>

board::board() {
    whiteMove = true;
    selected = 64;
    clearSelected = 64;
    clearMoves   = 0b0000000000000000000000000000000000000000000000000000000000000000;
    moves        = 0b0000000000000000000000000000000000000000000000000000000000000000;

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
}

bool board::isWhiteMove() const {
    return whiteMove;
}

bool board::isOccupied(int buttonId) const {
    return (whitePawns | whiteKnights | whiteRooks | whiteBishops | whiteQueens | whiteKings |
                blackPawns | blackKnights | blackRooks | blackBishops | blackQueens | blackKings) & (1ULL << buttonId);
}

bool board::isEnemyOccupied(int buttonId) const {
    if(whiteMove) {
        return (blackPawns | blackKnights | blackRooks | blackBishops | blackQueens | blackKings) & (1ULL << buttonId);
    } else {
        return (whitePawns | whiteKnights | whiteRooks | whiteBishops | whiteQueens | whiteKings) & (1ULL << buttonId);
    }
}

void board::pressedButton(int buttonId)
{

    //// do testowania legalności ruchów (wypisany bitboard w konsoli)
    ////

    clearSelected = selected;
    clearMoves = moves;
    moves = Pawn::legalMoves(buttonId, *this);

    std::bitset<64> bitset(moves);
    std::string legalMovesStr = bitset.to_string();
    std::cout << "Legal moves bitmap: " << legalMovesStr << std::endl;

    if (whitePawns & (1ULL << buttonId)) {
        selected = buttonId;
    }
    else if (whiteRooks & (1ULL << buttonId)) {
        selected = buttonId; // TODO: Rozważ rozgrywkę wieżami
    }
    else if (whiteKnights & (1ULL << buttonId)) {
        selected = buttonId;
    }
    else if (whiteBishops & (1ULL << buttonId)) {
        selected = buttonId;
    }
    else if (whiteQueens & (1ULL << buttonId)) {
        selected = buttonId;
    }
    else if (whiteKings & (1ULL << buttonId)) {
        selected = buttonId; // TODO: Rozgrywka królem (np. roszada)
    }

    // Sprawdzenie obecności figury czarnej
    else if (blackPawns & (1ULL << buttonId)) {
        selected = buttonId;
    }
    else if (blackRooks & (1ULL << buttonId)) {
        selected = buttonId; // TODO: Rozgrywka wieżami
    }
    else if (blackKnights & (1ULL << buttonId)) {
        selected = buttonId;
    }
    else if (blackBishops & (1ULL << buttonId)) {
        selected = buttonId;
    }
    else if (blackQueens & (1ULL << buttonId)) {
        selected = buttonId;
    }
    else if (blackKings & (1ULL << buttonId)) {
        selected = buttonId; // TODO: Rozgrywka królem (np. roszada)
    }

    // Gdy zostało wciśnięte pole z tablicy ruchu
    else if (selected != 64 && (moves & (1ULL << buttonId))) {
        // Logika dla ruchu figury na wybrane pole
        // Możesz dodać odpowiednią funkcję, która zaktualizuje stan gry
        // Przykład:
        selected = 64; // Resetowanie zaznaczenia po wykonaniu ruchu
    }

    // Gdy zostało wciśnięte puste pole
    else {
        selected = 64; // Resetowanie zaznaczenia, jeśli pole jest puste
        moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
    }

    // Sprawdzanie, czy zmieniła się pozycja, i resetowanie zaznaczenia, jeśli nie
    if (selected == clearSelected) {
        selected = 64; // Resetowanie, gdy nie zmieniono pozycji
        moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
    }
}
