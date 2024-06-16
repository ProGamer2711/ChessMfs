#pragma once

#include "Chess.h"
#include "Constants.h"

byte calculateSeedLength(byte pieceCount);

void writeReplayToFile(byte *seed, size_t seedLength, Vector *moves);

void replayGame();