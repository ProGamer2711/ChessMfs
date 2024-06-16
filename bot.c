#include <stdio.h>

#include "Chess.h"
#include "Constants.h"
#include "Vector.h"

#define MAX_DEPTH 4

double staticEvaluation(Vector* board, Vector* pieces) {
    double material = 0;

    for (byte i = 0; i < pieces->length; i++) {
        Piece* piece = pieces->get(pieces, i);

        double tempMat = 0;

        if (piece->isTaken) continue;

        if (piece->type == KING) tempMat = 8000;
        if (piece->type == ROOK_1 || piece->type == ROOK_2) tempMat = 5;

        // if the piece is black which wont happen here
        if (!piece->isWhite) tempMat *= -1;
        material += tempMat;
    }


    return material;
}

// white is maxxer
double minMax(Vector* moves, Vector* board, Vector* pieces, byte depth, byte whitePlayer) {
    Piece* blackKing = getPieceByName(pieces,"kg");
    Piece* whiteKing = getPieceByName(pieces,"KG");

    // This should prevent king sacking
    if(blackKing->isTaken)return 1e9;
    if(whiteKing->isTaken)return -1e9;

    if (depth == 0) {
        return staticEvaluation(board, pieces);
    }

    if (whitePlayer) {
        double maxx = -1e9;

        for (byte i = 0; i < pieces->length; i++) {
            Piece* currentPiece = pieces->get(pieces, i);

            if (currentPiece->isTaken) continue;
            if (currentPiece->isWhite != whitePlayer) continue;

            Vector* legalMoves = getLegalMoves(pieces, currentPiece, board);

            for (byte i = 0; i < legalMoves->length; i++) {
                Coordinate* move = legalMoves->get(legalMoves, i);

                makeMove(moves, board, currentPiece, *move);
                double result = minMax(moves, board, pieces, depth - 1, 0);

                maxx = max(maxx, result);
                undoMove(moves, board);
            }
        }

        if (maxx == 1e9) {
            return -depth * 1000;
        }

        return maxx;
    } else {
        double minn = 1e9;

        for (byte i = 0; i < pieces->length; i++) {
            Piece* currentPiece = pieces->get(pieces, i);

            if (currentPiece->isTaken) continue;
            if (currentPiece->isWhite != whitePlayer) continue;

            Vector* legalMoves = getLegalMoves(pieces, currentPiece, board);

            for (byte i = 0; i < legalMoves->length; i++) {
                Coordinate* move = legalMoves->get(legalMoves, i);

                makeMove(moves, board, currentPiece, *move);
                double result = minMax(moves, board, pieces, depth - 1, 1);

                minn = min(minn, result);
                undoMove(moves, board);
            }
        }

        if (minn == 1e9) {
            return depth * 1000;
        }

        return minn;
    }
}

void blackTurn(Vector* moves, Vector* board, Vector* pieces) {
    Piece* ansPiece;
    Coordinate ansMove;

    double bestEval = 1e9 + 1;


    for (byte i = 0; i < pieces->length; i++) {
        Piece* currentPiece = pieces->get(pieces, i);

        if (currentPiece->isTaken || currentPiece->isWhite != 0) continue;

        Vector* legalMoves = getLegalMoves(pieces, currentPiece, board);

        for (byte i = 0; i < legalMoves->length; i++) {
            Coordinate* move = legalMoves->get(legalMoves, i);

            makeMove(moves, board, currentPiece, *move);

            double result = minMax(moves, board, pieces, MAX_DEPTH, 1);

            if (result < bestEval) {
                ansPiece = currentPiece;
                ansMove = *move;
                bestEval = result;
            }

            undoMove(moves, board);
        }
    }

    makeMove(moves, board, ansPiece, ansMove);
}