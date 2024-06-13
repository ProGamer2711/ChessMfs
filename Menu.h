#pragma once

#include "Constants.h"

// type for a function that takes a byte and returns void
typedef void (*ExecuteSelectionFunction)(byte);

void clearScreen();

void waitForEnter();

void printMainMenu();

void runMainMenu(ExecuteSelectionFunction executeSelection);