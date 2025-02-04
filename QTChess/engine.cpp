#include "engine.h"
#include "evaluation.h"
#include <QtConcurrent/QtConcurrent>

Engine::Engine() {}
long allCount=0;

bool Engine::isPromotion(Board* board, int buttonId, unsigned char selected, unsigned long long moves){
    if(selected==64) return false;
    if(!(moves & (1ULL<<buttonId))) return false;
    if(board->isWhiteMove()){
        if(buttonId<56) return false;
        if(board->getWhitePawns() & (1ULL<<selected)) return true;
    }else{
        if(buttonId>7) return false;
        if(board->getBlackPawns() & (1ULL<<selected)) return true;
    }
    return false;
}



// FINISHED /maybe change into smaller functions
void Engine::pressedButton(Board** board, int buttonId, unsigned char* selected, unsigned long long* moves, char botDepth, unsigned short promotion) // Reads input and calls most of other functions
{
    if(isGameEnded(*board)&0b11) return;
    if(promotion == 5) return;
    // If pressed the same tile twice
    if (*selected == buttonId) {
        *selected = 64;
        *moves = 0;
        return;
    }
    // If selected current players piece
    if ((*board)->isOccupied(buttonId) && !(*board)->isEnemyOccupied(buttonId)) {
        *selected = buttonId;
        *moves = getLegalMoves(buttonId, *board);
        return;
    }
    // If selected is a possible move
    if (*moves & (1ULL << buttonId)) {
        if((*board)->next==nullptr) {
            QMessageBox::critical(nullptr, "Error", "Next Board does not exist!! (ERROR EK01)");
            QCoreApplication::quit();
        }
        findAndApplyMove(board, *selected, buttonId, promotion);
        (*board)->cutSideBranches();

        if(isGameEnded(*board)&0b11) return;
        if(botDepth>1){
            printMoveDebug(*board);
            //Engine::minimaxTreeSearch(*board, botDepth);
            Engine::alphaBetaTreeSearch(*board, botDepth, -INFINITY, INFINITY);
            (*board)->next = Engine::getBestMove(*board);
            (*board)=(*board)->next;
            (*board)->cutSideBranches();
        }

        Engine::minimaxTreeSearch(*board, 1);

        //QFuture<void> future = QtConcurrent::run([=]() {
        //    Engine::buildFutureGameTree(*board, botDepth);
        //    Evaluation::calcEvalFromBranchTips(*board);
        //});

        //Reset selection on board
        *moves = 0;
        *selected = 64;
        printMoveDebug(*board);
    }
    // If tile not in move list
    else {
        *selected = 64;
        *moves = 0;
    }
}

// FINISHED
void Engine::findAndApplyMove(Board** board, unsigned char from, unsigned char to, unsigned short promotion) { // Function creating new Board instance on next and perfoming move on it
    for(Board *comparator = (*board)->next;comparator!=nullptr;comparator=comparator->right){
        if(comparator->getLastMoveFrom() == from && comparator->getLastMoveTo() == to && comparator->getLastMovePromotion() == promotion){
            (*board)->next=comparator;
            (*board)=(*board)->next;
            comparator->cutSideBranches();
            return;
        }
    }
    QMessageBox::critical(nullptr, "Error", "Move coudn't be found!! (ERROR EK02)");
    QCoreApplication::quit();
}

// FINISHED
Board* Engine::getBestMove(Board* startingBoard){
    Board* best=nullptr;
    if (startingBoard->next == nullptr) {
        QMessageBox::critical(nullptr, "Error", "Next Board does not exist in engine!! (ERROR EK03)");
        QCoreApplication::quit();
    }

    for(Board* current=startingBoard->next;current!=nullptr;current=current->right){
        if(best==nullptr) best=current;
        else if(startingBoard->isWhiteMove()){
            if(current->getBoardEval() > best->getBoardEval()){ best=current; }
        }else{
            if(current->getBoardEval() < best->getBoardEval()){ best=current; }
        }
    }
    return best;
}

// Builds game tree without evaluating - used mainly for 1 depth (players move)

void Engine::buildFutureGameTree(Board* startingBoard, int n) {
    if (n <= 0) return;
    Board* current = startingBoard;
    if (current->next!=nullptr){
        for(current=current->next;current!=nullptr;current=current->right){
            buildFutureGameTree(current, n - 1);
        }
        return;
    }
    for (short from = 0; from < 64; from++) {
        if(startingBoard->isOccupied(from) && !startingBoard->isEnemyOccupied(from)){
            long long moves = getLegalMoves(from, startingBoard);
            short promotion;
            while (moves) {
                short to = __builtin_ctzll(moves);
                moves &= ~(1LL << to);
                if(isPromotion(startingBoard,to,from,UINT64_MAX)) promotion = 4;
                else promotion = 0;
                do{
                    Board* possibleMove = new Board(startingBoard, from, to, promotion,false);
                    allCount++;
                    if (current == startingBoard) {
                        current = current->next = possibleMove;
                    } else {
                        current->right = possibleMove;
                        possibleMove->left = current;
                        current = possibleMove;
                    }

                    //ALFA BETA GOES HERE


                    buildFutureGameTree(current, n - 1);


                } while(promotion-->0);
            }
        }
    }
}


// TODO Check & Fix
unsigned long long Engine::getLegalMoves(short from, Board *startingBoard) {
    unsigned long long moves = startingBoard->getMoves(from);
    unsigned long long legalMoves = 0ULL;
    Board *newBoard = nullptr;

    for (short to = 0; to < 64; ++to) {
        if (moves & (1ULL << to)) {
            newBoard = new Board(startingBoard, from, to,0,false);
            newBoard->setTurnNumber(startingBoard->getTurnNumber());

            short kingPosition = newBoard->getKingPosition();
            if (!newBoard->isAttacked(kingPosition, startingBoard->isWhiteMove())) { legalMoves |= (1ULL << to); }

            delete newBoard;
        }
    }

    return legalMoves;
}


// FINISHED
int Engine::printPossibleMoveCount(Board* startingBoard, int count) {
    if (!startingBoard) { return count; }
    int temp = count + 1;
    if (startingBoard->right) { temp = printPossibleMoveCount(startingBoard->right, temp); }
    if (startingBoard->next) { temp = printPossibleMoveCount(startingBoard->next, temp); }
    if (count == -1) { qWarning() << "All Calculated Possible Moves: " << temp; }
    return temp;
}

// FINISHED
void Engine::printMoveDebug(Board* startingBoard){
    printPossibleMoveCount(startingBoard);
    (startingBoard)->printRootLength();
    qWarning() << "All calculated boards: " << allCount;
    qWarning() << "Boards in memory: " << (startingBoard)->existingBoards;
    qWarning() << "Current board evaluation " <<(startingBoard)->getBoardEval();
    qWarning() << "PST board eval: " <<
        Evaluation::pieceSquareTables(startingBoard) <<
        "Pawn structure evaluation: " <<
        Evaluation::pawnStructure(startingBoard) <<
        "Piece development evaluation: " <<
        Evaluation::pieceDevelopmentEvaluation(startingBoard);
    (startingBoard)->printLastMove();
    qWarning() << "Last move promotion: " << (startingBoard)->getLastMovePromotion();
    switch(isGameEnded(startingBoard)){
    case 1:
        qWarning() << "Game state: Black Won";
        break;
    case 2:
        qWarning() << "Game state: White Won";
        break;
    case 3:
        qWarning() << "Game state: Draw";
        break;
    default:
        qWarning() << "Game state: During Game";

    }

    qWarning() << "-------------------------------";
}

void Engine::minimaxTreeSearch (Board* startingBoard, int n) {

    if (startingBoard->getBlackCheckmate()) {
        startingBoard->setBoardEval(10000+n); // adding n --> makes sure that the engine goes for the quicker checkmate
        return;
    } else if (startingBoard->getWhiteCheckmate()){
        startingBoard->setBoardEval(-10000-n);
        return;
    } else if (false) {      // TODO remis
        startingBoard->setBoardEval(0);
        return;
    }

    if (n <= 0) {
        Evaluation::evaluatePosition(startingBoard);
        return;
    }

    Board* current = startingBoard;
    Board* bestBoard = nullptr;
    double bestValue = startingBoard->isWhiteMove() ? -INFINITY : INFINITY;

    for (short from = 0; from < 64; from++) {
        long long moves = getLegalMoves(from, startingBoard);
        while (moves) {
            short to = __builtin_ctzll(moves);
            moves &= ~(1LL << to);
            short promotion = 0;
            if(isPromotion(startingBoard,to,from,UINT64_MAX)) promotion = 4;
            do{
                Board* temp = new Board(startingBoard, from, to, promotion,false);
                allCount++;
                if (current == startingBoard) {
                    current = current->next = temp;
                } else {
                    current->right = temp;
                    temp->left = current;
                    current = temp;
                }
                //Don't delete temp, as temporary is only the pointer and not it's destination
                minimaxTreeSearch(current, n - 1);

                if (startingBoard->isWhiteMove()) {
                    if(bestValue < temp->getBoardEval()) {
                        bestValue = temp->getBoardEval();
                        bestBoard = temp;
                    }

                } else {
                    if(bestValue > temp->getBoardEval()) {
                        bestValue = temp->getBoardEval();
                        bestBoard = temp;
                    }
                }

            } while(promotion-->0);
        }
    }
    //bestBoard->cutAllBranches();
    //startingBoard->next = bestBoard;
    startingBoard->setBoardEval(bestValue);
}

void Engine::alphaBetaTreeSearch (Board* startingBoard, int n, double alpha, double beta) {

    if (startingBoard->getBlackCheckmate()) {
        startingBoard->setBoardEval(10000+n); // adding n --> makes sure that the engine goes for the quicker checkmate
        return;
    } else if (startingBoard->getWhiteCheckmate()){
        startingBoard->setBoardEval(-10000-n);
        return;
    } else if (false) {      // TODO remis
        startingBoard->setBoardEval(0);
        return;
    }

    if (n <= 0) {
        Evaluation::evaluatePosition(startingBoard);
        return;
    }

    Board* current = startingBoard;
    Board* bestBoard = nullptr;
    double bestValue = startingBoard->isWhiteMove() ? -INFINITY : INFINITY;

    for (short from = 0; from < 64; from++) {
        long long moves = getLegalMoves(from, startingBoard);
        while (moves) {
            short to = __builtin_ctzll(moves);
            moves &= ~(1LL << to);
            short promotion = 0;
            if (isPromotion(startingBoard, to, from, UINT64_MAX)) promotion = 4;

            do {
                Board* temp = new Board(startingBoard, from, to, promotion, false);
                allCount++;

                if (current == startingBoard) {
                    current = current->next = temp;
                } else {
                    current->right = temp;
                    temp->left = current;
                    current = temp;
                }
                alphaBetaTreeSearch(current, n - 1, alpha, beta);

                if (startingBoard->isWhiteMove()) {
                    if(bestValue < temp->getBoardEval()) {
                        bestValue = temp->getBoardEval();
                        bestBoard = temp;
                    }
                    alpha = std::max(alpha, bestValue);

                } else {
                    if(bestValue > temp->getBoardEval()) {
                        bestValue = temp->getBoardEval();
                        bestBoard = temp;
                    }
                    beta = std::min(beta, bestValue);
                }

                // Pruning condition
                if (beta <= alpha) {
                    break;
                }

            } while (promotion --> 0);

            // Pruning condition
            if (beta <= alpha) {
                break;
            }
        }
    }
    //bestBoard->cutAllBranches();
    //startingBoard->next = bestBoard;
    startingBoard->setBoardEval(bestValue);
}

// FINISHED
short Engine::isGameEnded(Board* startingBoard) {
    //return 1 - black win, return 2 - white win, return 3 draw, return anything else - game continuous
    return startingBoard->getGameState() >> 2;
}

bool Engine::setIfGameEnded(Board* startingBoard){
    return startingBoard->getWhiteCheckmate() || startingBoard->getBlackCheckmate();
}

