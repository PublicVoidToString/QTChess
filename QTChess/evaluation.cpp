#include "evaluation.h"
#include <QMessageBox>

Evaluation::Evaluation() {
}

int Evaluation::flipSquare(int value) {
    return 63-value;
}

double Evaluation::evaluatePosition(Board* board) { // Main eval function, calculating based on private eval functions

    // Sprawdzenie czy nastąpił mat, pat TODO

    if(board->getWhiteCheckmate()) {
        board->setBoardEval(-1000);
    }
    else if(board->getBlackCheckmate()) {
        board->setBoardEval(1000);
    }
    else if(false) board->setBoardEval(0);
    else {
        double mg_white = 0;
        double eg_white = 0;
        double mg_black = 0;
        double eg_black = 0;
        int gamePhase = 0;

        //Material Points and basic positional heuristics
        for (int square = 0; square < 64; square++) {
            if (board->getWhitePawns() & (1ULL << square)) {
                mg_white += mg_pawn_table[square] + mg_pawn_value;
                eg_white += eg_pawn_table[square] + eg_pawn_value;
                gamePhase += pawn_gamephaseIncrease;
                continue;
            }
            if (board->getWhiteKnights() & (1ULL << square)) {
                mg_white += mg_knight_table[square] + mg_knight_value;
                eg_white += eg_knight_table[square] + eg_knight_value;
                gamePhase += knight_gamephaseIncrease;
                continue;
            }
            if (board->getWhiteBishops() & (1ULL << square)) {
                mg_white += mg_bishop_table[square] + mg_bishop_value;
                eg_white += eg_bishop_table[square] + eg_bishop_value;
                gamePhase += bishop_gamephaseIncrease;
                continue;
            }
            if (board->getWhiteRooks() & (1ULL << square)) {
                mg_white += mg_rook_table[square] + mg_rook_value;
                eg_white += eg_rook_table[square] + eg_rook_value;
                gamePhase += rook_gamephaseIncrease;
                continue;
            }
            if (board->getWhiteQueens() & (1ULL << square)) {
                mg_white += mg_queen_table[square] + mg_queen_value;
                eg_white += eg_queen_table[square] + eg_queen_value;
                gamePhase += queen_gamephaseIncrease;
                continue;
            }
            if (board->getWhiteKings() & (1ULL << square)) {
                mg_white += mg_king_table[square] + king_value;
                eg_white += eg_king_table[square] + king_value;
                continue;
            }

            int flippedSquare = flipSquare(square);

            if (board->getBlackPawns() & (1ULL << square)) {
                mg_black += mg_pawn_table[flippedSquare] + mg_pawn_value;
                eg_black += eg_pawn_table[flippedSquare] + eg_pawn_value;
                gamePhase += pawn_gamephaseIncrease;
                continue;
            }
            if (board->getBlackKnights() & (1ULL << square)) {
                mg_black += mg_knight_table[flippedSquare] + mg_knight_value;
                eg_black += eg_knight_table[flippedSquare] + eg_knight_value;
                gamePhase += knight_gamephaseIncrease;
                continue;
            }
            if (board->getBlackBishops() & (1ULL << square)) {
                mg_black += mg_bishop_table[flippedSquare] + mg_bishop_value;
                eg_black += eg_bishop_table[flippedSquare] + eg_bishop_value;
                gamePhase += bishop_gamephaseIncrease;
                continue;
            }
            if (board->getBlackRooks() & (1ULL << square)) {
                mg_black += mg_rook_table[flippedSquare] + mg_rook_value;
                eg_black += eg_rook_table[flippedSquare] + eg_rook_value;
                gamePhase += rook_gamephaseIncrease;
                continue;
            }
            if (board->getBlackQueens() & (1ULL << square)) {
                mg_black += mg_queen_table[flippedSquare] + mg_queen_value;
                eg_black += eg_queen_table[flippedSquare] + eg_queen_value;
                gamePhase += queen_gamephaseIncrease;
                continue;
            }
            if (board->getBlackKings() & (1ULL << square)) {
                mg_black += mg_king_table[flippedSquare] + king_value;
                eg_black += eg_king_table[flippedSquare] + king_value;
            }
        }

        //TODO pawn structure, king safety etc.

        // tapered evaluation - based on gamephase

        double mgScore;
        double egScore;

        mgScore = mg_white - mg_black;
        egScore = eg_white - eg_black;

        int mgPhase = gamePhase;
        if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
        int egPhase = 24 - mgPhase;

        board->setBoardEval((mgScore * mgPhase + egScore * egPhase) / 24.0);
    }

    return board->getBoardEval();
}

Board* Evaluation::calcEvalFromBranchTips(Board* startingBoard) {

    if (startingBoard->next != nullptr) {
        if (startingBoard->isWhiteMove()) {
            startingBoard->setBoardEval(-1001);
            for (Board* current = startingBoard->next; current != nullptr; current = current->right) {
                calcEvalFromBranchTips(current);
                if (startingBoard->getBoardEval() < current->getBoardEval())
                    startingBoard->setBoardEval(current->getBoardEval());
            }
        } else {
            startingBoard->setBoardEval(1001);
            for (Board* current = startingBoard->next; current != nullptr; current = current->right) {
                calcEvalFromBranchTips(current);
                if (startingBoard->getBoardEval() > current->getBoardEval())
                    startingBoard->setBoardEval(current->getBoardEval());
            }
        }
    } else {
        Evaluation::evaluatePosition(startingBoard);
    }

    return startingBoard;
}

//TODO Add Alpha Beta pruning, adjust minmaxing
Board* Evaluation::getBestBranchFromGT(Board* startingBoard){
    if(startingBoard==nullptr || startingBoard->next==nullptr) return nullptr;
    Board* best = nullptr;
    for(Board* current = startingBoard->next; current->right!=nullptr;current=current->right){
        //Evaluate board position based on Game Tree
        Board* currentBranch = getBestBranchFromGT(current);
        if(currentBranch != nullptr) current->setBoardEval(currentBranch->getBoardEval());

        if(current != nullptr && ( best == nullptr || best->getBoardEval() < current->getBoardEval() )) best=current;
    }
    return best;
}

