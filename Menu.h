#pragma once

#include "Constants.h"

typedef void (*PrintMenuFunction)();
typedef void (*ExecuteSelectionFunction)(byte);

void clearScreen();

void waitForEnter();

void printMainMenu();

void runMenu(PrintMenuFunction printMenu, ExecuteSelectionFunction executeSelection);