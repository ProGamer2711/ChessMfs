#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ! Change the paths of .h
#include "Chess.c"
#include "Constants.h"
#include "Menu.c"
// ! Check if this import is needed later
// it is currently used to not have to compile with Vector.c
#include "Printing.c"
#include "Replay.c"
#include "Vector.c"

// ? Should this be a global variable or should it be passed along?
byte currentBoardSize = 8;

void changeBoardSize() {
    clearScreen();

    unsigned short newBoardSize;

    printf("Enter new board size\n> ");
    scanf("%hu", &newBoardSize);

    if (newBoardSize < MIN_BOARD_SIZE || newBoardSize > MAX_BOARD_SIZE) {
        printf("Invalid board size\n");
    } else {
        currentBoardSize = newBoardSize;
    }
}

void executeSelection(byte selection) {
    switch (selection) {
        case 1:
            runChessGame(currentBoardSize);

            break;
        case 2:
            changeBoardSize();
            break;
        case 3:
            replayGame();
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("Invalid selection\n");
            break;
    }
}

int main() {
    srand(time(NULL));

    runMainMenu(executeSelection);

    return 0;
}