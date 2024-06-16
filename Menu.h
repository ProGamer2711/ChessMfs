#pragma once

#include <stdio.h>

#include "Constants.h"
#include "Vector.h"

typedef byte (*ExecuteMainSelectionFunction)(byte, byte*);
typedef byte (*ExecuteReplaySelectionFunction)(byte, FILE*, Vector*, Vector**, Vector**, Vector**);

void clearScreen();

void waitForEnter();

void runMainMenu(ExecuteMainSelectionFunction executeSelection, byte* currentBoardSize);

void runReplayMenu(ExecuteReplaySelectionFunction executeSelection, FILE* file, Vector* pieceStartingPositions, Vector** board, Vector** pieces, Vector** moves);