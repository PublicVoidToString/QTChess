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

void Engine::engineMove(Board** board, char botDepth) {
    if(isGameEnded(*board)&0b11) return;
    if(botDepth>1) {
        //Engine::minimaxTreeSearch(*board, botDepth);
        Engine::alphaBetaTreeSearch(*board, botDepth, -INFINITY, INFINITY);
        (*board)->next = Engine::getBestMove(*board);
        (*board)=(*board)->next;
        (*board)->cutSideBranches();
        printMoveDebug(*board);
    }
}

// FINISHED /maybe change into smaller functions
bool Engine::madePlayerMove(Board** board, int buttonId, unsigned char* selected, unsigned long long* moves, char botDepth, unsigned short promotion) // Reads input and calls most of other functions
{
    if(isGameEnded(*board)&0b11) return false;
    if(promotion == 5) return false;
    // If pressed the same tile twice
    if (*selected == buttonId) {
        *selected = 64;
        *moves = 0;
        return false;
    }
    // If selected current players piece
    if ((*board)->isOccupied(buttonId) && !(*board)->isEnemyOccupied(buttonId)) {
        *selected = buttonId;
        *moves = getLegalMoves(buttonId, *board);
        return false;
    }
    // If selected is a possible move
    if (*moves & (1ULL << buttonId)) {

        (*board)->next = new Board(*board, *selected, buttonId, promotion);
        (*board) = (*board)->next;
        setIfGameEnded(*board);

        (*board)->cutSideBranches();

        //Reset selection on board
        *moves = 0;
        *selected = 64;
        printMoveDebug(*board);

        return true;
    }
    // If tile not in move list
    else {
        *selected = 64;
        *moves = 0;

        return false;
    }
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

bool Engine::hasLegalMoves(Board* board) {
    for (short from = 0; from < 64; from++) {
        if(board->isOccupied(from) && !board->isEnemyOccupied(from)){
            long long moves = getLegalMoves(from, board);
            if(moves != 0) {
                return true;
            }
        }
    }

    return false;
}

// TODO Check & Fix
unsigned long long Engine::getLegalMoves(short from, Board *startingBoard) {
    unsigned long long moves = startingBoard->getMoves(from);
    unsigned long long legalMoves = 0ULL;
    Board *newBoard = nullptr;

    for (short to = 0; to < 64; ++to) {
        if (moves & (1ULL << to)) {
            newBoard = new Board(startingBoard, from, to,0,false);
            newBoard->increaseTurnNumber();

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
    qWarning() << "All calculated boards: " << allCount;
    qWarning() << "Boards in memory: " << (startingBoard)->existingBoards;
    qWarning() << "Current board evaluation (taking depth into consideration) " <<(startingBoard)->getBoardEval();
    qWarning() << "Current board static evaluation (not taking depth into account)";
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
    } else if(!startingBoard->isPossibleMove()){ // stalemate
        startingBoard->setBoardEval(0);
        return;
    }

    if (n <= 0) {
        Evaluation::evaluatePosition(startingBoard);
        return;
    }

    Board* current = startingBoard;
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
                    }

                } else {
                    if(bestValue > temp->getBoardEval()) {
                        bestValue = temp->getBoardEval();
                    }
                }

            } while(promotion-->0);
        }
    }

    startingBoard->setBoardEval(bestValue);
}

void Engine::alphaBetaTreeSearch (Board* startingBoard, int n, double alpha, double beta) {

    if (startingBoard->getBlackCheckmate()) {
        startingBoard->setBoardEval(10000+n); // adding n --> makes sure that the engine goes for the quicker checkmate
        return;
    } else if (startingBoard->getWhiteCheckmate()){
        startingBoard->setBoardEval(-10000-n);
        return;
    } else if(!startingBoard->isPossibleMove()){ // stalemate
        startingBoard->setBoardEval(0);
        return;
    }

    if (n <= 0) {
        Evaluation::evaluatePosition(startingBoard);
        return;
    }

    Board* tail = nullptr;

    // getting all moves in and ordering them by orderScore
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

                if (startingBoard->next == nullptr) {
                    startingBoard->next = temp;
                    tail = temp;
                } else {
                    if (temp->getOrderingScore() > 0) {
                        // Insert from front (for captures and queen promotions)
                        Board* current = startingBoard->next;
                        while (current != nullptr && temp->getOrderingScore() < current->getOrderingScore()) {
                            current = current->right;
                        }
                        if (current != nullptr) {
                            temp->right = current;
                            temp->left = current->left;
                            if (current->left) current->left->right = temp;
                            current->left = temp;
                            if (current == startingBoard->next) startingBoard->next = temp;
                        } else {
                            tail->right = temp;
                            temp->left = tail;
                            tail = temp;
                        }
                    } else {
                        // Insert from the back (for quiet moves and underpromotions)
                        Board* current = tail;
                        while (current != nullptr && temp->getOrderingScore() > current->getOrderingScore()) {
                            current = current->left;
                        }
                        if (current != nullptr) {
                            temp->left = current;
                            temp->right = current->right;
                            if (current->right) current->right->left = temp;
                            current->right = temp;
                            if (current == tail) tail = temp;
                        } else {
                            temp->right = startingBoard->next;
                            startingBoard->next->left = temp;
                            startingBoard->next = temp;
                        }
                    }

                }

            } while (promotion --> 0);
        }
    }

    double bestValue = startingBoard->isWhiteMove() ? -INFINITY : INFINITY;

    // evaluation
    for(Board* current=startingBoard->next;current!=nullptr;current=current->right){

        alphaBetaTreeSearch(current, n - 1, alpha, beta);

        if (startingBoard->isWhiteMove()) {
            if(bestValue < current->getBoardEval()) {
                bestValue = current->getBoardEval();
            }
            alpha = std::max(alpha, bestValue);

        } else {
            if(bestValue > current->getBoardEval()) {
                bestValue = current->getBoardEval();
            }
            beta = std::min(beta, bestValue);
        }

        // Pruning condition
        if (beta <= alpha) {
            current->cutRightBranches();
            break;
        }
    }

    startingBoard->setBoardEval(bestValue);
}

// FINISHED
short Engine::isGameEnded(Board* startingBoard) {
    //return 1 - black win, return 2 - white win, return 3 draw, return anything else - game continuous
    return startingBoard->getGameState();
}

bool Engine::setIfGameEnded(Board* startingBoard){
    return startingBoard->getWhiteCheckmate() || startingBoard->getBlackCheckmate() || startingBoard->isPossibleMove();
}

