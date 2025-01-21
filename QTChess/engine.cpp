#include "engine.h"
#include "evaluation.h"
#include "pawn.h"
#include "bishop.h"
#include "knight.h"
#include "rook.h"
#include "king.h"

Engine::Engine() {}

long prevcount=0;
long count=0;
// Engine Functions
void Engine::nextMove(Board* board, unsigned char from, unsigned char to) { // Function creating new Board instance on next and perfoming move on it
    if(board->next!=nullptr){
        Board *comp = new Board(board);
        comp->move(from,to);
        for(Board *nextMove=board->next;nextMove!=nullptr;nextMove=nextMove->right){
            if(*nextMove==*comp){
                board->next=nextMove;
                board->next->cutSideBranches();
                delete comp;
                return;
            }
            if(nextMove->right==nullptr) qWarning()<< "Somethings wrong Engine.cpp line 27";
        }
        delete comp;
    }
    else {
        board->next = new Board(board);
        board->next -> move(from,to);
    }
}

Board* Engine::engineNextMove(Board* board,char botDepth){ // Function playing the move calculated as best by the engine
    count = 0;
    Engine::buildFutureGameTree(board, botDepth);
    prevcount+=count;
    Evaluation::calcEvalFromBranchTips(board);
    Board* best = Engine::getBestMove(board);
    best->cutSideBranches();

    return best;
}

Board* Engine::getBestMove(Board* startingBoard){
    Board* best=nullptr;
    if (startingBoard->next == nullptr) {
        QMessageBox::critical(nullptr, "Error", "The next board is null. The application will close.");
        QCoreApplication::quit();
    }
    for(Board* current=startingBoard->next;current->right!=nullptr;current=current->right){
        if(best==nullptr) best=current;
        else if(startingBoard->isWhiteMove()){
            if(best->getBoardEval()<current->getBoardEval()){
                best=current;
            }
        }else{
            if(best->getBoardEval()>current->getBoardEval()){
                best=current;
            }
        }
    }
    return best;
}

//TODO Maybe add a chance to incease n number on branches with small amounts of moves, idk we'll see how it goes
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
        long long moves = getLegalMoves(from, startingBoard);
        while (moves) {
            short to = __builtin_ctzll(moves);
            moves &= ~(1LL << to);
            Board* temp = new Board(startingBoard);
            count++;
            temp->move(from, to);
            if (current == startingBoard) {
                current = current->next = temp;
            } else {
                current->right = temp;
                temp->left = current;
                current = temp;
            }
            //Don't delete temp, as temporary is only the pointer and not it's destination
            buildFutureGameTree(current, n - 1);
        }
    }
}

// Main Functions
void Engine::pressedButton(Board** board, int buttonId, unsigned char* selected, unsigned char* clearSelected, unsigned long long* moves, unsigned long long* clearMoves, char botDepth) // Reads input and calls most of other functions
{
    *clearSelected = *selected;
    *clearMoves = *moves;

    // If position didn't change
    if (*selected == buttonId) {
        *selected = 64;
        *moves = 0;
        return;
    }

    if ((*board)->isOccupied(buttonId) && !(*board)->isEnemyOccupied(buttonId)) {
        *selected = buttonId;
        *moves = getLegalMoves(buttonId, *board);
    } else {
        if (*moves & (1ULL << buttonId)) {
            nextMove(*board, *selected, buttonId);
            (*board)->setLastMoveFrom(*selected);
            (*board)->setLastMoveTo(buttonId);
            // When against player
            if(botDepth==0){
                Engine::buildFutureGameTree(*board, 2);
                Evaluation::calcEvalFromBranchTips(*board);
                *board = (*board)->next;
            // When against bot
            } else{
                *board = (*board)->next;
                (*board)->next = engineNextMove(*board,botDepth);
                *board = (*board)->next;
            }
            *moves = 0;
            *selected = 64;
            printMoveDebug(*board);
        }
        // Gdy zostało wciśnięte puste pole
        else {
            *selected = 64;
            *moves = 0;
        }
    }
}

unsigned long long Engine::getLegalMoves(short from, Board *startingBoard) {
    unsigned long long moves = startingBoard->getMoves(from);
    unsigned long long legalMoves = 0ULL;

    for (short to = 0; to < 64; ++to) {
        if (moves & (1ULL << to)) {
            Board *newBoard = new Board(startingBoard);
            newBoard->setTurnNumber(startingBoard->getTurnNumber());
            newBoard->move(from, to);

            short kingPosition = newBoard->isWhiteMove()
                                     ? __builtin_ctzll(newBoard->getWhiteKings())
                                     : __builtin_ctzll(newBoard->getBlackKings());

            if (!newBoard->isAttacked(kingPosition, startingBoard->isWhiteMove())) {
                legalMoves |= (1ULL << to);
            }

            delete newBoard;
        }
    }

    return legalMoves;
}

void Engine::printMoveDebug(Board* startingBoard){
    printPossibleMoveCount(startingBoard);
    qWarning() << "All calculated boards: " << prevcount;
    qWarning() << "New calculated boards: " << count;
    (startingBoard)->printRootLength();
    qWarning() << "Boards in memory: " << (startingBoard)->existingBoards;
    qWarning() << "Current board evaluation " <<(startingBoard)->getBoardEval();
    (startingBoard)->printLastMove();
    qWarning() << "-------------------------------";
}

int Engine::printPossibleMoveCount(Board* startingBoard, int count) {
    if (!startingBoard) {
        return count;
    }

    int temp = count + 1;

    if (startingBoard->right) {
        temp = printPossibleMoveCount(startingBoard->right, temp);
    }
    if (startingBoard->next) {
        temp = printPossibleMoveCount(startingBoard->next, temp);
    }
    if (count == -1) { qWarning() << "All Calculated Possible Moves: " << temp; }
    return temp;
}
