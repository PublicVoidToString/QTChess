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
    lastMove[0] = 64; // FROM; zainicjowane poza szachownicą;
    lastMove[1] = 64; // TO; zainicjowane poza szachownicą;
    clearMoves   = 0b0000000000000000000000000000000000000000000000000000000000000000;
    moves        = 0b0000000000000000000000000000000000000000000000000000000000000000;

    whiteLongCastlePossible = true;
    whiteShortCastlePossible = true;
    blackLongCastlePossible = true;
    blackShortCastlePossible = true;

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

bool board::getWhiteLongCastlePossible() const {
    return whiteLongCastlePossible;
}
bool board::getWhiteShortCastlePossible() const {
    return whiteShortCastlePossible;
}
bool board::getBlackLongCastlePossible() const {
    return blackLongCastlePossible;
}
bool board::getBlackShortCastlePossible() const {
    return blackShortCastlePossible;
}

bool board::isEnPassantEligible(int buttonId) const {

    // Sprawdza czy ostatni ruch był wyjściem o dwa pola do przodu

    if(board::isWhiteMove()) {
        return (blackPawns & (1ULL << buttonId)) && lastMove[0] == buttonId+16 && lastMove[1] == buttonId;
    } else {
        return (whitePawns & (1ULL << buttonId)) && lastMove[0] == buttonId-16 && lastMove[1] == buttonId;
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
            lastMove[0] = selected; // ustawienie ostatniego ruchu (do sprawdzania en passant)
            lastMove[1] = buttonId;
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
        bool isEnPassant = false;
        unsigned char capturedPawnPosition = -1; // zmienna na wypadek en passant, przechowująca jego lokalizację

        // Sprawdź, do której zmiennej należy figura na polu "from"
        if (whitePawns & (1ULL << from)) {
            whitePawns &= ~(1ULL << from); // Usuwanie figury z poprzedniego pola
            whitePawns |= (1ULL << to);    // Przesunięcie figury na nowe pole
            isWhitePiece = true;

            if ((to - from) == 9 || (to - from) == 7) { // Sprawdzenie - czy nastąpiło En Passant
                if (!this->isEnemyOccupied(to)) {
                    isEnPassant = true;
                    capturedPawnPosition = to - 8;
                }
            }

        } else if (whiteRooks & (1ULL << from)) {

            if (from == 0) {
                whiteLongCastlePossible = false;
            } else if (from == 7) {
                whiteShortCastlePossible = false;
            }

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

            // Roszady dla białych
            if (from == 4 && to == 6 && whiteShortCastlePossible) { // Krótka roszada białych
                whiteRooks &= ~(1ULL << 7);
                whiteRooks |= (1ULL << 5);
            } else if (from == 4 && to == 2 && whiteLongCastlePossible) { // Długa roszada białych
                whiteRooks &= ~(1ULL << 0);
                whiteRooks |= (1ULL << 3);
            }

            // Po ruchu króla roszada przestaje być legalna
            whiteShortCastlePossible = false;
            whiteLongCastlePossible = false;

            isWhitePiece = true;
        } else if (blackPawns & (1ULL << from)) {
            blackPawns &= ~(1ULL << from);
            blackPawns |= (1ULL << to);

            if ((from - to) == 9 || (from - to) == 7) {
                if (!this->isEnemyOccupied(to)) {
                    isEnPassant = true;
                    capturedPawnPosition = to + 8;
                }
            }

        } else if (blackRooks & (1ULL << from)) {

            if (from == 56) {
                blackLongCastlePossible = false;
            } else if (from == 63) {
                blackShortCastlePossible = false;
            }

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

            if (from == 60 && to == 62 && blackShortCastlePossible) { // Krótka roszada czarnych
                blackRooks &= ~(1ULL << 63);
                blackRooks |= (1ULL << 61);
            } else if (from == 60 && to == 58 && blackLongCastlePossible) { // Długa roszada czarnych
                blackRooks &= ~(1ULL << 56);
                blackRooks |= (1ULL << 59);
            }
            blackShortCastlePossible = false;
            blackLongCastlePossible = false;
        }

        // Usunięcie figury przeciwnika z docelowego pola, jeśli tam była
        if (isEnPassant) {
            if (isWhitePiece) {
                blackPawns &= ~(1ULL << capturedPawnPosition);
            } else {
                whitePawns &= ~(1ULL << capturedPawnPosition);
            }
        } else {
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
}

bool board::isDraw() const {
    return false;
}

bool board::isBlackMated() const {
    return false;
}

bool board::isWhiteMated() const {
    return false;
}

unsigned char board::sumBits(unsigned long long variable) const {
    unsigned char sum = 0;
    unsigned long long buffer = variable;
    for (sum = 0; buffer; sum++)
    {
        buffer &= buffer - 1; // Usunięcie najmniej znaczącego bitu
    }

    return sum;
}


long long board::sumWhiteMaterial() const {
    long long score = 0;
    score += sumBits(whitePawns);
    score += sumBits(whiteBishops)*3;
    score += sumBits(whiteKnights)*3;
    score += sumBits(whiteRooks)*5;
    score += sumBits(whiteQueens)*9;

    return score;
}

long long board::sumBlackMaterial() const {
    long long score = 0;
    score += sumBits(blackPawns);
    score += sumBits(blackBishops)*3;
    score += sumBits(blackKnights)*3;
    score += sumBits(blackRooks)*5;
    score += sumBits(blackQueens)*9;

    return score;
}


long long board::evaluatePosition() const {
    long long finalScore = 0;

    // Sprawdzenie czy nastąpił mat, pat TODO

    if(isDraw()) {
        return 0;
    }

    if(isBlackMated()) {
        return 1000;
    }

    if(isWhiteMated()) {
        return -1000;
    }

    // Policzenie materiału

    finalScore += sumWhiteMaterial();
    finalScore -= sumBlackMaterial();

    // Wzięcie pod uwagę lokalizację materiału TODO


    return finalScore;
}
