#include "Chess.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"
#include "Menu.h"
#include "Printing.h"
#include "Replay.h"
#include "Vector.h"

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

Tile *createTile(TileType type, byte x, byte y) {
    Tile *tile = (Tile *)malloc(sizeof(Tile));

    if (tile == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    tile->position.x = x;
    tile->position.y = y;
    tile->type = type;
    tile->piece = NULL;

    return tile;
}

Vector *createBoard(byte boardSize) {
    if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
        printf("Invalid board size\n");
        exit(1);
    }

    Vector *board = initVector();

    for (byte i = 0; i < boardSize; i++) {
        Vector *row = initVector();

        for (byte j = 0; j < boardSize; j++) {
            row->push(row, createTile(NORMAL, i, j));
        }

        board->push(board, row);
    }

    return board;
}

Tile *getTileFromBoard(Vector *board, byte x, byte y) {
    Vector *row = board->get(board, x);
    Tile *tile = row->get(row, y);

    return tile;
}

Piece *createPiece(Vector *board, byte x, byte y, PieceType type, byte isWhite) {
    Piece *piece = (Piece *)malloc(sizeof(Piece));

    if (piece == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    piece->tile = getTileFromBoard(board, x, y);
    piece->type = type;
    piece->isWhite = isWhite;
    piece->isTaken = 0;

    // ! used for debugging
    // piece->tile->piece = piece;

    return piece;
}

Move *createMove(Piece *piece, Coordinate start, Coordinate end, Piece *pieceTaken) {
    Move *move = (Move *)malloc(sizeof(Move));

    if (move == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    move->piece = piece;
    move->start = start;
    move->end = end;
    move->pieceTaken = pieceTaken;

    return move;
}

Piece *getPieceByName(Vector *pieces, char *pieceName) {
    for (byte i = 0; i < pieces->length; i++) {
        Piece *currentPiece = pieces->get(pieces, i);

        if (currentPiece->isWhite) {
            if ((currentPiece->type == KING && strcmp(pieceName, "KG") == 0) ||
                (currentPiece->type == ROOK_1 && strcmp(pieceName, "R1") == 0) ||
                (currentPiece->type == ROOK_2 && strcmp(pieceName, "R2") == 0)) {
                return currentPiece;
            }
        } else {
            if ((currentPiece->type == KING && strcmp(pieceName, "kg") == 0)) {
                return currentPiece;
            }
        }
    }

    return NULL;
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

Vector *getPossibleMoves(Piece *piece, Vector *board) {
    Vector *possibleMoves = initVector();

    if (piece->type == ROOK_1 || piece->type == ROOK_2) {
        // left movement
        Coordinate newCoordinate = piece->tile->position;
        // we do these offsets to avoid the piece itself
        newCoordinate.x--;

        while (!isOutOfBounds(newCoordinate, board->length)) {
            possibleMoves->push(possibleMoves, createCoordinate(newCoordinate.x, newCoordinate.y));

            Tile *currentTile = getTileFromBoard(board, newCoordinate.x, newCoordinate.y);

            if (currentTile->piece == NULL || (currentTile->piece->type == KING && currentTile->piece->isWhite != piece->isWhite)) {
                newCoordinate.x--;

                continue;
            }

            break;
        }

        // right movement
        newCoordinate = piece->tile->position;
        newCoordinate.x++;

        while (!isOutOfBounds(newCoordinate, board->length)) {
            possibleMoves->push(possibleMoves, createCoordinate(newCoordinate.x, newCoordinate.y));

            Tile *currentTile = getTileFromBoard(board, newCoordinate.x, newCoordinate.y);

            if (currentTile->piece == NULL || (currentTile->piece->type == KING && currentTile->piece->isWhite != piece->isWhite)) {
                newCoordinate.x++;

                continue;
            }

            break;
        }

        // up movement
        newCoordinate = piece->tile->position;
        newCoordinate.y--;

        while (!isOutOfBounds(newCoordinate, board->length)) {
            possibleMoves->push(possibleMoves, createCoordinate(newCoordinate.x, newCoordinate.y));

            Tile *currentTile = getTileFromBoard(board, newCoordinate.x, newCoordinate.y);

            if (currentTile->piece == NULL || (currentTile->piece->type == KING && currentTile->piece->isWhite != piece->isWhite)) {
                newCoordinate.y--;

                continue;
            }

            break;
        }

        // down movement
        newCoordinate = piece->tile->position;
        newCoordinate.y++;

        while (!isOutOfBounds(newCoordinate, board->length)) {
            possibleMoves->push(possibleMoves, createCoordinate(newCoordinate.x, newCoordinate.y));

            Tile *currentTile = getTileFromBoard(board, newCoordinate.x, newCoordinate.y);

            if (currentTile->piece == NULL || (currentTile->piece->type == KING && currentTile->piece->isWhite != piece->isWhite)) {
                newCoordinate.y++;

                continue;
            }

            break;
        }
    } else if (piece->type == KING) {
        for (byte i = max(piece->tile->position.x - 1, 0); i <= piece->tile->position.x + 1; i++) {
            for (byte j = max(piece->tile->position.y - 1, 0); j <= piece->tile->position.y + 1; j++) {
                Coordinate *newCoordinate = createCoordinate(i, j);

                if (isOutOfBounds(*newCoordinate, board->length) || coordinatesMatch(*newCoordinate, piece->tile->position)) {
                    free(newCoordinate);

                    continue;
                }

                possibleMoves->push(possibleMoves, newCoordinate);
            }
        }
    }

    return possibleMoves;
}

Vector *getLegalMoves(Vector *pieces, Piece *piece, Vector *board) {
    Vector *possibleMoves = getPossibleMoves(piece, board);

    // we technically need to check only the black king
    // since the black king is the only piece that black has
    // and it can't move to a place where it is in check
    // so it can't check the white king
    if (piece->type == KING) {
        Vector *allEnemyMoves = initVector();

        for (byte i = 0; i < pieces->length; i++) {
            Piece *currentPiece = pieces->get(pieces, i);

            if (currentPiece->isTaken) {
                continue;
            }

            if (currentPiece->isWhite != piece->isWhite) {
                Vector *currentEnemyMoves = getPossibleMoves(currentPiece, board);

                for (byte j = 0; j < currentEnemyMoves->length; j++) {
                    allEnemyMoves->push(allEnemyMoves, currentEnemyMoves->get(currentEnemyMoves, j));
                }

                freeVector(currentEnemyMoves);
            }
        }

        for (byte i = 0; i < possibleMoves->length; i++) {
            Coordinate *currentMove = possibleMoves->get(possibleMoves, i);

            // if the move is in the enemy moves it is illegal
            for (byte j = 0; j < allEnemyMoves->length; j++) {
                Coordinate *currentEnemyMove = allEnemyMoves->get(allEnemyMoves, j);

                if (coordinatesMatch(*currentMove, *currentEnemyMove)) {
                    possibleMoves->popIndex(possibleMoves, i--);

                    free(currentMove);

                    break;
                }
            }
        }

        for (byte i = 0; i < allEnemyMoves->length; i++) {
            free(allEnemyMoves->get(allEnemyMoves, i));
        }

        freeVector(allEnemyMoves);
    }

    for (byte i = 0; i < possibleMoves->length; i++) {
        Coordinate *currentMove = possibleMoves->get(possibleMoves, i);

        Tile *currentTile = getTileFromBoard(board, currentMove->x, currentMove->y);

        if (currentTile->piece != NULL && piece->isWhite == currentTile->piece->isWhite) {
            possibleMoves->popIndex(possibleMoves, i--);

            free(currentMove);
        }
    }

    return possibleMoves;
}

// If the king is in the enemy's possible moves
// it is in check
byte isInCheck(Vector *pieces, Piece *piece, Vector *board) {
    byte isInCheck = 0;

    if (piece->type != KING) {
        return isInCheck;
    }

    Vector *allEnemyMoves = initVector();

    for (byte i = 0; i < pieces->length; i++) {
        Piece *currentPiece = pieces->get(pieces, i);

        if (currentPiece->isTaken) {
            continue;
        }

        if (currentPiece->isWhite != piece->isWhite) {
            Vector *currentEnemyMoves = getPossibleMoves(currentPiece, board);

            for (byte j = 0; j < currentEnemyMoves->length; j++) {
                allEnemyMoves->push(allEnemyMoves, currentEnemyMoves->get(currentEnemyMoves, j));
            }

            freeVector(currentEnemyMoves);
        }
    }

    for (byte i = 0; i < allEnemyMoves->length; i++) {
        Coordinate *currentEnemyMove = allEnemyMoves->get(allEnemyMoves, i);

        if (coordinatesMatch(*currentEnemyMove, piece->tile->position)) {
            isInCheck = 1;
        }
    }

    for (byte i = 0; i < allEnemyMoves->length; i++) {
        free(allEnemyMoves->get(allEnemyMoves, i));
    }

    freeVector(allEnemyMoves);

    return isInCheck;
}

byte isInStalemate(Vector *pieces, Piece *piece, Vector *board) {
    byte isInStalemate = 0;

    if (piece->type != KING) {
        return isInStalemate;
    }

    // if the only pieces not taken are the two kings
    // it is a stalemate
    byte piecesNotTaken = 0;

    for (byte i = 0; i < pieces->length; i++) {
        Piece *currentPiece = pieces->get(pieces, i);

        if (!currentPiece->isTaken) {
            piecesNotTaken++;
        }
    }

    if (piecesNotTaken == 2) {
        return 1;
    }

    // if the piece has no possible moves
    // it is a stalemate
    Vector *possibleMoves = getLegalMoves(pieces, piece, board);

    if (possibleMoves->length == 0) {
        isInStalemate = 1;
    }

    for (byte i = 0; i < possibleMoves->length; i++) {
        free(possibleMoves->get(possibleMoves, i));
    }

    freeVector(possibleMoves);

    return isInStalemate;
}

// If the piece is both in check and in stalemate
// it is a checkmate
byte isInCheckmate(Vector *pieces, Piece *piece, Vector *board) {
    return isInCheck(pieces, piece, board) && isInStalemate(pieces, piece, board);
}

// This function places the pieces randomly on the board
// and returns a seed for the replay file
byte *placePiecesRandomly(Vector *board, Vector *pieces) {
    byte currentSeedLength = 0;
    byte *seed = calloc(sizeof(byte), calculateSeedLength(pieces->length));

    if (seed == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    // write the board size to the seed
    seed[currentSeedLength++] = board->length;

    // write the piece count to the seed
    seed[currentSeedLength++] = pieces->length;

    for (byte i = 0; i < pieces->length; i++) {
        Piece *currentPiece = pieces->get(pieces, i);

        byte x, y;
        Tile *currentTile;

        do {
            x = rand() % board->length;
            y = rand() % board->length;

            currentTile = getTileFromBoard(board, x, y);

            if (currentTile->piece != NULL) {
                continue;
            }

            currentPiece->tile = currentTile;
        } while (currentTile->piece != NULL || (currentPiece->type == KING && isInCheck(pieces, currentPiece, board)));

        currentPiece->tile = currentTile;
        currentTile->piece = currentPiece;

        // write the starting coordinates to the seed
        seed[currentSeedLength++] = x;
        seed[currentSeedLength++] = y;
    }

    return seed;
}

void makeMove(Vector *moves, Vector *board, Piece *piece, Coordinate end) {
    // check if the move is a capture
    Tile *endTile = getTileFromBoard(board, end.x, end.y);

    Piece *pieceTaken = NULL;

    if (endTile->piece != NULL) {
        pieceTaken = endTile->piece;
        pieceTaken->isTaken = 1;
    }

    // move the piece
    Tile *startTile = piece->tile;

    startTile->piece = NULL;
    endTile->piece = piece;

    piece->tile = endTile;

    Move *move = createMove(piece, startTile->position, end, pieceTaken);

    moves->push(moves, move);
}

// sets the type of the tiles to POSSIBLE_MOVE
// for all legal moves of the piece
void setPossibleMoves(Vector *board, Vector *legalMoves) {
    for (byte i = 0; i < legalMoves->length; i++) {
        Coordinate *currentMove = legalMoves->get(legalMoves, i);

        Tile *currentTile = getTileFromBoard(board, currentMove->x, currentMove->y);
        currentTile->type = POSSIBLE_MOVE;
    }
}

void removePossibleMoves(Vector *board) {
    for (byte i = 0; i < board->length; i++) {
        Vector *row = board->get(board, i);

        for (byte j = 0; j < row->length; j++) {
            Tile *currentTile = row->get(row, j);

            if (currentTile->type == POSSIBLE_MOVE) {
                currentTile->type = NORMAL;
            }
        }
    }
}

// ! free memory
void makeLegalMove(Vector *moves, Vector *pieces, Vector *board) {
    // ask the user which piece to move
    // here we enter the piece as a string
    // ie. KG for white king, R1 for white rook 1
    // ! make sure the user can't move the black pieces
    printf("Which piece do you want to move? [KG/R1/R2]\n> ");
    char pieceName[3];
    scanf("%2s", pieceName);

    Piece *selectedPiece = getPieceByName(pieces, pieceName);

    if (selectedPiece == NULL) {
        printf("Invalid piece\n");

        waitForEnter();
        return;
    }

    Vector *legalMoves = getLegalMoves(pieces, selectedPiece, board);

    // mark all tiles with possible moves
    setPossibleMoves(board, legalMoves);

    printBoard(board);

    // ask the user where to move
    printf("Where do you want to move the piece? [row, column]\n> ");
    unsigned short row, column;

    // %hhu is not reliable so we use this
    if (scanf("%hu, %hu", &row, &column) != 2) {
        printf("Invalid input format. Please enter as [row, column]\n");

        waitForEnter();
        return;
    }

    // check if row or column isn't within uint8_t range
    if (row > UCHAR_MAX || column > UCHAR_MAX) {
        printf("Invalid input\n");

        waitForEnter();
        return;
    }

    Coordinate *newCoordinate = createCoordinate(row - 1, column - 1);

    // check if the move is legal
    byte isLegal = 0;

    for (byte i = 0; i < legalMoves->length; i++) {
        Coordinate *currentMove = legalMoves->get(legalMoves, i);

        if (coordinatesMatch(*currentMove, *newCoordinate)) {
            isLegal = 1;
            break;
        }
    }

    if (!isLegal) {
        printf("Illegal move\n");

        free(newCoordinate);

        waitForEnter();
        return;
    }

    for (byte i = 0; i < legalMoves->length; i++) {
        free(legalMoves->get(legalMoves, i));
    }

    freeVector(legalMoves);

    makeMove(moves, board, selectedPiece, *newCoordinate);

    free(newCoordinate);
}

void undoMove(Vector *moves, Vector *board) {
    if (moves->length == 0) {
        printf("No moves to undo\n");
        waitForEnter();

        return;
    }

    Move *lastMove = moves->pop(moves);

    Tile *startTile = getTileFromBoard(board, lastMove->start.x, lastMove->start.y);
    Tile *endTile = getTileFromBoard(board, lastMove->end.x, lastMove->end.y);

    startTile->piece = lastMove->piece;
    endTile->piece = lastMove->pieceTaken;
    lastMove->pieceTaken->isTaken = 0;

    lastMove->piece->tile = startTile;

    free(lastMove);
}

void runChessGame(byte boardSize) {
    Vector *board = createBoard(boardSize);

    Vector *pieces = initVector();

    pieces->push(pieces, createPiece(board, 0, 0, ROOK_1, 1));
    pieces->push(pieces, createPiece(board, 0, 0, ROOK_2, 1));
    // ! for testing purposes
    // pieces->push(pieces, createPiece(board, 0, 0, ROOK_1, 0));
    pieces->push(pieces, createPiece(board, 0, 0, KING, 0));
    pieces->push(pieces, createPiece(board, 0, 0, KING, 1));

    byte *seed = placePiecesRandomly(board, pieces);

    Vector *moves = initVector();

    Piece *whiteKing = getPieceByName(pieces, "KG");
    Piece *blackKing = getPieceByName(pieces, "kg");

    if (whiteKing == NULL || blackKing == NULL) {
        printf("Failed to find kings\n");

        exit(1);
    }

    while (1) {
        printBoard(board);

        if (isInCheckmate(pieces, blackKing, board) || isInCheckmate(pieces, whiteKing, board)) {
            printf("Checkmate\n");

            writeReplayToFile(seed, calculateSeedLength(pieces->length), moves);

            break;
        }

        if (isInCheck(pieces, blackKing, board) || isInCheckmate(pieces, whiteKing, board)) {
            printf("Check\n");
        }

        if (isInStalemate(pieces, blackKing, board) || isInCheckmate(pieces, whiteKing, board)) {
            printf("Stalemate\n");

            writeReplayToFile(seed, calculateSeedLength(pieces->length), moves);

            break;
        }

        makeLegalMove(moves, pieces, board);

        removePossibleMoves(board);

        clearScreen();

        if (moves->length > 0) {
            printf("Your move: ");
            printMove(moves->get(moves, moves->length - 1));
        }
    }

    // free memory
    for (byte i = 0; i < moves->length; i++) {
        free(moves->get(moves, i));
    }

    freeVector(moves);

    for (byte i = 0; i < pieces->length; i++) {
        free(pieces->get(pieces, i));
    }

    freeVector(pieces);

    for (byte i = 0; i < board->length; i++) {
        Vector *row = board->get(board, i);

        for (byte j = 0; j < row->length; j++) {
            free(row->get(row, j));
        }

        freeVector(row);
    }

    freeVector(board);
}