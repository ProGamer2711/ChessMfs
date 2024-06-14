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

// ? Should this be a global variable or should it be passed along?
byte currentBoardSize = 8;

void writeToFile(char *fileName, byte *seed, byte **moves, size_t movesSize) {
    FILE *file = fopen(fileName, "wb");

    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    size_t seedLength = 9;

    if (fwrite(seed, sizeof(byte), seedLength, file) != seedLength) {
        perror("Failed to write seed to file");
        fclose(file);
        return;
    }

    for (size_t i = 0; i < movesSize; i++) {
        byte *move = moves[i];
        if (fwrite(move, sizeof(byte), 4, file) != 4) {
            perror("Failed to write move to file");
            fclose(file);
            return;
        }
    }

    fclose(file);
}

void changeBoardSize() {
    clearScreen();

    unsigned short newBoardSize;

    printf("Enter new board size: ");
    scanf("%hu", &newBoardSize);

    if (newBoardSize < MIN_BOARD_SIZE || newBoardSize > MAX_BOARD_SIZE) {
        printf("Invalid board size\n");
    } else {
        currentBoardSize = newBoardSize;
    }
}

void replayGame() {
    clearScreen();

    printf("Enter file name: ");

    char fileName[MAX_FILE_NAME_LENGTH];
    fgets(fileName, MAX_FILE_NAME_LENGTH, stdin);

    // ! Nathaniel go brr...
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

    waitForEnter();
}

int main() {
    srand(time(NULL));

    // byte seed[9] = {3, 0, 0, 0, 1, 0, 2, 1, 0};
    // byte move1[] = {0, 0, 0};
    // byte move2[] = {1, 1, 1};
    // byte move3[] = {2, 2, 2};
    // byte *moves[] = {move1,
    //                  move2,
    //                  move3};

    // writeToFile("replay.bin", seed, moves, 3);

    runMainMenu(executeSelection);

    return 0;
}