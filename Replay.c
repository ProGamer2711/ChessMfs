#include "Replay.h"

// porbably not all imports necessary
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"
#include "Menu.h"
#include "Vector.h"

void writeToFile(char *fileName, byte *seed, size_t seedLength, Vector *moves)
{
    FILE *file = fopen(fileName, "wb");

    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    if (fwrite(seed, sizeof(byte), seedLength, file) != seedLength)
    {
        perror("Failed to write seed to file");
        fclose(file);
        return;
    }

    if (fwrite(moves->data, sizeof(Move), moves->length, file) != moves->length)
    {
        perror("Failed to write move to file");
        fclose(file);
        return;
    }

    fclose(file);
}

void replayGame()
{
    clearScreen();

    printf("Enter file name: ");

    char fileName[MAX_FILE_NAME_LENGTH];
    fgets(fileName, MAX_FILE_NAME_LENGTH, stdin);

    // ! Nathaniel go brr...
}