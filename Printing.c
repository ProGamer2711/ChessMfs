#include "Printing.h"

#include "Chess.h"

#include <stdio.h>

static void printTile(Tile *tile) {
    if (tile->type == POSSIBLE_MOVE) {
        printf("(");
    } else {
        printf(" ");
    }

    if (tile->piece == NULL) {
        printf("  ");
    } else if (tile->piece->isWhite) {
        switch (tile->piece->type) {
            case KING:
                printf("KG");
                break;
            case ROOK_1:
                printf("R1");
                break;
            case ROOK_2:
                printf("R2");
                break;
            default:
                printf("??");
                break;
        }
    } else {
        switch (tile->piece->type) {
            case KING:
                printf("kg");
                break;
            // rooks are not needed here
            // but we can still add them
            case ROOK_1:
                printf("r1");
                break;
            case ROOK_2:
                printf("r2");
                break;
            default:
                printf("??");
                break;
        }
    }

    if (tile->type == POSSIBLE_MOVE) {
        printf(")");
    } else {
        printf(" ");
    }
}

// If you use the simplified table
// you will not see your possible moves
// to save space in the console
static void printTileSimplified(Tile *tile) {
    if (tile->piece == NULL) {
        printf("--");
    } else if (tile->piece->isWhite) {
        switch (tile->piece->type) {
            case KING:
                printf("KG");
                break;
            case ROOK_1:
                printf("R1");
                break;
            case ROOK_2:
                printf("R2");
                break;
            default:
                printf("??");
                break;
        }
    } else {
        switch (tile->piece->type) {
            case KING:
                printf("kg");
                break;
            // rooks are not needed here
            default:
                printf("??");
                break;
        }
    }
}

static void printHorizontalBorder(byte boardSize) {
    for (byte j = 0; j < boardSize; j++) {
        printf("+----");
    }
    printf("+\n");
}

static void printHorizontalNumbers(byte boardSize) {
    // add one extra to start from 00 with an extra cell
    // so we can have row numbers too
    for (byte j = 0; j < boardSize + 1; j++) {
        printf("| %02d ", j);
    }

    printf("|\n");
}

static void printBoardAsTable(Vector *board) {
    printHorizontalBorder(board->length + 1);

    printHorizontalNumbers(board->length);

    for (byte i = 0; i < board->length; i++) {
        // top border
        printHorizontalBorder(board->length + 1);

        // add the actual cells
        for (byte j = 0; j < board->length; j++) {
            // print the index as a cell before that
            if (j == 0) {
                printf("| %02d ", i + 1);
            }

            printf("|");

            Tile *currentTile = getTileFromBoard(board, i, j);

            printTile(currentTile);
        }
        printf("|\n");
    }

    // bottom border
    printHorizontalBorder(board->length + 1);
}

static void printBoardAsSimplifiedTable(Vector *board) {
    for (byte i = 0; i < board->length; i++) {
        // print the top numbers
        if (i == 0) {
            for (byte j = 0; j < board->length + 1; j++) {
                printf("%02d ", j);
            }
            printf("\n");
        }

        for (byte j = 0; j < board->length; j++) {
            // print the index
            if (j == 0) {
                printf("%02d ", i + 1);
            }

            Tile *currentTile = getTileFromBoard(board, i, j);

            if (currentTile->piece != NULL) {
                printTileSimplified(currentTile);
            } else {
                printf("--");
            }

            printf(" ");
        }

        printf("\n");
    }
}

void printBoard(Vector *board) {
    printf("You play as white\n");
    printf("White pieces have uppercase letters\n");
    printf("Black pieces have lowercase letters\n");
    printf("White king: KG\n");
    printf("White rook 1: R1\n");
    printf("White rook 2: R2\n");
    printf("Black king: kg\n");

    if (board->length <= 20) {
        printf("\n");

        printBoardAsTable(board);
    } else {
        printf("Empty spaces are marked with --\n\n");

        printBoardAsSimplifiedTable(board);
    }
}

void printMove(Move *move) {
    printf("%s %s ", move->piece->isWhite ? "White" : "Black", move->piece->type == KING ? "king" : "rook");
    printf("(%d, %d) -> ", move->start.x + 1, move->start.y + 1);
    printf("(%d, %d)", move->end.x + 1, move->end.y + 1);

    if (move->pieceTaken != NULL) {
        printf(" (piece taken: %s %s)", move->pieceTaken->isWhite ? "white" : "black", move->pieceTaken->type == KING ? "king" : "rook");
    }

    printf("\n");
}

void printStatistics(Vector* moves, Vector* pieces, byte blackInCheckCounter) {
    printf("The total number of moves was %d\n", moves->length);

    for(byte i = 0; i < pieces->length; i++) {
        Piece* currentPiece = pieces->get(pieces, i);
        byte moveCounter = movesPerPiece(moves, currentPiece);

        char color[6];
        strcpy(color, currentPiece->isWhite ? "white" : "black");

        printf("The ");

        switch(currentPiece->type) {
            case KING: 
                printf("%s king ", color);
                break;
            case ROOK_1:
                printf("first %s rook ", color);
                break;
            case ROOK_2:
                printf("second %s rook ", color);
                break;
            default:
                printf("Invalid piece\n");
                break;
        }

        printf("was moved %d time", moveCounter);
        
        if(moveCounter != 1) {
            printf("s");
        }

        printf("\n");
    }

    printf("The black king has been checked %d time", blackInCheckCounter);

    if(blackInCheckCounter != 1) {
        printf("s");
    }

    printf("\n");

    printf("The number of pieces captured by the black king was %d\n", capturedPieces(pieces));
}
