#include "ChessUtils.h"

#include <stdio.h>

void printCell(byte cell) {
    switch (cell) {
        case EMPTY_SPACE:
            printf("  ");
            break;
        case WHITE_KING:
            printf("wk");
            break;
        case WHITE_ROOK:
            printf("wr");
            break;
        case BLACK_KING:
            printf("bk");
            break;
        default:
            printf("??");
            break;
    }
}

void printHorizontalBorder(byte boardSize) {
    for (byte j = 0; j < boardSize; j++) {
        printf("+----");
    }
    printf("+\n");
}

void printHorizontalNumbers(byte boardSize) {
    // add one extra to start from 00 with an extra cell
    // so we can have row numbers too
    for (byte j = 0; j < boardSize + 1; j++) {
        printf("| %02d ", j);
    }

    printf("|\n");
}

void printBoardAsTable(byte **board, byte boardSize) {
    printHorizontalBorder(boardSize + 1);

    printHorizontalNumbers(boardSize);

    for (byte i = 0; i < boardSize; i++) {
        // top border
        printHorizontalBorder(boardSize + 1);

        // add the actual cells
        for (byte j = 0; j < boardSize; j++) {
            // print the index as a cell before that
            if (j == 0) {
                printf("| %02d ", i + 1);
            }

            printf("| ");
            printCell(board[i][j]);
            printf(" ");
        }
        printf("|\n");
    }

    // bottom border
    printHorizontalBorder(boardSize + 1);
}

void printBoard(byte **board, byte boardSize) {
    if (boardSize <= 20) {
        printBoardAsTable(board, boardSize);
    } else {
        for (byte i = 0; i < boardSize; i++) {
            // print the top numbers
            if (i == 0) {
                printf("   ");
                for (byte j = 0; j < boardSize; j++) {
                    printf("%02d ", j + 1);
                }
                printf("\n");
            }

            for (byte j = 0; j < boardSize; j++) {
                // print the index
                if (j == 0) {
                    printf("%02d ", i + 1);
                }

                if (board[i][j] != EMPTY_SPACE) {
                    printCell(board[i][j]);
                } else {
                    printf("--");
                }

                printf(" ");
            }

            printf("\n");
        }
    }
}
