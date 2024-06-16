#pragma once

#include "Chess.h"
#include "Constants.h"

byte calculateSeedLength(byte pieceCount);

void writeMoveToFile(FILE *file, Move *move);

void writeReplayToFile(byte *seed, size_t seedLength, Vector *moves);

byte executeReplaySelection(byte selection);

void replayGame();