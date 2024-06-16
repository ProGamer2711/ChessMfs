#include "Menu.h"

#include <stdio.h>

void clearScreen() {
    system(CLEAR_COMMAND);
}

void waitForEnter() {
    // clear the input buffer
    getchar();

    printf("Press Enter to continue...");
    while (getchar() != '\n');
}

static void printMainMenu() {
    printf("1. New game\n");
    printf("2. Change board size\n");
    printf("3. Replay a game\n");
    printf("4. Exit\n");
}

void runMainMenu(ExecuteMainSelectionFunction executeSelection, byte* currentBoardSize) {
    unsigned short currentChoice = 0;

    while (1) {
        clearScreen();

        printMainMenu();

        printf("> ");
        scanf("%hu", &currentChoice);

        byte shouldContinue = executeSelection(currentChoice, currentBoardSize);

        if (shouldContinue == 0) return;
    }
}

static void printReplayMenu() {
    printf("1. To begining of game\n");
    printf("2. Forward\n");
    printf("3. Backwards\n");
    printf("4. To end of game\n");
    printf("5. Exit\n");
}

void runReplayMenu(ExecuteReplaySelectionFunction executeSelection, Vector* pieceStartingPositions, Vector* board, Vector* pieces, Vector* moves) {
    unsigned short currentChoice = 0;

    while (1) {
        clearScreen();

        printReplayMenu();

        printf("> ");
        scanf("%hu", &currentChoice);

        byte shouldContinue = executeSelection(currentChoice, pieceStartingPositions, board, pieces, moves);

        if (shouldContinue == 0) return;
    }
}