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

void printMainMenu() {
    printf("1. New game\n");
    printf("2. Change board size\n");
    printf("3. Replay a game\n");
    printf("4. Exit\n");
}

void runMainMenu(ExecuteSelectionFunction executeSelection) {
    unsigned short currentChoice = 0;

    while (1) {
        clearScreen();

        printMainMenu();

        printf("> ");
        scanf("%hu", &currentChoice);

        executeSelection(currentChoice);
    }
}
