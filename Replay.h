#pragma once

#include "Constants.h"
#include "Chess.h"

void writeToFile(char *fileName, byte *seed, size_t seedLength, Vector *moves);
void replayGame();