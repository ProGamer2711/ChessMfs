#pragma once

#include <stdint.h>

// Could be any size but the best experience is up to 50
#define MAX_BOARD_SIZE 50

#define EMPTY_SPACE 0
#define WHITE_KING 1
#define WHITE_ROOK_1 2
#define WHITE_ROOK_2 3
#define BLACK_KING 4

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

typedef uint8_t byte;