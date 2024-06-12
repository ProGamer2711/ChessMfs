#pragma once

#include <stdint.h>

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

typedef struct Coordinate {
    byte x;
    byte y;
} Coordinate;

typedef uint8_t byte;