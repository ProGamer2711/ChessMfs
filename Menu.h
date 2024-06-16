#pragma once

#include "Constants.h"

typedef void (*PrintMenuFunction)();
typedef byte (*ExecuteSelectionFunction)(byte);

void clearScreen();

void waitForEnter();

void printMainMenu();

void printReplayMenu();

void runMenu(PrintMenuFunction printMenu, ExecuteSelectionFunction executeSelection);