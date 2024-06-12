#pragma once

#include "Constants.h"

// Could be any size but the best experience is up to 50
#define MAX_BOARD_SIZE 50

#define EMPTY_SPACE 0
#define POSSIBLE_MOVE 1
#define WHITE_KING 2
#define WHITE_ROOK_1 3
#define WHITE_ROOK_2 4
#define BLACK_KING 5

typedef struct Piece {
    Coordinate position;
    byte type;
} Piece;

void printBoard(byte **board, byte boardSize);

void runChessGame(byte boardSize);