#include "Menu.h"

#include <stdio.h>

void clearScreen() {
    system(CLEAR_COMMAND);
}

void waitForEnter() {
    // clear the input buffer
    getchar();

    printf("Press Enter to continue...");
    while (getchar() != '\n');
}

static void printMainMenu() {
    printf("1. New game\n");
    printf("2. Change board size\n");
    printf("3. Replay a game\n");
    printf("4. Exit\n");
}

void runMainMenu(ExecuteMainSelectionFunction executeSelection, byte* currentBoardSize) {
    unsigned short currentChoice = 0;

    while (1) {
        clearScreen();

        printMainMenu();

        printf("> ");
        scanf("%hu", &currentChoice);

        byte shouldContinue = executeSelection(currentChoice, currentBoardSize);

        if (shouldContinue == 0) return;
    }
}

static void printReplayMenu() {
    printf("1. To beginning of game\n");
    printf("2. Backwards\n");
    printf("3. Forwards\n");
    printf("4. To end of game\n");
    printf("5. Exit\n");
}

void runReplayMenu(ExecuteReplaySelectionFunction executeSelection, FILE* file, Vector* pieceStartingPositions, Vector** board, Vector** pieces, Vector** moves) {
    unsigned short currentChoice = 0;

    while (1) {
        clearScreen();

        printBoard(*board);

        Piece *whiteKing = getPieceByName(*pieces, "KG");
        Piece *blackKing = getPieceByName(*pieces, "kg");

        if (whiteKing == NULL || blackKing == NULL) {
            printf("Failed to find kings\n");

            exit(1);
        }

        if (isInCheckmate(*pieces, blackKing, *board) || isInCheckmate(*pieces, whiteKing, *board)) {
            printf("Game Over: Checkmate\n");
        } else if (isInStalemate(*pieces, blackKing, *board) || isInStalemate(*pieces, whiteKing, *board)) {
            printf("Game Over: Stalemate\n");
        }

        printReplayMenu();

        printf("> ");
        scanf("%hu", &currentChoice);

        byte shouldContinue = executeSelection(currentChoice, file, pieceStartingPositions, board, pieces, moves);

        if (shouldContinue == 0) return;
    }
}