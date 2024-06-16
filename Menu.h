#pragma once

#include "Constants.h"

typedef void (*PrintMenuFunction)();
typedef int (*ExecuteSelectionFunction)(byte);

void clearScreen();

void waitForEnter();

void printMainMenu();

void printReplayMenu();

void runMenu(PrintMenuFunction printMenu, ExecuteSelectionFunction executeSelection);