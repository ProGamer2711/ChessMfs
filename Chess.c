#include "Chess.h"

#include <stdio.h>

#include "Menu.h"
#include "Vector.h"

Piece *createPiece(byte x, byte y, byte type) {
    Piece *piece = (Piece *)malloc(sizeof(Piece));

    if (piece == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    piece->position.x = x;
    piece->position.y = y;
    piece->type = type;

    return piece;
}

// ! Move this later
Coordinate *createCoordinate(byte x, byte y) {
    Coordinate *coordinate = (Coordinate *)malloc(sizeof(Coordinate));

    if (coordinate == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    coordinate->x = x;
    coordinate->y = y;

    return coordinate;
}

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

byte isOutOfBounds(Coordinate coordinate, byte boardSize) {
    if (coordinate.x < 0 || coordinate.x >= boardSize || coordinate.y < 0 || coordinate.y >= boardSize) {
        return 1;
    }

    return 0;
}

byte coordinatesMatch(Coordinate coordinate1, Coordinate coordinate2) {
    if (coordinate1.x == coordinate2.x && coordinate1.y == coordinate2.y) {
        return 1;
    }

    return 0;
}

void freeBoard(byte **board, byte boardSize) {
    for (byte i = 0; i < boardSize; i++) {
        free(board[i]);
    }

    free(board);
}

void placePiecesRandomly(byte **board, byte boardSize, Vector *pieces) {
    for (byte i = 0; i < pieces->length; i++) {
        Piece *currentPiece = pieces->get(pieces, i);

        byte x = rand() % boardSize;
        byte y = rand() % boardSize;

        // ! Check validity of the placement
        while (board[x][y] != EMPTY_SPACE) {
            x = rand() % boardSize;
            y = rand() % boardSize;
        }

        currentPiece->position.x = x;
        currentPiece->position.y = y;

        board[x][y] = currentPiece->type;
    }
}

void printCell(byte cell) {
    switch (cell) {
        case EMPTY_SPACE:
            printf("  ");
            break;
        case POSSIBLE_MOVE:
            printf("()");
            break;
        case WHITE_KING:
            printf("KG");
            break;
        case WHITE_ROOK_1:
            printf("R1");
            break;
        case WHITE_ROOK_2:
            printf("R2");
            break;
        case BLACK_KING:
            printf("kg");
            break;
        default:
            printf("??");
            break;
    }
}

void printHorizontalBorder(byte boardSize) {
    for (byte j = 0; j < boardSize; j++) {
        printf("+----");
    }
    printf("+\n");
}

void printHorizontalNumbers(byte boardSize) {
    // add one extra to start from 00 with an extra cell
    // so we can have row numbers too
    for (byte j = 0; j < boardSize + 1; j++) {
        printf("| %02d ", j);
    }

    printf("|\n");
}

void printBoardAsTable(byte **board, byte boardSize) {
    printHorizontalBorder(boardSize + 1);

    printHorizontalNumbers(boardSize);

    for (byte i = 0; i < boardSize; i++) {
        // top border
        printHorizontalBorder(boardSize + 1);

        // add the actual cells
        for (byte j = 0; j < boardSize; j++) {
            // print the index as a cell before that
            if (j == 0) {
                printf("| %02d ", i + 1);
            }

            printf("| ");
            printCell(board[i][j]);
            printf(" ");
        }
        printf("|\n");
    }

    // bottom border
    printHorizontalBorder(boardSize + 1);
}

void printBoardAsSimplifiedTable(byte **board, byte boardSize) {
    for (byte i = 0; i < boardSize; i++) {
        // print the top numbers
        if (i == 0) {
            for (byte j = 0; j < boardSize + 1; j++) {
                printf("%02d ", j);
            }
            printf("\n");
        }

        for (byte j = 0; j < boardSize; j++) {
            // print the index
            if (j == 0) {
                printf("%02d ", i + 1);
            }

            if (board[i][j] != EMPTY_SPACE) {
                printCell(board[i][j]);
            } else {
                printf("--");
            }

            printf(" ");
        }

        printf("\n");
    }
}

void printBoard(byte **board, byte boardSize) {
    clearScreen();

    printf("You play as white\n");
    printf("White pieces have uppercase letters\n");
    printf("Black pieces have lowercase letters\n");
    printf("White king: KG\n");
    printf("White rook 1: R1\n");
    printf("White rook 2: R2\n");
    printf("Black king: kg\n");

    if (boardSize <= 20) {
        printf("\n");

        printBoardAsTable(board, boardSize);
    } else {
        printf("Empty spaces are marked with --\n\n");

        printBoardAsSimplifiedTable(board, boardSize);
    }
}

Vector *getPossibleMoves(Piece *piece, byte **board, byte boardSize) {
    Vector *possibleMoves = initVector();

    if (piece->type == WHITE_ROOK_1 || piece->type == WHITE_ROOK_2) {
        // left movement
        Coordinate newCoordinate = piece->position;
        // we do these offsets to avoid the piece itself
        newCoordinate.x--;

        while (!isOutOfBounds(newCoordinate, boardSize)) {
            // in this case we don't need to check if it is an enemy piece
            // since the enemy only has a king and if it is in the way it is mate
            if (board[newCoordinate.x][newCoordinate.y] != EMPTY_SPACE && board[newCoordinate.x][newCoordinate.y] != BLACK_KING) {
                break;
            }

            possibleMoves->push(possibleMoves, createCoordinate(newCoordinate.x--, newCoordinate.y));
        }

        // right movement
        newCoordinate = piece->position;
        newCoordinate.x++;

        while (!isOutOfBounds(newCoordinate, boardSize)) {
            if (board[newCoordinate.x][newCoordinate.y] != EMPTY_SPACE && board[newCoordinate.x][newCoordinate.y] != BLACK_KING) {
                break;
            }

            possibleMoves->push(possibleMoves, createCoordinate(newCoordinate.x++, newCoordinate.y));
        }

        // up movement
        newCoordinate = piece->position;
        newCoordinate.y--;

        while (!isOutOfBounds(newCoordinate, boardSize)) {
            if (board[newCoordinate.x][newCoordinate.y] != EMPTY_SPACE && board[newCoordinate.x][newCoordinate.y] != BLACK_KING) {
                break;
            }

            possibleMoves->push(possibleMoves, createCoordinate(newCoordinate.x, newCoordinate.y--));
        }

        // down movement
        newCoordinate = piece->position;
        newCoordinate.y++;

        while (!isOutOfBounds(newCoordinate, boardSize)) {
            if (board[newCoordinate.x][newCoordinate.y] != EMPTY_SPACE && board[newCoordinate.x][newCoordinate.y] != BLACK_KING) {
                break;
            }

            possibleMoves->push(possibleMoves, createCoordinate(newCoordinate.x, newCoordinate.y++));
        }
    } else if (piece->type == BLACK_KING || piece->type == WHITE_KING) {

        //must be short or else we might have an underflow
        for (short i = piece->position.x - 1; i <= piece->position.x + 1; i++) {
            for (short j = piece->position.y - 1; j <= piece->position.y + 1; j++) {
                Coordinate *newCoordinate = createCoordinate(i, j);

                if (isOutOfBounds(*newCoordinate, boardSize) || coordinatesMatch(*newCoordinate, piece->position)) {
                    free(newCoordinate);
                    continue;
                }

                possibleMoves->push(possibleMoves, newCoordinate);
            }
        }
    }

    return possibleMoves;
}


Vector *getLegalMoves(Vector *pieces, Piece *piece, byte **board, byte boardSize) {
    Vector *possibleMoves = getPossibleMoves(piece, board, boardSize);

    // if the piece is a rook we don't need to check for anything else
    // since rooks don't have illegal moves
    if (piece->type == WHITE_ROOK_1 || piece->type == WHITE_ROOK_2) {
        return possibleMoves;
    }

    // we technically need to check only the black king
    // since the black king is the only piece that black has
    // and it can't move to a place where it is in check
    // so it can't check the white king 
    if (piece->type == BLACK_KING) {
        Vector *allEnemyMoves = initVector();

        for (byte i = 0; i < pieces->length; i++) {
            Piece *currentPiece = pieces->get(pieces, i);

            if (currentPiece->type != piece->type) {
                Vector *currentEnemyMoves = getPossibleMoves(currentPiece, board, boardSize);

                for (byte j = 0; j < currentEnemyMoves->length; j++) {
                    allEnemyMoves->push(allEnemyMoves, currentEnemyMoves->get(currentEnemyMoves, j));
                }
            }
        }

        for (byte i = 0; i < possibleMoves->length; i++) {
            Coordinate *currentMove = possibleMoves->get(possibleMoves, i);

            // if the move is in the enemy moves it is illegal
            for (byte j = 0; j < allEnemyMoves->length; j++) {
                Coordinate *currentEnemyMove = allEnemyMoves->get(allEnemyMoves, j);

                if (coordinatesMatch(*currentMove, *currentEnemyMove)) {
                    // remove the move (Malinov add this to the vector)
                    possibleMoves->popIndex(possibleMoves, i);

                    // We have removed this index so we must 
                    // go back one move to not skip the next one
                    i--;

                    break;
                }
            }
        }

        return possibleMoves;
    }

    return possibleMoves;
    // !TBD add for white king
}

void runChessGame(byte boardSize) {
    byte **board = generateBoard(boardSize);

    // use the pieces struct
    Vector *pieces = initVector();

    pieces->push(pieces, createPiece(0, 0, WHITE_KING));
    pieces->push(pieces, createPiece(0, 0, WHITE_ROOK_1));
    pieces->push(pieces, createPiece(0, 0, WHITE_ROOK_2));
    pieces->push(pieces, createPiece(0, 0, BLACK_KING));
    
    placePiecesRandomly(board, boardSize, pieces);


    // more stuff here
    Vector* legal=getLegalMoves(pieces, pieces->data[3], board, boardSize);



    for(byte i=0;i<legal->length;i++)
    {
        Coordinate* coord=legal->data[i];
        board[coord->x][coord->y]=POSSIBLE_MOVE;
        printf("|%d %d| ",coord->x,coord->y);

    }
    printBoard(board, boardSize);

    freeBoard(board, boardSize);
    


    for (int i = 0; i < pieces->length; i++) {
        free(pieces->get(pieces, i));
    }

    freeVector(pieces);
}
