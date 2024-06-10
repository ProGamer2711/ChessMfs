#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ChessUtils.h"

byte **generateBoard(byte boardSize) {
    if (boardSize > MAX_BOARD_SIZE) {
        printf("Board size is too large\n");
        exit(1);
    }

    byte **board = (byte **)calloc(boardSize, sizeof(byte *));

    if (board == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (byte i = 0; i < boardSize; i++) {
        board[i] = (byte *)calloc(boardSize, sizeof(byte));

        if (board[i] == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    return board;
}

void placePiecesRandomly(byte **board, byte boardSize) {
    byte placed[] = {WHITE_KING, WHITE_ROOK, BLACK_KING, WHITE_ROOK};

    for (byte i = 0; i < 4; i++) {
        byte x = rand() % boardSize;
        byte y = rand() % boardSize;

        while (board[x][y] != EMPTY_SPACE) {
            x = rand() % boardSize;
            y = rand() % boardSize;
        }

        board[x][y] = placed[i];
    }
}

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

int main() {
    // byte seed[9] = {3, 0, 0, 0, 1, 0, 2, 1, 0};
    // byte move1[] = {0, 0, 0};
    // byte move2[] = {1, 1, 1};
    // byte move3[] = {2, 2, 2};
    // byte *moves[] = {move1,
    //                  move2,
    //                  move3};

    // writeToFile("replay.bin", seed, moves, 3);

    srand(time(NULL));

    byte boardSize = 20;

    byte **board = generateBoard(boardSize);

    placePiecesRandomly(board, boardSize);

    printBoard(board, boardSize);

    return 0;
}