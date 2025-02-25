#ifndef EVALUATION_H
#define EVALUATION_H
#include "board.h"

class Evaluation
{
private:

    static const constexpr double mg_pawn_value = 0.82;
    static const constexpr double mg_knight_value = 3.37;
    static const constexpr double mg_bishop_value = 3.65;
    static const constexpr double mg_rook_value = 4.77;
    static const constexpr double mg_queen_value = 10.25;

    static const constexpr double eg_pawn_value = 0.94;
    static const constexpr double eg_knight_value = 2.81;
    static const constexpr double eg_bishop_value = 2.97;
    static const constexpr double eg_rook_value = 5.12;
    static const constexpr double eg_queen_value = 9.36;

    static const constexpr double king_value = 1000; //TODO FIX and remove

    // PST - piece-square-tables + different evaulations for midgame and endgame
    // these values get tapered - more towards the endgame as it moves away from the opening

    static const constexpr double mg_pawn_table[64] = {
        0.0,   0.0,   0.0,   0.0,   0.0,   0.0,  0.0,   0.0,
        0.98, 1.34,  0.61,  0.95,  0.68, 1.26, 0.34, -0.11,
        -0.06,   0.07,  0.26,  0.31,  0.65,  0.56, 0.25, -0.20,
        -0.14,  0.13,   0.06,  0.21,  0.23,  0.12, 0.17, -0.23,
        -0.27,  -0.02,  -0.05,  0.12,  0.17,   0.06, 0.10, -0.25,
        -0.26,  -0.04,  -0.04, -0.10,   0.03,   0.03, 0.33, -0.12,
        -0.35,  -0.01, -0.20, -0.23, -0.15,  0.24, 0.38, -0.22,
        0.0,   0.0,   0.0,   0.0,   0.0,   0.0,  0.0,   0.0,
    };

    static const constexpr double eg_pawn_table[64] = {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        1.78, 1.73, 1.58, 1.34, 1.47, 1.32, 1.65, 1.87,
        0.94, 1.00, 0.85, 0.67, 0.56, 0.53, 0.82, 0.84,
        0.32, 0.24, 0.13, 0.05, -0.02, 0.04, 0.17, 0.17,
        0.13, 0.09, -0.03, -0.07, -0.07, -0.08, 0.03, -0.01,
        0.04, 0.07, -0.06, 0.01, 0.0, -0.05, -0.01, -0.08,
        0.13, 0.08, 0.08, 0.10, 0.13, 0.0, 0.02, -0.07,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    };

    static const constexpr double mg_knight_table[64] = {
        -1.67, -0.89, -0.34, -0.49, 0.61, -0.97, -0.15, -1.07,
        -0.73, -0.41, 0.72, 0.36, 0.23, 0.62, 0.07, -0.17,
        -0.47, 0.60, 0.37, 0.65, 0.84, 1.29, 0.73, 0.44,
        -0.09, 0.17, 0.19, 0.53, 0.37, 0.69, 0.18, 0.22,
        -0.13, 0.04, 0.16, 0.13, 0.28, 0.19, 0.21, -0.08,
        -0.23, -0.09, 0.12, 0.10, 0.19, 0.17, 0.25, -0.16,
        -0.29, -0.53, -0.12, -0.03, -0.01, 0.18, -0.14, -0.19,
        -1.05, -0.21, -0.58, -0.33, -0.17, -0.28, -0.19, -0.23,
    };

    static const constexpr double eg_knight_table[64] = {
        -0.58, -0.38, -0.13, -0.28, -0.31, -0.27, -0.63, -0.99,
        -0.25, -0.08, -0.25, -0.02, -0.09, -0.25, -0.24, -0.52,
        -0.24, -0.20, 0.10, 0.09, -0.01, -0.09, -0.19, -0.41,
        -0.17, 0.03, 0.22, 0.22, 0.22, 0.11, 0.08, -0.18,
        -0.18, -0.06, 0.16, 0.25, 0.16, 0.17, 0.04, -0.18,
        -0.23, -0.03, -0.01, 0.15, 0.10, -0.03, -0.20, -0.22,
        -0.42, -0.20, -0.10, -0.05, -0.02, -0.20, -0.23, -0.44,
        -0.29, -0.51, -0.23, -0.15, -0.22, -0.18, -0.50, -0.64,
    };

    static const constexpr double mg_bishop_table[64] = {
        -0.29,  0.04, -0.82, -0.37, -0.25, -0.42,  0.07, -0.08,
        -0.26,  0.16, -0.18, -0.13,  0.30,  0.59,  0.18, -0.47,
        -0.16,  0.37,  0.43,  0.40,  0.35,  0.50,  0.37, -0.02,
        -0.04,  0.05,  0.19,  0.50,  0.37,  0.37,  0.07, -0.02,
        -0.06,  0.13,  0.13,  0.26,  0.34,  0.12,  0.10,  0.04,
        0.00,  0.15,  0.15,  0.15,  0.14,  0.27,  0.18,  0.10,
        0.04,  0.15,  0.16,  0.00,  0.07,  0.21,  0.33,  0.01,
        -0.33, -0.03, -0.14, -0.21, -0.13, -0.12, -0.39, -0.21,
    };

    static const constexpr double eg_bishop_table[64] = {
        -0.14, -0.21, -0.11, -0.08, -0.07, -0.09, -0.17, -0.24,
        -0.08, -0.04,  0.07, -0.12, -0.03, -0.13, -0.04, -0.14,
        0.02, -0.08,  0.00, -0.01, -0.02,  0.06,  0.00,  0.04,
        -0.03,  0.09,  0.12,  0.09,  0.14,  0.10,  0.03,  0.02,
        -0.06,  0.03,  0.13,  0.19,  0.07,  0.10, -0.03, -0.09,
        -0.12, -0.03,  0.08,  0.10,  0.13,  0.03, -0.07, -0.15,
        -0.14, -0.18, -0.07, -0.01,  0.04, -0.09, -0.15, -0.27,
        -0.23, -0.09, -0.23, -0.05, -0.09, -0.16, -0.05, -0.17,
    };

    static const constexpr double mg_rook_table[64] = {
        0.32, 0.42, 0.32, 0.51, 0.63, 0.09, 0.31, 0.43,
        0.27, 0.32, 0.58, 0.62, 0.80, 0.67, 0.26, 0.44,
        -0.05, 0.19, 0.26, 0.36, 0.17, 0.45, 0.61, 0.16,
        -0.24, -0.11, 0.07, 0.26, 0.24, 0.35, -0.08, -0.20,
        -0.36, -0.26, -0.12, -0.01, 0.09, -0.07, 0.06, -0.23,
        -0.45, -0.25, -0.16, -0.17, 0.03, 0.00, -0.05, -0.33,
        -0.44, -0.16, -0.20, -0.09, -0.01, 0.11, -0.06, -0.71,
        -0.19, -0.13, 0.01, 0.17, 0.16, 0.07, -0.37, -0.26
    };

    static const constexpr double eg_rook_table[64] = {
        0.13, 0.10, 0.18, 0.15, 0.12, 0.12, 0.08, 0.05,
        0.11, 0.13, 0.13, 0.11, -0.03, 0.03, 0.08, 0.03,
        0.07, 0.07, 0.07, 0.05, 0.04, -0.03, -0.05, -0.03,
        0.04, 0.03, 0.13, 0.01, 0.02, 0.01, -0.01, 0.02,
        0.03, 0.05, 0.08, 0.04, -0.05, -0.06, -0.08, -0.11,
        -0.04, 0.00, -0.05, -0.01, -0.07, -0.12, -0.08, -0.16,
        -0.06, -0.06, 0.00, 0.02, -0.09, -0.09, -0.11, -0.03,
        -0.09, 0.02, 0.03, -0.01, -0.05, -0.13, 0.04, -0.20
    };

    static const constexpr double mg_queen_table[64] = {
        -0.28,  0.00,  0.29,  0.12,  0.59,  0.44,  0.43,  0.45,
        -0.24, -0.39, -0.05,  0.01, -0.16,  0.57,  0.28,  0.54,
        -0.13, -0.17,  0.07,  0.08,  0.29,  0.56,  0.47,  0.57,
        -0.27, -0.27, -0.16, -0.16, -0.01,  0.17, -0.02,  0.01,
        -0.09, -0.26, -0.09, -0.10, -0.02, -0.04,  0.03, -0.03,
        -0.14,  0.02, -0.11, -0.02, -0.05,  0.02,  0.14,  0.05,
        -0.35, -0.08,  0.11,  0.02,  0.08,  0.15, -0.03,  0.01,
        -0.01, -0.18, -0.09,  0.10, -0.15, -0.25, -0.31, -0.50,
    };

    static const constexpr double eg_queen_table[64] = {
        -0.09,  0.22,  0.22,  0.27,  0.27,  0.19,  0.10,  0.20,
        -0.17,  0.20,  0.32,  0.41,  0.58,  0.25,  0.30,  0.00,
        -0.20,  0.06,  0.09,  0.49,  0.47,  0.35,  0.19,  0.09,
        0.03,  0.22,  0.24,  0.45,  0.57,  0.40,  0.57,  0.36,
        -0.18,  0.28,  0.19,  0.47,  0.31,  0.34,  0.39,  0.23,
        -0.16, -0.27,  0.15,  0.06,  0.09,  0.17,  0.10,  0.05,
        -0.22, -0.23, -0.30, -0.16, -0.16, -0.23, -0.36, -0.32,
        -0.33, -0.28, -0.22, -0.43, -0.05, -0.32, -0.20, -0.41,
    };

    static const constexpr double mg_king_table[64] = {
        -0.65,  0.23,  0.16, -0.15, -0.56, -0.34,  0.02,  0.13,
        0.29, -0.01, -0.20, -0.07, -0.08, -0.04, -0.38, -0.29,
        -0.09,  0.24,  0.02, -0.16, -0.20,  0.06,  0.22, -0.22,
        -0.17, -0.20, -0.12, -0.27, -0.30, -0.25, -0.14, -0.36,
        -0.49, -0.01, -0.27, -0.39, -0.46, -0.44, -0.33, -0.51,
        -0.14, -0.14, -0.22, -0.46, -0.44, -0.30, -0.15, -0.27,
        0.01,  0.07, -0.08, -0.64, -0.43, -0.16,  0.09,  0.08,
        -0.15,  0.36,  0.12, -0.54,  0.08, -0.28,  0.24,  0.14,
    };

    static const constexpr double eg_king_table[64] = {
        -0.74, -0.35, -0.18, -0.18, -0.11,  0.15,  0.04, -0.17,
        -0.12,  0.17,  0.14,  0.17,  0.17,  0.38,  0.23,  0.11,
        0.10,  0.17,  0.23,  0.15,  0.20,  0.45,  0.44,  0.13,
        -0.08,  0.22,  0.24,  0.27,  0.26,  0.33,  0.26,  0.03,
        -0.18, -0.04,  0.21,  0.24,  0.27,  0.23,  0.09, -0.11,
        -0.19, -0.03,  0.11,  0.21,  0.23,  0.16,  0.07, -0.09,
        -0.27, -0.11,  0.04,  0.13,  0.14,  0.04, -0.05, -0.17,
        -0.53, -0.34, -0.21, -0.11, -0.28, -0.14, -0.24, -0.43
    };

    static const constexpr int pawn_gamephaseIncrease = 0;
    static const constexpr int knight_gamephaseIncrease = 1;
    static const constexpr int bishop_gamephaseIncrease = 1;
    static const constexpr int rook_gamephaseIncrease = 2;
    static const constexpr int queen_gamephaseIncrease = 4;

    static int flipSquare(int value);

    static const constexpr unsigned long long fileA = 0x0101010101010101;
    static const constexpr unsigned long long fileH = 0x8080808080808080;
    static const constexpr unsigned long long rank1 = 0x00000000000000FF;
    static const constexpr unsigned long long rank8 = 0xFF00000000000000;


    static unsigned long long leftShift(unsigned long long bitboard);
    static unsigned long long rightShift(unsigned long long bitboard);
    static unsigned long long upShift(unsigned long long bitboard);
    static unsigned long long downShift(unsigned long long bitboard);

    static int sumBits(unsigned long long bitboard);


    static int isolatedPawnCount(unsigned long long bitboard);
    static int doubledPawnCount(unsigned long long bitboard);
    static int backwardPawnCount(unsigned long long whitePawns, unsigned long long blackPawns);
    static int passedPawnCount(unsigned long long whitePawns, unsigned long long blackPawns);

    // pawn structure penalties/bonuses
    // TODO - values for parameters

    // Isolated pawns have no friendly pawns on the neighbouring ranks
    static const constexpr double isolatedPawnPenalty = -0.20;

    // Two pawns of the same color on the same file
    static const constexpr double doubledPawnPenalty = -0.10;

    // Cannot be supported by another pawn from behind.
    // Is blocked by an enemy pawn - directly in front or adjacent
    static const constexpr double backwardPawnPenalty = -0.10;

    // Adjecent and the same files are clear from the opposite color pawns
    static const constexpr double passedPawnBonus = 0.9;


    // Number of moves, where development heuristics are considered
    static const constexpr int openingPhaseMoveCount = 30;

    static const constexpr unsigned long long undesiredWhiteMinorPieceLocations = 0b0000000000000000000000000000000000000000000000000000000011100111;
    static const constexpr unsigned long long undesiredBlackMinorPieceLocations = 0b1110011100000000000000000000000000000000000000000000000000000000;
    static const constexpr double undesiredBishopLocationPenalty = -0.2;
    static const constexpr double undesiredKnightLocationPenalty = -0.1;

    static const constexpr unsigned long long undesiredWhiteRooksLocations = 0b0000000000000000000000000000000000000000000000000000000011000111;
    static const constexpr unsigned long long undesiredBlackRooksLocations = 0b1100011100000000000000000000000000000000000000000000000000000000;

    static const constexpr double undesiredRookLocationPenalty = -0.2;
    static const constexpr unsigned long long undesiredWhiteKingLocations = 0b1111111111111111111111111111111111111111111111111111111100111000;
    static const constexpr unsigned long long undesiredBlackKingLocations = 0b0011100011111111111111111111111111111111111111111111111111111111;

    static const constexpr double undesiredKingLocationPenalty = -0.8;

public:
    Evaluation();
    static double evaluatePosition(Board* board); // Main eval function, calculating based on private eval functions

    // different heuristics
    static double pieceDevelopmentEvaluation(Board* board);
    static double pieceSquareTables(Board* board);
    static double pawnStructure(Board* board);

};

#endif // EVALUATION_H
