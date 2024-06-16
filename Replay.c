#include "Replay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Chess.h"
#include "Constants.h"
#include "Menu.h"
#include "Vector.h"

#define MOVE_SIZE 5

// 1 byte for board size
// 1 byte for piece count
// 2 bytes for each piece's position
byte calculateSeedLength(byte pieceCount) {
    return 2 + pieceCount * 2;
}

static void writeMoveToFile(FILE *file, Move *move) {
    PieceType pieceTakenType = move->pieceTaken == NULL ? 0 : move->pieceTaken->type;
    byte pieceTaken = pieceTakenType;

    // a move can be represented by 5 bytes
    // 2 bytes for the start position
    // 2 bytes for the end position
    // 1 byte for the piece taken (the byte 0 if no piece was taken)
    byte moveBytes[MOVE_SIZE] = {
        move->start.x,
        move->start.y,
        move->end.x,
        move->end.y,
        pieceTaken,
    };

    if (fwrite(moveBytes, sizeof(byte), MOVE_SIZE, file) != MOVE_SIZE) {
        printf("Failed to write move to file");
        fclose(file);

        exit(1);
    }
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

    for (byte i = 0; i < moves->length; i++) {
        writeMoveToFile(file, moves->get(moves, i));
    }

    fclose(file);
}

void freeReplayArgument(Vector* replayArgument) {
    for(byte i = 0; i < replayArgument->length; i++) {
        free(replayArgument->get(replayArgument, i));
    }

    freeVector(replayArgument);
}

void freeReplayArguments(Vector* board, Vector* pieces, Vector* moves) {
    freeBoard(board);
    freeReplayArgument(pieces);
    freeReplayArgument(moves);
}

static byte executeReplaySelection(byte selection, FILE* file, Vector* pieceStartingPositions, Vector** board, Vector** pieces, Vector** moves) {
    switch (selection) {
        case 1:
            byte offset = calculateSeedLength((*pieces)->length), boardSize = (*board)->length;

            fseek(file, offset, SEEK_SET);

            freeReplayArguments(*board, *pieces, *moves);

            initializeReplay(boardSize, pieceStartingPositions, board, pieces, moves);

            return 1;
        case 2:
            return 1;
        case 3:
            byte position[2];
            fread(position, sizeof(byte), 2, file);
            Tile* tile = getTileFromBoard(*board, position[0], position[1]);

            fread(position, sizeof(byte), 2, file);
            Coordinate endCoordinate = { position[0], position[1] };

            fseek(file, 1, SEEK_CUR);

            makeMove(*moves, *board, tile->piece, endCoordinate);

            return 1;
        case 4:
            return 1;
        case 5:
            return 0;
        default:
            printf("Invalid selection\n");
            return 1;
    }
}

void replayGame() {
    clearScreen();

    getchar();

    printf("Which file do you want to replay from?\n> ");

    char fileName[MAX_FILE_NAME_LENGTH];
    fgets(fileName, MAX_FILE_NAME_LENGTH, stdin);

    // remove the \n from the end of the string
    fileName[strlen(fileName) - 1] = '\0';

    byte boardSize, numberOfPieces;
    Vector *pieceStartingPositions, *board = NULL, *pieces = NULL, *moves = NULL;

    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Failed to open file");

        exit(1);
    }

    // read the first 2 bytes
    fread(&boardSize, sizeof(byte), 1, file);
    fread(&numberOfPieces, sizeof(byte), 1, file);

    pieceStartingPositions = initVector();

    for (byte i = 0; i < numberOfPieces; i++) {
        byte position[2];
        fread(position, sizeof(byte), 2, file);
        Coordinate *coordinate = createCoordinate(position[0], position[1]);
        pieceStartingPositions->push(pieceStartingPositions, coordinate);
    }

    initializeReplay(boardSize, pieceStartingPositions, &board, &pieces, &moves);

    // printf("%d, %d  ", boardSize, numberOfPieces);
    // for(int i=0; i < pieceStartingPositions->length;i++) {
    //     Coordinate* coord = pieceStartingPositions->get(pieceStartingPositions, i);
    //     printf("x: %d, y:%d     ", coord->x, coord->y);
    // }

    runReplayMenu(executeReplaySelection, file, pieceStartingPositions, &board, &pieces, &moves);

    fclose(file);

    // free stuff later
    // freeVector(pieceStartingPositions);
}
