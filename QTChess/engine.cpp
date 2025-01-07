#include "engine.h"

Engine::Engine() {}

// Engine Functions
void Engine::nextMove(Board* board, unsigned char from, unsigned char to) { // Function creating new Board instance on next and perfoming move on it
    board->next = new Board(board);
    board->next->move(from,to);
}

void Engine::engineNextMove(Board* board){ // Function playing the move calculated as best by the engine
    //TODO change parameter to a variable and make it easier to adjust, this parameter is the depth of the algorithm //Should be divisible by 2
    Engine::buildFutureGameTree(board, 2);
    Board* best = Evaluation::calcEvalFromBranchTips(board);
    qWarning() << "BoardEval: " << best->getBoardEval();
}

//TODO Maybe add a chance to incease n number on branches with small amounts of moves, idk we'll see how it goes
void Engine::buildFutureGameTree(Board* startingBoard, int n) {
    if (n <= 0) return;

    Board* current = startingBoard;

    for (short from = 0; from < 64; from++) {
        long long moves = 0;

        if (startingBoard->isWhiteMove()) {
            if (startingBoard->getWhitePawns() >> from & 1) moves = Pawn::legalMoves(from, *startingBoard, true);
            else if (startingBoard->getWhiteRooks() >> from & 1) moves = Rook::legalMoves(from, *startingBoard);
            else if (startingBoard->getWhiteKnights() >> from & 1) moves = Knight::legalMoves(from, *startingBoard);
            else if (startingBoard->getWhiteBishops() >> from & 1) moves = Bishop::legalMoves(from, *startingBoard);
            else if (startingBoard->getWhiteQueens() >> from & 1) moves = Rook::legalMoves(from, *startingBoard) | Bishop::legalMoves(from, *startingBoard);
            else if (startingBoard->getWhiteKings() >> from & 1) moves = King::legalMoves(from, *startingBoard);
        }
        else {
            if (startingBoard->getBlackPawns() >> from & 1) moves = Pawn::legalMoves(from, *startingBoard, false);
            else if (startingBoard->getBlackRooks() >> from & 1) moves = Rook::legalMoves(from, *startingBoard);
            else if (startingBoard->getBlackKnights() >> from & 1) moves = Knight::legalMoves(from, *startingBoard);
            else if (startingBoard->getBlackBishops() >> from & 1) moves = Bishop::legalMoves(from, *startingBoard);
            else if (startingBoard->getBlackQueens() >> from & 1) moves = Rook::legalMoves(from, *startingBoard) | Bishop::legalMoves(from, *startingBoard);
            else if (startingBoard->getBlackKings() >> from & 1) moves = King::legalMoves(from, *startingBoard);
        }

        while (moves) {
            short to = __builtin_ctzll(moves);
            moves &= ~(1LL << to);
            Board* temp = new Board(startingBoard);
            temp->move(from, to);
            if (current == startingBoard) {
                current = current->next = temp;
            } else {
                current->right = temp;
                temp->left = current;
                current = temp;
            }
            buildFutureGameTree(current, n - 1);
        }
    }
}


// Main Functions
bool Engine::pressedButton(Board* board, int buttonId, unsigned char* selected, unsigned char* clearSelected, unsigned long long* moves, unsigned long long* clearMoves ) // Reads input and calls most of other functions
{
     *clearSelected =   *selected;
     *clearMoves =  *moves;
    if(board->isOccupied(buttonId) && !board->isEnemyOccupied(buttonId)){
        *selected = buttonId;
        if(board->isWhiteMove()){
            if (board->getWhitePawns()  & 1ULL <<  *selected)    *moves = Pawn::legalMoves(buttonId, *board);
            else if (board->getWhiteRooks()  & 1ULL <<  *selected)    *moves = Rook::legalMoves(buttonId, *board);
            else if (board->getWhiteKnights()  & 1ULL <<  *selected)  *moves = Knight::legalMoves(buttonId, *board);
            else if (board->getWhiteBishops()  & 1ULL <<  *selected)  *moves = Bishop::legalMoves(buttonId, *board);
            else if (board->getWhiteQueens()  & 1ULL <<  *selected)   *moves = Bishop::legalMoves(buttonId, *board) | Rook::legalMoves(buttonId, *board);
            else if (board->getWhiteKings()  & 1ULL <<  *selected)    *moves = King::legalMoves(buttonId, *board);
        }
        else {
            if (board->getBlackPawns() & 1ULL <<  *selected)    *moves = Pawn::legalMoves(buttonId, *board);
            else if (board->getBlackRooks() & 1ULL <<  *selected)    *moves = Rook::legalMoves(buttonId, *board);
            else if (board->getBlackKnights() & 1ULL <<  *selected)  *moves = Knight::legalMoves(buttonId, *board);
            else if (board->getBlackBishops() & 1ULL <<  *selected)  *moves = Bishop::legalMoves(buttonId, *board);
            else if (board->getBlackQueens() & 1ULL <<  *selected)   *moves = Bishop::legalMoves(buttonId, *board) | Rook::legalMoves(buttonId, *board);
            else if (board->getBlackKings() & 1ULL <<  *selected)    *moves = King::legalMoves(buttonId, *board);
        }
    } else{
        // Gdy zostało wciśnięte pole z tablicy ruchu
        if ( *moves & (1ULL << buttonId)) {
            nextMove(board, *selected, buttonId);
            board->lastMove[0] =  *selected; // ustawienie ostatniego ruchu (do sprawdzania en passant)
            board->lastMove[1] = buttonId;
             *moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
             *selected = 64;
            return true;
        }

        // Gdy zostało wciśnięte puste pole
        else {
             *selected = 64;
             *moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
        }
    }

    // Sprawdzanie, czy zmieniła się pozycja, i resetowanie zaznaczenia, jeśli nie
    if (*selected == *clearSelected) {
         *selected = 64;
         *moves = 0b0000000000000000000000000000000000000000000000000000000000000000;
    }
    return false;
}

