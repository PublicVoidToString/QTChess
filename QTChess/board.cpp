#include "Board.h"
//pieces
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "king.h"
#include <cstddef>
#include <QMessageBox>

// PRIVATE:

// Functions
void Board::move(unsigned char from, unsigned char to) // Function making moving figure from->to on current board
{
    bool isWhitePiece = false;
    bool isEnPassant = false;
    unsigned char capturedPawnPosition = -1; // variable used in case of enPassant

    // Checking move type by piece and from location
    if (whitePawns & (1ULL << from)) {
        whitePawns &= ~(1ULL << from); // remove a piece from old location
        whitePawns |= (1ULL << to);    // adding a piece to a new location
        isWhitePiece = true;

        if ((to - from) == 9 || (to - from) == 7) { // checking special move - en passant
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

        // Castles
        if (from == 4 && to == 6 && whiteShortCastlePossible) { // 0-0 short castles
            whiteRooks &= ~(1ULL << 7);
            whiteRooks |= (1ULL << 5);
        } else if (from == 4 && to == 2 && whiteLongCastlePossible) { // 0-0-0 long castles
            whiteRooks &= ~(1ULL << 0);
            whiteRooks |= (1ULL << 3);
        }

        // Updating castling legality after moving
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

        if (from == 60 && to == 62 && blackShortCastlePossible) {
            blackRooks &= ~(1ULL << 63);
            blackRooks |= (1ULL << 61);
        } else if (from == 60 && to == 58 && blackLongCastlePossible) {
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

// Evaluation functions
unsigned char Board::sumBits(unsigned long long variable) const {
    unsigned char sum = 0;
    unsigned long long buffer = variable;
    for (sum = 0; buffer; sum++)
    {
        buffer &= buffer - 1; // Removing least significant bit
    }

    return sum;
}
long long Board::sumWhiteMaterial() const {
    long long score = 0;
    score += sumBits(whitePawns);
    score += sumBits(whiteBishops)*3;
    score += sumBits(whiteKnights)*3;
    score += sumBits(whiteRooks)*5;
    score += sumBits(whiteQueens)*9;

    return score;
}
long long Board::sumBlackMaterial() const {
    long long score = 0;
    score += sumBits(blackPawns);
    score += sumBits(blackBishops)*3;
    score += sumBits(blackKnights)*3;
    score += sumBits(blackRooks)*5;
    score += sumBits(blackQueens)*9;

    return score;
}

// PUBLIC:

Board::Board() {
    whiteMove = true; //TODO: Zmienić na numer rundy %2 i masz ruch
    selected = 64;
    clearSelected = 64;
    lastMove[0] = 64; // initiated out of the chessboard
    lastMove[1] = 64;
    moves        = 0b0000000000000000000000000000000000000000000000000000000000000000;
    clearMoves   = 0b0000000000000000000000000000000000000000000000000000000000000000;

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

    prev = NULL;
    right = NULL;
    left = NULL;
    next = NULL;
}
Board::Board(Board* previousBoard) {
    whiteMove = !previousBoard->whiteMove; //TODO: Zmienić na numer rundy %2 i masz ruch
    selected = 64;
    clearSelected = 64;
    moves        = 0b0000000000000000000000000000000000000000000000000000000000000000;
    clearMoves   = 0b0000000000000000000000000000000000000000000000000000000000000000;

    whiteShortCastlePossible = previousBoard->whiteShortCastlePossible;
    whiteLongCastlePossible = previousBoard->whiteLongCastlePossible;
    blackShortCastlePossible = previousBoard->blackShortCastlePossible;
    blackLongCastlePossible = previousBoard->blackLongCastlePossible;

    whitePawns   = previousBoard->whitePawns;
    whiteRooks   = previousBoard->whiteRooks;
    whiteKnights = previousBoard->whiteKnights;
    whiteBishops = previousBoard->whiteBishops;
    whiteQueens  = previousBoard->whiteQueens;
    whiteKings   = previousBoard->whiteKings;

    blackPawns   = previousBoard->blackPawns;
    blackRooks   = previousBoard->blackRooks;
    blackKnights = previousBoard->blackKnights;
    blackBishops = previousBoard->blackBishops;
    blackQueens  = previousBoard->blackQueens;
    blackKings   = previousBoard->blackKings;



    prev = previousBoard;
    right = NULL;
    left = NULL;
    //next = NULL //miłego szukania errora
}

// Main Functions
bool Board::pressedButton(int buttonId) // Reads input and calls most of other functions
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
            nextMove(selected, buttonId);
            lastMove[0] = selected; // ustawienie ostatniego ruchu (do sprawdzania en passant)
            lastMove[1] = buttonId;
            moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
            selected = 64;
            return true;
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
    return false;
}
long long Board::evaluatePosition() const { // Main eval function, calculating based on private eval functions
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

// Logic Functions
bool Board::isOccupied(int buttonId) const { // Is any figure on buttonID tile
    return (whitePawns | whiteKnights | whiteRooks | whiteBishops | whiteQueens | whiteKings |
            blackPawns | blackKnights | blackRooks | blackBishops | blackQueens | blackKings) & (1ULL << buttonId);
}
bool Board::isEnemyOccupied(int buttonId) const { // Is enemy on buttonID tile
    if(whiteMove) {
        return (blackPawns | blackKnights | blackRooks | blackBishops | blackQueens | blackKings) & (1ULL << buttonId);
    } else {
        return (whitePawns | whiteKnights | whiteRooks | whiteBishops | whiteQueens | whiteKings) & (1ULL << buttonId);
    }
}
bool Board::isEnPassantEligible(int buttonId) const {

    // Sprawdza czy ostatni ruch był wyjściem o dwa pola do przodu

    if(Board::isWhiteMove()) {
        return (blackPawns & (1ULL << buttonId)) && lastMove[0] == buttonId+16 && lastMove[1] == buttonId;
    } else {
        return (whitePawns & (1ULL << buttonId)) && lastMove[0] == buttonId-16 && lastMove[1] == buttonId;
    }
}
//TODO
bool Board::isDraw() const {
    return false;
}
bool Board::isBlackMated() const {
    return false;
}
bool Board::isWhiteMated() const {
    return false;
}


// Engine Functions
void Board::nextMove(unsigned char from, unsigned char to) { // Function creating new Board instance on next and perfoming move on it
    next = new Board(this);
    next->move(from,to);
}

// Getters
bool Board::isWhiteMove() const { return whiteMove; }

bool Board::getWhiteLongCastlePossible() const { return whiteLongCastlePossible; }
bool Board::getWhiteShortCastlePossible() const { return whiteShortCastlePossible; }
bool Board::getBlackLongCastlePossible() const { return blackLongCastlePossible; }
bool Board::getBlackShortCastlePossible() const { return blackShortCastlePossible; }

long long Board::getWhitePawns() const { return whitePawns; }
long long Board::getWhiteKnights() const { return whiteKnights; }
long long Board::getWhiteRooks() const { return whiteRooks; }
long long Board::getWhiteBishops() const { return whiteBishops; }
long long Board::getWhiteQueens() const { return whiteQueens; }
long long Board::getWhiteKings() const { return whiteKings; }

long long Board::getBlackPawns() const { return blackPawns; }
long long Board::getBlackKnights() const { return blackKnights; }
long long Board::getBlackRooks() const { return blackRooks; }
long long Board::getBlackBishops() const { return blackBishops; }
long long Board::getBlackQueens() const { return blackQueens; }
long long Board::getBlackKings() const { return blackKings; }

unsigned char Board::getSelected() const { return selected; }
unsigned char Board::getClearSelected() const { return clearSelected; }
long long Board::getMoves() const { return moves; }
long long Board::getClearMoves() const { return clearMoves; }
