#pragma once

#include <stdio.h>

#include "Constants.h"
#include "Vector.h"

#define MOVE_SIZE 4

byte calculateSeedLength(byte pieceCount);

byte hasNextMove(FILE *file);

void writeReplayToFile(byte *seed, size_t seedLength, Vector *moves);

void replayGame();