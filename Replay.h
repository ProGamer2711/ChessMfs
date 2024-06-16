#pragma once

#include "Chess.h"
#include "Constants.h"

#define MOVE_SIZE 5

byte calculateSeedLength(byte pieceCount);

byte hasNextMove(FILE *file);

void writeReplayToFile(byte *seed, size_t seedLength, Vector *moves);

void replayGame();