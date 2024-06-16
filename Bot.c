#include "Bot.h"

#include <stdlib.h>

#include "Chess.h"
#include "Constants.h"
#include "Vector.h"

static double staticEvaluation(Vector* board, Vector* pieces) {
    double material = 0;

    for (byte i = 0; i < pieces->length; i++) {
        Piece* piece = pieces->get(pieces, i);

        double tempMaterial = 0;

        if (piece->isTaken) continue;

        if (piece->type == KING) tempMaterial = 8000;
        if (piece->type == ROOK_1 || piece->type == ROOK_2) tempMaterial = 5;

        // if the piece is black which wont happen here
        if (!piece->isWhite) tempMaterial *= -1;

        material += tempMaterial;
    }

    return material;
}

// white is max and black is min
static double minMax(Vector* moves, Vector* board, Vector* pieces, byte depth, byte whitePlayer) {
    Piece* blackKing = getPieceByName(pieces, "kg");
    Piece* whiteKing = getPieceByName(pieces, "KG");

    // This should prevent king sacking
    if (blackKing->isTaken) return 1e9;
    if (whiteKing->isTaken) return -1e9;

    if (depth == 0) {
        return staticEvaluation(board, pieces);
    }

    if (whitePlayer) {
        double max = -1e9;

        for (byte i = 0; i < pieces->length; i++) {
            Piece* currentPiece = pieces->get(pieces, i);

            if (currentPiece->isTaken) continue;
            if (currentPiece->isWhite != whitePlayer) continue;

            Vector* legalMoves = getLegalMoves(pieces, currentPiece, board);

            for (byte i = 0; i < legalMoves->length; i++) {
                Coordinate* move = legalMoves->get(legalMoves, i);

                makeMove(moves, board, currentPiece, *move);
                double result = minMax(moves, board, pieces, depth - 1, 0);

                max = MAX(max, result);
                undoMove(moves, board);
            }

            for (byte i = 0; i < legalMoves->length; i++) {
                free(legalMoves->get(legalMoves, i));
            }

            freeVector(legalMoves);
        }

        if (max == -1e9) {
            return -depth * 1000;
        }

        return max;
    } else {
        double min = 1e9;

        for (byte i = 0; i < pieces->length; i++) {
            Piece* currentPiece = pieces->get(pieces, i);

            if (currentPiece->isTaken) continue;
            if (currentPiece->isWhite != whitePlayer) continue;

            Vector* legalMoves = getLegalMoves(pieces, currentPiece, board);

            for (byte i = 0; i < legalMoves->length; i++) {
                Coordinate* move = legalMoves->get(legalMoves, i);

                makeMove(moves, board, currentPiece, *move);
                double result = minMax(moves, board, pieces, depth - 1, 1);

                min = MIN(min, result);
                undoMove(moves, board);
            }

            for (byte i = 0; i < legalMoves->length; i++) {
                free(legalMoves->get(legalMoves, i));
            }

            freeVector(legalMoves);
        }

        if (min == 1e9) {
            return depth * 1000;
        }

        return min;
    }
}

void blackTurn(Vector* moves, Vector* board, Vector* pieces) {
    Piece* bestPiece;
    Coordinate bestMove;

    double bestEval = 1e9 + 1;

    for (byte i = 0; i < pieces->length; i++) {
        Piece* currentPiece = pieces->get(pieces, i);

        if (currentPiece->isTaken || currentPiece->isWhite != 0) continue;

        Vector* legalMoves = getLegalMoves(pieces, currentPiece, board);

        for (byte i = 0; i < legalMoves->length; i++) {
            Coordinate* move = legalMoves->get(legalMoves, i);

            makeMove(moves, board, currentPiece, *move);

            double result = minMax(moves, board, pieces, MAX_DEPTH / board->length, 1);

            if (result < bestEval) {
                bestPiece = currentPiece;
                bestMove = *move;
                bestEval = result;
            }

            undoMove(moves, board);
        }

        for (byte i = 0; i < legalMoves->length; i++) {
            free(legalMoves->get(legalMoves, i));
        }

        freeVector(legalMoves);
    }

    makeMove(moves, board, bestPiece, bestMove);
}