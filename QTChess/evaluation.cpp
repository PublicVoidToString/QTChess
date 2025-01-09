#include "evaluation.h"

Evaluation::Evaluation() {
}

int Evaluation::flipSquare(int value) {
    return 63-value;
}

double Evaluation::evaluatePosition(Board* board) { // Main eval function, calculating based on private eval functions

    // Sprawdzenie czy nastąpił mat, pat TODO

    if(board->isBlackMated()) board->setBoardEval(1000);
    else if(board->isWhiteMated()) board->setBoardEval(-1000);
    else if(board->isDraw()) board->setBoardEval(0);
    else {
        int mg_white = 0;
        int eg_white = 0;
        int mg_black = 0;
        int eg_black = 0;
        int gamePhase = 0;

        double eval = 0;
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
                mg_white += mg_king_table[square];
                eg_white += eg_king_table[square];
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
                mg_black += mg_king_table[flippedSquare];
                eg_black += eg_king_table[flippedSquare];
            }
        }

        //TODO pawn structure, king safety etc.

        // tapered evaluation - based on gamephase

        int mgScore;
        int egScore;
        if(board->isWhiteMove()) {
            mgScore = mg_white - mg_black;
            egScore = eg_white - eg_black;
        } else {
            mgScore = mg_black - mg_white;
            egScore = eg_black - eg_white;
        }

        int mgPhase = gamePhase;
        if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
        int egPhase = 24 - mgPhase;
        return (mgScore * mgPhase + egScore * egPhase) / 24;

        board->setBoardEval(eval);
    }

    return board->getBoardEval();
}

// Evaluation functions
unsigned char Evaluation::sumBits(unsigned long long variable)  {
    unsigned char sum = 0;
    unsigned long long buffer = variable;
    for (sum = 0; buffer; sum++)
    {
        buffer &= buffer - 1; // Removing least significant bit
    }
    return sum;
}

double Evaluation::sumWhiteMaterial(Board* board)  {
    long long score = 0;
    score += sumBits(board->getWhitePawns());
    score += sumBits(board->getWhiteBishops())*3;
    score += sumBits(board->getWhiteKnights())*3;
    score += sumBits(board->getWhiteRooks())*5;
    score += sumBits(board->getWhiteQueens())*9;
    return score;
}

double Evaluation::sumBlackMaterial(Board* board)  {
    long long score = 0;
    score += sumBits(board->getBlackPawns());
    score += sumBits(board->getBlackBishops())*3;
    score += sumBits(board->getBlackKnights())*3;
    score += sumBits(board->getBlackRooks())*5;
    score += sumBits(board->getBlackQueens())*9;
    return score;
}


Board* Evaluation::calcEvalFromBranchTips(Board* startingBoard) {

    if (startingBoard == nullptr)
        return nullptr;

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

