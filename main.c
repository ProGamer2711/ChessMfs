#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ! Change the paths of .h
#include "Bot.c"
#include "Chess.c"
#include "Constants.h"
#include "Menu.c"
// ! Check if this import is needed later
// it is currently used to not have to compile with Vector.c
#include "Printing.c"
#include "Replay.c"
#include "Vector.c"

void changeBoardSize(byte* currentBoardSize) {
    clearScreen();

    printf("Current board size: %hu\n", *currentBoardSize);

    unsigned short newBoardSize;

    printf("Enter new board size\n> ");
    scanf("%hu", &newBoardSize);

    if (newBoardSize < MIN_BOARD_SIZE || newBoardSize > MAX_BOARD_SIZE) {
        printf("Invalid board size\n");
    } else {
        *currentBoardSize = newBoardSize;
    }
}

byte executeMainSelection(byte selection, byte* currentBoardSize) {
    switch (selection) {
        case 1:
            runChessGame(*currentBoardSize);

            return 1;
        case 2:
            changeBoardSize(currentBoardSize);

            return 1;
        case 3:
            replayGame();

            return 1;
        case 4:
            exit(0);

            return 1;
        default:
            printf("Invalid selection\n");

            return 0;
    }
}

int main() {
    srand(time(NULL));

    byte currentBoardSize = 8;

    runMainMenu(executeMainSelection, &currentBoardSize);

    return 0;
}