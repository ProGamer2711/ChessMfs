#include "Chess.h"

#include <stdio.h>

#include "Menu.h"
#include "Vector.h"

Piece *createPiece(byte x, byte y, byte type) {
    Piece *piece = (Piece *)malloc(sizeof(Piece));

    if (piece == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    piece->x = x;
    piece->y = y;
    piece->type = type;

    return piece;
}

// it is simpler to keep this a 2D array and not a vector
byte **generateBoard(byte boardSize) {
    if (boardSize > MAX_BOARD_SIZE) {
        printf("Board size is too large\n");
        exit(1);
    }

    byte **board = (byte **)calloc(boardSize, sizeof(byte *));

    if (board == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (byte i = 0; i < boardSize; i++) {
        board[i] = (byte *)calloc(boardSize, sizeof(byte));

        if (board[i] == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    return board;
}

void freeBoard(byte **board, byte boardSize) {
    for (byte i = 0; i < boardSize; i++) {
        free(board[i]);
    }

    free(board);
}

void placePiecesRandomly(byte **board, byte boardSize, Vector *pieces) {
    for (byte i = 0; i < pieces->length; i++) {
        Piece *currentPiece = pieces->get(pieces, i);

        byte x = rand() % boardSize;
        byte y = rand() % boardSize;

        while (board[x][y] != EMPTY_SPACE) {
            x = rand() % boardSize;
            y = rand() % boardSize;
        }

        currentPiece->x = x;
        currentPiece->y = y;

        board[x][y] = currentPiece->type;
    }
}

void printCell(byte cell) {
    switch (cell) {
        case EMPTY_SPACE:
            printf("  ");
            break;
        case WHITE_KING:
            printf("KG");
            break;
        case WHITE_ROOK_1:
            printf("R1");
            break;
        case WHITE_ROOK_2:
            printf("R2");
            break;
        case BLACK_KING:
            printf("kg");
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

void printBoardAsSimplifiedTable(byte **board, byte boardSize) {
    for (byte i = 0; i < boardSize; i++) {
        // print the top numbers
        if (i == 0) {
            for (byte j = 0; j < boardSize + 1; j++) {
                printf("%02d ", j);
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

void printBoard(byte **board, byte boardSize) {
    clearScreen();

    printf("You play as white\n");
    printf("White pieces have uppercase letters\n");
    printf("Black pieces have lowercase letters\n");

    if (boardSize <= 20) {
        printf("\n");

        printBoardAsTable(board, boardSize);
    } else {
        printf("Empty spaces are marked with --\n\n");

        printBoardAsSimplifiedTable(board, boardSize);
    }
}

void runChessGame(byte boardSize) {
    byte **board = generateBoard(boardSize);

    // use the pieces struct
    Vector *pieces = initVector();

    pieces->push(pieces, createPiece(0, 0, WHITE_KING));
    pieces->push(pieces, createPiece(0, 0, WHITE_ROOK_1));
    pieces->push(pieces, createPiece(0, 0, WHITE_ROOK_2));
    pieces->push(pieces, createPiece(0, 0, BLACK_KING));

    placePiecesRandomly(board, boardSize, pieces);

    printBoard(board, boardSize);

    // more stuff here

    freeBoard(board, boardSize);

    for (int i = 0; i < pieces->length; i++) {
        free(pieces->get(pieces, i));
    }

    freeVector(pieces);
}
