#include "evaluation.h"
#include <QDebug>

Evaluation::Evaluation() {
}

int Evaluation::flipSquare(int value) {
    return value ^ 56;
}

double Evaluation::evaluatePosition(Board* board) { // Main eval function, calculating based on private eval functions

    // Sprawdzenie czy nastąpił mat, pat TODO

    if(board->isBlackMated()) board->setBoardEval(1000);
    else if(board->isWhiteMated()) board->setBoardEval(-1000);
    else if(board->isDraw()) board->setBoardEval(0);
    else {
        double evaluation = 0;
        double pstBoardsEvaluation = pieceSquareTables(board);
        double pawnStructureEvaluation = pawnStructure(board);
        // qWarning() << "PST pesto eval: " << pstBoardsEvaluation;
        // qWarning() << "Pawn structure evaluation: " << pawnStructureEvaluation;

        evaluation = pstBoardsEvaluation + pawnStructureEvaluation;

        if (board->getTurnNumber() < openingPhaseMoveCount) {
            evaluation += pieceDevelopmentEvaluation(board);
        }

        board->setBoardEval(evaluation);
    }

    return board->getBoardEval();
}

double Evaluation::pieceSquareTables(Board* board) {

    double mg_white = 0;
    double eg_white = 0;
    double mg_black = 0;
    double eg_black = 0;
    int gamePhase = 0;


    //Material Points and basic positional heuristics
    for (int i = 0; i < 64; i++) {
        int square = flipSquare(i);
        if (board->getWhitePawns() & (1ULL << i)) {
            mg_white += mg_pawn_table[square] + mg_pawn_value;
            eg_white += eg_pawn_table[square] + eg_pawn_value;
            gamePhase += pawn_gamephaseIncrease;
            continue;
        }
        if (board->getWhiteKnights() & (1ULL << i)) {
            mg_white += mg_knight_table[square] + mg_knight_value;
            eg_white += eg_knight_table[square] + eg_knight_value;
            gamePhase += knight_gamephaseIncrease;
            continue;
        }
        if (board->getWhiteBishops() & (1ULL << i)) {
            mg_white += mg_bishop_table[square] + mg_bishop_value;
            eg_white += eg_bishop_table[square] + eg_bishop_value;
            gamePhase += bishop_gamephaseIncrease;
            continue;
        }
        if (board->getWhiteRooks() & (1ULL << i)) {
            mg_white += mg_rook_table[square] + mg_rook_value;
            eg_white += eg_rook_table[square] + eg_rook_value;
            gamePhase += rook_gamephaseIncrease;
            continue;
        }
        if (board->getWhiteQueens() & (1ULL << i)) {
            mg_white += mg_queen_table[square] + mg_queen_value;
            eg_white += eg_queen_table[square] + eg_queen_value;
            gamePhase += queen_gamephaseIncrease;
            continue;
        }
        if (board->getWhiteKings() & (1ULL << i)) {
            mg_white += mg_king_table[square] + king_value;
            eg_white += eg_king_table[square] + king_value;
            continue;
        }

        square = flipSquare(square);

        if (board->getBlackPawns() & (1ULL << i)) {
            mg_black += mg_pawn_table[square] + mg_pawn_value;
            eg_black += eg_pawn_table[square] + eg_pawn_value;
            gamePhase += pawn_gamephaseIncrease;
            continue;
        }
        if (board->getBlackKnights() & (1ULL << i)) {
            mg_black += mg_knight_table[square] + mg_knight_value;
            eg_black += eg_knight_table[square] + eg_knight_value;
            gamePhase += knight_gamephaseIncrease;
            continue;
        }
        if (board->getBlackBishops() & (1ULL << i)) {
            mg_black += mg_bishop_table[square] + mg_bishop_value;
            eg_black += eg_bishop_table[square] + eg_bishop_value;
            gamePhase += bishop_gamephaseIncrease;
            continue;
        }
        if (board->getBlackRooks() & (1ULL << i)) {
            mg_black += mg_rook_table[square] + mg_rook_value;
            eg_black += eg_rook_table[square] + eg_rook_value;
            gamePhase += rook_gamephaseIncrease;
            continue;
        }
        if (board->getBlackQueens() & (1ULL << i)) {
            mg_black += mg_queen_table[square] + mg_queen_value;
            eg_black += eg_queen_table[square] + eg_queen_value;
            gamePhase += queen_gamephaseIncrease;
            continue;
        }
        if (board->getBlackKings() & (1ULL << i)) {
            mg_black += mg_king_table[square] + king_value;
            eg_black += eg_king_table[square] + king_value;
        }
    }

    // tapered evaluation - based on gamephase

    double mgScore;
    double egScore;

    mgScore = mg_white - mg_black;
    egScore = eg_white - eg_black;

    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
    int egPhase = 24 - mgPhase;
    return (mgScore * mgPhase + egScore * egPhase) / 24.0;

}

unsigned long long Evaluation::leftShift(unsigned long long bitboard)
{
    return (bitboard << 1) & ~fileA;
}

unsigned long long Evaluation::rightShift(unsigned long long bitboard)
{
    return (bitboard >> 1) & ~fileH;
}

unsigned long long Evaluation::upShift(unsigned long long bitboard)
{
    return (bitboard << 8) & ~rank1;
}

unsigned long long Evaluation::downShift(unsigned long long bitboard)
{
    return (bitboard >> 8) & ~rank8;
}

unsigned int Evaluation::sumBits(unsigned long long bitboard) {
    unsigned int sum = 0;
    unsigned long long buffer = bitboard;
    for (sum = 0; buffer; sum++)
    {
        buffer &= buffer - 1; // Usunięcie najmniej znaczącego bitu
    }
    return sum;
}

unsigned int Evaluation::isolatedPawnCount(unsigned long long bitboard) {

    int isolatedPawnCount = 0;
    for (int file = 0; file <8; file++) {
        unsigned int count = sumBits((fileA << file) & bitboard);
        if(count > 0) {
            if(!(leftShift(fileA << file) & bitboard) && !(rightShift(fileA << file) & bitboard)) {
                isolatedPawnCount+=count;
            }
        }
    }

    return isolatedPawnCount;
}

unsigned int Evaluation::doubledPawnCount(unsigned long long bitboard) {
    unsigned int doubledPawnCount = 0;
    for(int file = 0; file < 8; file++) {
        unsigned int count = sumBits(bitboard & (fileA << file));
        if (count > 1) {
            doubledPawnCount += count-1;
        }
    }
    return doubledPawnCount;
}

int Evaluation::passedPawnCount(unsigned long long whitePawns, unsigned long long blackPawns) {

    // passed pawns

    uint64_t blackAreaOfControl = downShift(blackPawns | leftShift(blackPawns) | rightShift(blackPawns));
    uint64_t whiteAreaOfControl = upShift(whitePawns | leftShift(whitePawns) | rightShift(whitePawns)) ;

    for(int i=0; i<6; i++) {
        blackAreaOfControl |= downShift(blackAreaOfControl | leftShift(blackAreaOfControl) | rightShift(blackAreaOfControl));
        whiteAreaOfControl |= upShift(whiteAreaOfControl | leftShift(whiteAreaOfControl) | rightShift(whiteAreaOfControl));
    }
    uint64_t whitePassers = whitePawns & (~blackAreaOfControl);
    uint64_t blackPassers = blackPawns & (~whiteAreaOfControl);

    //qWarning() << "White passed pawns " << sumBits(whitePassers);
    //qWarning() << "Black passed pawns " << sumBits(blackPassers);

    return sumBits(whitePassers) - sumBits(blackPassers);
}

int Evaluation::backwardPawnCount(unsigned long long whitePawns, unsigned long long blackPawns) {
    // white pawns

    // Check if is supported
    uint64_t leftSupport = leftShift(whitePawns);
    uint64_t rightSupport = rightShift(whitePawns);

    for(int i=0; i<6; i++) {
        leftSupport |= upShift(leftSupport);
        rightSupport |= upShift(rightSupport);
    }
    uint64_t supportedPawns = leftSupport | rightSupport;

    // Check enemy block - one enemy pawn directly in front OR in L-shape ahead
    uint64_t enemyFront = downShift(blackPawns); // Directly in front
    uint64_t enemyLShape = downShift(downShift(leftShift(blackPawns))) | downShift(downShift(rightShift(blackPawns))); // L-shape in front

    uint64_t enemyBlockers = enemyFront | enemyLShape;

    // A pawn is backward if
    // - It has no support
    // - It has enemy pawns blocking its advancement
    uint64_t whiteBackwardPawns = whitePawns & ~supportedPawns & enemyBlockers;
    //qWarning() << "White backward pawn count: " << sumBits(whiteBackwardPawns);


    // black pawns
    leftSupport = leftShift(blackPawns);
    rightSupport = rightShift(blackPawns);
    for(int i=0; i<6; i++) {
        leftSupport |= downShift(leftSupport);
        rightSupport |= downShift(rightSupport);
    }
    supportedPawns = leftSupport | rightSupport;

    enemyFront = upShift(whitePawns);
    enemyLShape = upShift(upShift(leftShift(whitePawns))) | upShift(upShift(rightShift(whitePawns))); // L-shape in front

    enemyBlockers = enemyFront | enemyLShape;

    uint64_t blackBackwardPawns = blackPawns & ~supportedPawns & enemyBlockers;
    //qWarning() << "Black backward pawn count: " << sumBits(blackBackwardPawns);


    return sumBits(whiteBackwardPawns) - sumBits(blackBackwardPawns);
}




double Evaluation::pawnStructure(Board* board) {
    double bonus = 0;
    // isolated pawns
    //qWarning() << "Isolated white pawn count: " << isolatedPawnCount(board->getWhitePawns());
    //qWarning() << "Isolated black pawn count: " << isolatedPawnCount(board->getBlackPawns());
    bonus += isolatedPawnPenalty * (isolatedPawnCount(board->getWhitePawns()) - isolatedPawnCount(board->getBlackPawns()));
    // doubled pawns
    //qWarning() << "Doubled white pawn count: " << doubledPawnCount(board->getWhitePawns());
    //qWarning() << "Doubled black pawn count: " << doubledPawnCount(board->getBlackPawns());
    bonus += doubledPawnPenalty * (doubledPawnCount(board->getWhitePawns()) - doubledPawnCount(board->getWhitePawns()));
    // backward pawns
    bonus += backwardPawnPenalty * backwardPawnCount(board->getWhitePawns(), board->getBlackPawns());
    // passed pawns
    bonus += passedPawnBonus * passedPawnCount(board->getWhitePawns(), board->getBlackPawns());

    return bonus;
}

double Evaluation::pieceDevelopmentEvaluation(Board* board) {
    double whiteDevelopmentPenalty = 0;
    whiteDevelopmentPenalty += undesiredKnightLocationPenalty*sumBits(undesiredWhiteMinorPieceLocations & board->getWhiteKnights());
    whiteDevelopmentPenalty += undesiredBishopLocationPenalty*sumBits(undesiredWhiteMinorPieceLocations & board->getWhiteBishops());
    whiteDevelopmentPenalty += undesiredRookLocationPenalty*sumBits(undesiredWhiteRooksLocations & board->getWhiteRooks());
    whiteDevelopmentPenalty += undesiredKingLocationPenalty*sumBits(undesiredWhiteKingLocations & board->getWhiteKings());

    // qWarning() << "White development penalty " << whiteDevelopmentPenalty;

    double blackDevelopmentPenalty = 0;
    blackDevelopmentPenalty += undesiredKnightLocationPenalty*sumBits(undesiredBlackMinorPieceLocations & board->getBlackKnights());
    blackDevelopmentPenalty += undesiredBishopLocationPenalty*sumBits(undesiredBlackMinorPieceLocations & board->getBlackBishops());
    blackDevelopmentPenalty += undesiredRookLocationPenalty*sumBits(undesiredBlackRooksLocations & board->getBlackRooks());
    blackDevelopmentPenalty += undesiredKingLocationPenalty*sumBits(undesiredBlackKingLocations & board->getBlackKings());

    // qWarning() << "Black development penalty " << blackDevelopmentPenalty;

    return whiteDevelopmentPenalty - blackDevelopmentPenalty;
}

Board* Evaluation::calcEvalFromBranchTips(Board* startingBoard) {

    if (startingBoard == nullptr)
        return nullptr;
    if (startingBoard->getWhiteKings()==0 ) {
        startingBoard->setBoardEval(-10000);
        return startingBoard;
    } else if (startingBoard->getBlackKings()==0){
        startingBoard->setBoardEval(10000);
        return startingBoard;
    }

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

