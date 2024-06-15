#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ! Change the paths of .h
#include "Chess.c"
#include "Constants.h"
#include "Menu.c"
// ! Check if this import is needed later
// it is currently used to not have to compile with Vector.c
#include "Vector.c"
#include "Replay.c"

// ? Should this be a global variable or should it be passed along?
byte currentBoardSize = 8;

void changeBoardSize()
{
    clearScreen();

    unsigned short newBoardSize;

    printf("Enter new board size: ");
    scanf("%hu", &newBoardSize);

    if (newBoardSize < MIN_BOARD_SIZE || newBoardSize > MAX_BOARD_SIZE)
    {
        printf("Invalid board size\n");
    }
    else
    {
        currentBoardSize = newBoardSize;
    }
}

void executeSelection(byte selection)
{
    switch (selection)
    {
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

    waitForEnter();
}

int main()
{
    srand(time(NULL));

    // byte seed[9] = {8, 0, 0, 1, 1, 2, 2, 7, 7};
    // Move *move1 = createMove();

    // writeToFile("replay.bin", seed, moves, 3);

    // runMainMenu(executeSelection);
    printf("%ld", sizeof(Move));

    return 0;
}