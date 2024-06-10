#pragma once

#include <stdint.h>

// Could be any size but the best experience is up to 50
#define MAX_BOARD_SIZE 50

#define EMPTY_SPACE 0
#define WHITE_KING 1
#define WHITE_ROOK 2
#define BLACK_KING 3

typedef uint8_t byte;

void printCell(byte cell);

void printHorizontalBorder(byte boardSize);

void printHorizontalNumbers(byte boardSize);

void printBoardAsTable(byte **board, byte boardSize);

void printBoard(byte **board, byte boardSize);