#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define EMPTY_SPACE 0
#define WHITE_KING 1
#define WHITE_ROOK 2
#define BLACK_KING 3

typedef uint8_t byte;

void writeToFile(char *fileName, byte *seed, byte **moves, size_t movesSize)
{
    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    size_t seedLength = 9;
    if (fwrite(seed, sizeof(byte), seedLength, file) != seedLength)
    {
        perror("Failed to write seed to file");
        fclose(file);
        return;
    }

    for (size_t i = 0; i < movesSize; i++)
    {
        byte *move = moves[i];
        if (fwrite(move, sizeof(byte), 3, file) != 3)
        {
            perror("Failed to write move to file");
            fclose(file);
            return;
        }
    }

    fclose(file);
}

int main()
{
    byte seed[9] = {3, 0, 0, 0, 1, 0, 2, 1, 0};
    byte move1[] = {0, 0, 0};
    byte move2[] = {1, 1, 1};
    byte move3[] = {2, 2, 2};
    byte *moves[] = {move1,
                     move2,
                     move3};

    writeToFile("replay.bin", seed, moves, 3);

    return 0;
}