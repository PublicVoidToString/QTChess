#ifndef EVALUATION_H
#define EVALUATION_H
#include "board.h"

class Evaluation
{
private:
    // Evaluation functions
    static unsigned char sumBits(unsigned long long variable) ;
    static long long sumWhiteMaterial(Board* board) ;
    static long long sumBlackMaterial(Board* board) ;
public:
    Evaluation();
    static double evaluatePosition(Board* board); // Main eval function, calculating based on private eval functions

    static Board* calcEvalFromBranchTips(Board* startingBoard);
    static Board* getBestBranchFromGT(Board* startingBoard);
};

#endif // EVALUATION_H
