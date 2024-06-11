#include "MainMenu.h"

void clearScreen() {
    // ANSI escape codes to clear the screen and move the cursor to the top-left
    // aka magic
    printf("\033[H\033[J");
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
    byte currentChoice = 0;

    while (1) {
        clearScreen();

        printMainMenu();

        printf("> ");
        scanf("%hhu", &currentChoice);

        executeSelection(currentChoice);
    }
}
