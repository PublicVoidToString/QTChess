#include "evaluation.h"

Evaluation::Evaluation() {
}


double Evaluation::evaluatePosition(Board* board) { // Main eval function, calculating based on private eval functions

    // Sprawdzenie czy nastąpił mat, pat TODO

    if(board->isBlackMated()) board->setBoardEval(1000);
    else if(board->isWhiteMated()) board->setBoardEval(-1000);
    else if(board->isDraw()) board->setBoardEval(0);
    else {
        double eval = 0;
        //Material Points
        eval += sumWhiteMaterial(board);
        eval -= sumBlackMaterial(board);
        //TODO Everything else

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
long long Evaluation::sumWhiteMaterial(Board* board)  {
    long long score = 0;
    score += sumBits(board->getWhitePawns());
    score += sumBits(board->getWhiteBishops())*3;
    score += sumBits(board->getWhiteKnights())*3;
    score += sumBits(board->getWhiteRooks())*5;
    score += sumBits(board->getWhiteQueens())*9;
    score += sumBits(board->getWhiteKings())*1000;
    return score;
}

long long Evaluation::sumBlackMaterial(Board* board)  {
    long long score = 0;
    score += sumBits(board->getBlackPawns());
    score += sumBits(board->getBlackBishops())*3;
    score += sumBits(board->getBlackKnights())*3;
    score += sumBits(board->getBlackRooks())*5;
    score += sumBits(board->getBlackQueens())*9;
    score += sumBits(board->getBlackKings())*1000;
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

