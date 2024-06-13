#pragma once

#include <stdint.h>

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

#define MAX_FILE_NAME_LENGTH 100

typedef uint8_t byte;

typedef struct Coordinate {
    byte x;
    byte y;
} Coordinate;