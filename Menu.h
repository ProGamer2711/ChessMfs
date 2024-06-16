#pragma once

#include "Constants.h"
#include "Vector.h"

typedef byte (*ExecuteMainSelectionFunction)(byte, byte*);
typedef byte (*ExecuteReplaySelectionFunction)(byte, Vector*, Vector*, Vector*, Vector*);

void clearScreen();

void waitForEnter();

void printMainMenu();

void runMainMenu(ExecuteMainSelectionFunction executeSelection, byte* currentBoardSize);

void printReplayMenu();

void runReplayMenu(ExecuteReplaySelectionFunction executeSelection, Vector* pieceStartingPositions, Vector* board, Vector* pieces, Vector* moves);