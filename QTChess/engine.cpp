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
    }
    else {
        board->next = new Board(board);
        board->next -> move(from,to);
    }
}

Board* Engine::engineNextMove(Board* board,char botDepth){ // Function playing the move calculated as best by the engine
    count = 0;
    Engine::buildFutureGameTree(board, botDepth);
    qWarning() << "All calculated boards: " << prevcount;
    qWarning() << "New calculated boards: " << count;
    qWarning() << "Boards in memory: " << board->existingBranches;
    prevcount+=count;
    Evaluation::calcEvalFromBranchTips(board);
    Board* best = Engine::getBestMove(board);
    best->cutSideBranches();

    board->printRootLength();
    qWarning() << "Current board evaluation " <<board->getBoardEval();
    qWarning() << "-------------------------------";
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
            count++;
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
        *moves = (*board)->getMoves(buttonId);
    } else {
        if (*moves & (1ULL << buttonId)) {
            nextMove(*board, *selected, buttonId);

            (*board)->setLastMoveFrom(*selected);
            (*board)->setLastMoveTo(buttonId);
            // When against player
            if(botDepth==0){
                *board = (*board)->next;
            // When against bot
            } else{
                *board = (*board)->next;
                (*board)->next = engineNextMove(*board,botDepth);
                *board = (*board)->next;
            }
            *moves = 0;
            *selected = 64;

            (*board)->printLastMove();
        }
        // Gdy zostało wciśnięte puste pole
        else {
            *selected = 64;
            *moves = 0;
        }
    }

}

