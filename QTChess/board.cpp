#include "board.h"
//pieces
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "queen.h"
#include "king.h"

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

bool board::isEnPassantEligible(int buttonId) const {
    //TODO unimplemented method - now only checks if opponents pawn is there

    if(board::isWhiteMove()) {
        return blackPawns & (1ULL << buttonId);
    } else {
        return whitePawns & (1ULL << buttonId);
    }
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
    clearSelected = selected;
    clearMoves = moves;
    if(isOccupied(buttonId) && !isEnemyOccupied(buttonId)){
        selected = buttonId;
        if(whiteMove){
            if (whitePawns  & 1ULL << selected)   moves = Pawn::legalMoves(buttonId, *this);
            else if (whiteRooks  & 1ULL << selected)   moves = Rook::legalMoves(buttonId, *this);
            else if (whiteKnights  & 1ULL << selected) moves = Knight::legalMoves(buttonId, *this);
            else if (whiteBishops  & 1ULL << selected) moves = Bishop::legalMoves(buttonId, *this);
            else if (whiteQueens  & 1ULL << selected)  moves = Bishop::legalMoves(buttonId, *this) | Rook::legalMoves(buttonId, *this);
            else if (whiteKings  & 1ULL << selected)   moves = King::legalMoves(buttonId, *this);
        }
        else {
            if (blackPawns & 1ULL << selected)   moves = Pawn::legalMoves(buttonId, *this);
            else if (blackRooks & 1ULL << selected)   moves = Rook::legalMoves(buttonId, *this);
            else if (blackKnights & 1ULL << selected) moves = Knight::legalMoves(buttonId, *this);
            else if (blackBishops & 1ULL << selected) moves = Bishop::legalMoves(buttonId, *this);
            else if (blackQueens & 1ULL << selected)  moves = Bishop::legalMoves(buttonId, *this) | Rook::legalMoves(buttonId, *this);
            else if (blackKings & 1ULL << selected)   moves = King::legalMoves(buttonId, *this);
        }
    } else{
        // Gdy zostało wciśnięte pole z tablicy ruchu
        if (moves & (1ULL << buttonId)) {
            move(selected, buttonId);
            moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
            selected = 64;
            whiteMove = !whiteMove;
        }

        // Gdy zostało wciśnięte puste pole
        else {
            selected = 64;
            moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
        }
    }

    // Sprawdzanie, czy zmieniła się pozycja, i resetowanie zaznaczenia, jeśli nie
    if (selected == clearSelected) {
        selected = 64;
        moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
    }
}

void board::move(int from, int to)
{
    if (moves & (1ULL << to)) {
        // Sprawdź, czy figura należy do białych czy czarnych
        bool isWhitePiece = false;

        // Sprawdź, do której zmiennej należy figura na polu "from"
        if (whitePawns & (1ULL << from)) {
            whitePawns &= ~(1ULL << from); // Usuwanie figury z poprzedniego pola
            whitePawns |= (1ULL << to);    // Przesunięcie figury na nowe pole
            isWhitePiece = true;
        } else if (whiteRooks & (1ULL << from)) {
            whiteRooks &= ~(1ULL << from);
            whiteRooks |= (1ULL << to);
            isWhitePiece = true;
        } else if (whiteKnights & (1ULL << from)) {
            whiteKnights &= ~(1ULL << from);
            whiteKnights |= (1ULL << to);
            isWhitePiece = true;
        } else if (whiteBishops & (1ULL << from)) {
            whiteBishops &= ~(1ULL << from);
            whiteBishops |= (1ULL << to);
            isWhitePiece = true;
        } else if (whiteQueens & (1ULL << from)) {
            whiteQueens &= ~(1ULL << from);
            whiteQueens |= (1ULL << to);
            isWhitePiece = true;
        } else if (whiteKings & (1ULL << from)) {
            whiteKings &= ~(1ULL << from);
            whiteKings |= (1ULL << to);
            isWhitePiece = true;
        } else if (blackPawns & (1ULL << from)) {
            blackPawns &= ~(1ULL << from);
            blackPawns |= (1ULL << to);
        } else if (blackRooks & (1ULL << from)) {
            blackRooks &= ~(1ULL << from);
            blackRooks |= (1ULL << to);
        } else if (blackKnights & (1ULL << from)) {
            blackKnights &= ~(1ULL << from);
            blackKnights |= (1ULL << to);
        } else if (blackBishops & (1ULL << from)) {
            blackBishops &= ~(1ULL << from);
            blackBishops |= (1ULL << to);
        } else if (blackQueens & (1ULL << from)) {
            blackQueens &= ~(1ULL << from);
            blackQueens |= (1ULL << to);
        } else if (blackKings & (1ULL << from)) {
            blackKings &= ~(1ULL << from);
            blackKings |= (1ULL << to);
        }

        // Usunięcie figury przeciwnika z docelowego pola, jeśli tam była
        if (isWhitePiece) {
            // Czarna figura na docelowym polu
            blackPawns &= ~(1ULL << to);
            blackRooks &= ~(1ULL << to);
            blackKnights &= ~(1ULL << to);
            blackBishops &= ~(1ULL << to);
            blackQueens &= ~(1ULL << to);
            blackKings &= ~(1ULL << to);
        } else {
            // Biała figura na docelowym polu
            whitePawns &= ~(1ULL << to);
            whiteRooks &= ~(1ULL << to);
            whiteKnights &= ~(1ULL << to);
            whiteBishops &= ~(1ULL << to);
            whiteQueens &= ~(1ULL << to);
            whiteKings &= ~(1ULL << to);
        }
    }
}
