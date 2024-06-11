#pragma once

#include "Constants.h"

#define MAX_BOARD_SIZE 50

#define EMPTY_SPACE 0
#define WHITE_KING 1
#define WHITE_ROOK_1 2
#define WHITE_ROOK_2 3
#define BLACK_KING 4

typedef struct Piece {
    byte x;
    byte y;
    byte type;
} Piece;

void printCell(byte cell);

void printHorizontalBorder(byte boardSize);

void printHorizontalNumbers(byte boardSize);

void printBoardAsTable(byte **board, byte boardSize);

void printBoard(byte **board, byte boardSize);

void runChessGame(byte boardSize);