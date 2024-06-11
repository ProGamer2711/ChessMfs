#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ! Change the paths of .h
#include "ChessUtils.c"
#include "Menu.c"
#include "Vector.c"

// ? Should this be a global variable?
byte currentBoardSize = 8;

// it is simpler to keep this a 2D array and not a vector
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

void freeBoard(byte **board, byte boardSize) {
    for (byte i = 0; i < boardSize; i++) {
        free(board[i]);
    }

    free(board);
}

void placePiecesRandomly(byte **board, byte boardSize, byte *pieces, byte piecesLength) {
    for (byte i = 0; i < piecesLength; i++) {
        byte x = rand() % boardSize;
        byte y = rand() % boardSize;

        while (board[x][y] != EMPTY_SPACE) {
            x = rand() % boardSize;
            y = rand() % boardSize;
        }

        board[x][y] = pieces[i];
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

void newGame() {
    byte **board = generateBoard(currentBoardSize);

    // ! I chose to use an array because if I used a Vector, I would need to cast to void*
    // ? Maybe make it a global variable?
    byte piecesToPlaceLength = 4;
    byte piecesToPlace[] = {WHITE_KING, WHITE_ROOK_1, WHITE_ROOK_2, BLACK_KING};

    placePiecesRandomly(board, currentBoardSize, piecesToPlace, piecesToPlaceLength);

    printBoard(board, currentBoardSize);

    freeBoard(board, currentBoardSize);
}

void changeBoardSize() {
    // ? Should we clear the screen here?
    // clearScreen();

    byte newBoardSize;

    printf("Enter new board size: ");
    scanf("%hhu", &newBoardSize);

    if (newBoardSize < 3 || newBoardSize > MAX_BOARD_SIZE) {
        printf("Invalid board size\n");
    } else {
        currentBoardSize = newBoardSize;
    }
}

void replayGame() {
    clearScreen();

    printf("Enter file name: ");

    char fileName[100];
    fgets(fileName, 100, stdin);

    // ! Nathaniel go brr...
}

void executeSelection(byte selection) {
    switch (selection) {
        case 1:
            newGame();
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