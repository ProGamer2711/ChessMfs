#pragma once

#include "Chess.h"
#include "Vector.h"

void printBoard(Vector *board);

void printMove(Move *move);

void printStatistics(Vector* moves, Vector* pieces, byte blackInCheckCounter);