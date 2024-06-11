#pragma once

#include <stdint.h>

// Could be any size but the best experience is up to 50
#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

typedef uint8_t byte;