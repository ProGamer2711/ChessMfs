#include "Replay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"
#include "Menu.h"
#include "Vector.h"

// 1 byte for board size
// 1 byte for piece count
// 2 bytes for each piece's position
byte calculateSeedLength(byte pieceCount) {
    return 2 + pieceCount * 2;
}

void writeReplayToFile(byte *seed, size_t seedLength, Vector *moves) {
    getchar();

    printf("Where do you want to store your replay?\n> ");

    char fileName[MAX_FILE_NAME_LENGTH];
    fgets(fileName, MAX_FILE_NAME_LENGTH, stdin);

    // remove the \n from the end of the string
    fileName[strlen(fileName) - 1] = '\0';

    FILE *file = fopen(fileName, "wb");

    if (file == NULL) {
        printf("Failed to open file");

        exit(1);
    }

    if (fwrite(seed, sizeof(byte), seedLength, file) != seedLength) {
        printf("Failed to write seed to file");
        fclose(file);

        exit(1);
    }

    if (fwrite(moves->data, sizeof(Move), moves->length, file) != moves->length) {
        printf("Failed to write move to file");
        fclose(file);

        exit(1);
    }

    fclose(file);
}

void replayGame() {
    clearScreen();

    printf("Which file do you want to replay from?\n> ");

    char fileName[MAX_FILE_NAME_LENGTH];
    fgets(fileName, MAX_FILE_NAME_LENGTH, stdin);

    // remove the \n from the end of the string
    fileName[strlen(fileName) - 1] = '\0';

    // ! Nathaniel go brr...
}