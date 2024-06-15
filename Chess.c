#include "Chess.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Menu.h"
#include "Vector.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

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

void printTile(Tile *tile) {
    if (tile->type == POSSIBLE_MOVE) {
        printf("(");
    } else {
        printf(" ");
    }

    if (tile->piece == NULL) {
        printf("  ");
    } else if (tile->piece->isWhite) {
        switch (tile->piece->type) {
            case KING:
                printf("KG");
                break;
            case ROOK_1:
                printf("R1");
                break;
            case ROOK_2:
                printf("R2");
                break;
            default:
                printf("??");
                break;
        }
    } else {
        switch (tile->piece->type) {
            case KING:
                printf("kg");
                break;
            // rooks are not needed here
            // but we can still add them
            case ROOK_1:
                printf("r1");
                break;
            case ROOK_2:
                printf("r2");
                break;
            default:
                printf("??");
                break;
        }
    }

    if (tile->type == POSSIBLE_MOVE) {
        printf(")");
    } else {
        printf(" ");
    }
}

// If you use the simplified table
// you will not see your possible moves
// to save space in the console
void printTileSimplified(Tile *tile) {
    if (tile->piece == NULL) {
        printf("--");
    } else if (tile->piece->isWhite) {
        switch (tile->piece->type) {
            case KING:
                printf("KG");
                break;
            case ROOK_1:
                printf("R1");
                break;
            case ROOK_2:
                printf("R2");
                break;
            default:
                printf("??");
                break;
        }
    } else {
        switch (tile->piece->type) {
            case KING:
                printf("kg");
                break;
            // rooks are not needed here
            default:
                printf("??");
                break;
        }
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

void printBoardAsTable(Vector *board) {
    printHorizontalBorder(board->length + 1);

    printHorizontalNumbers(board->length);

    for (byte i = 0; i < board->length; i++) {
        // top border
        printHorizontalBorder(board->length + 1);

        // add the actual cells
        for (byte j = 0; j < board->length; j++) {
            // print the index as a cell before that
            if (j == 0) {
                printf("| %02d ", i + 1);
            }

            printf("|");

            Tile *currentTile = getTileFromBoard(board, i, j);

            printTile(currentTile);
        }
        printf("|\n");
    }

    // bottom border
    printHorizontalBorder(board->length + 1);
}

void printBoardAsSimplifiedTable(Vector *board) {
    for (byte i = 0; i < board->length; i++) {
        // print the top numbers
        if (i == 0) {
            for (byte j = 0; j < board->length + 1; j++) {
                printf("%02d ", j);
            }
            printf("\n");
        }

        for (byte j = 0; j < board->length; j++) {
            // print the index
            if (j == 0) {
                printf("%02d ", i + 1);
            }

            Tile *currentTile = getTileFromBoard(board, i, j);

            if (currentTile->piece != NULL) {
                printTileSimplified(currentTile);
            } else {
                printf("--");
            }

            printf(" ");
        }

        printf("\n");
    }
}

void printBoard(Vector *board) {
    // ! add this back later
    // clearScreen();

    printf("You play as white\n");
    printf("White pieces have uppercase letters\n");
    printf("Black pieces have lowercase letters\n");
    printf("White king: KG\n");
    printf("White rook 1: R1\n");
    printf("White rook 2: R2\n");
    printf("Black king: kg\n");

    if (board->length <= 20) {
        printf("\n");

        printBoardAsTable(board);
    } else {
        printf("Empty spaces are marked with --\n\n");

        printBoardAsSimplifiedTable(board);
    }
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

    // if the only pieces left are the two kings
    // it is a stalemate
    if (pieces->length == 2) {
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

void placePiecesRandomly(Vector *board, Vector *pieces) {
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

        // printf("%d %d\n",x,y);
        currentPiece->tile = currentTile;
        currentTile->piece = currentPiece;
    }
}

void makeMove(Vector *moves, Vector *board, Piece *piece, Coordinate end) {
    // check if the move is a capture
    Tile *endTile = getTileFromBoard(board, end.x, end.y);

    Piece *pieceTaken = NULL;

    if (endTile->piece != NULL) {
        pieceTaken = endTile->piece;
    }

    // move the piece
    Tile *startTile = piece->tile;

    startTile->piece = NULL;
    endTile->piece = piece;

    piece->tile = endTile;

    Move *move = createMove(piece, startTile->position, end, pieceTaken);

    moves->push(moves, move);
}

// ! free memory
void makeLegalMove(Vector *moves, Vector *pieces, Vector *board) {
    // ask the user which piece to move
    // here we enter the piece as a string
    // ie. KG for white king, R1 for white rook 1
    printf("Which piece do you want to move? [KG/R1/R2]\n> ");
    char piece[3];
    scanf("%2s", piece);

    Piece *selectedPiece = NULL;

    for (byte i = 0; i < pieces->length; i++) {
        Piece *currentPiece = pieces->get(pieces, i);

        if (currentPiece->isWhite) {
            if ((currentPiece->type == KING && strcmp(piece, "KG") == 0) ||
                (currentPiece->type == ROOK_1 && strcmp(piece, "R1") == 0) ||
                (currentPiece->type == ROOK_2 && strcmp(piece, "R2") == 0)) {
                selectedPiece = currentPiece;
                break;
            }
        }
    }

    if (selectedPiece == NULL) {
        printf("Invalid piece\n");
        waitForEnter();
        return;
    }

    Vector *legalMoves = getLegalMoves(pieces, selectedPiece, board);

    // mark all tiles with possible moves
    for (byte i = 0; i < legalMoves->length; i++) {
        Coordinate *currentMove = legalMoves->get(legalMoves, i);

        Tile *currentTile = getTileFromBoard(board, currentMove->x, currentMove->y);
        currentTile->type = POSSIBLE_MOVE;
    }

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
        waitForEnter();
        free(newCoordinate);
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

    lastMove->piece->tile = startTile;

    free(lastMove);
}

void printMove(Move *move) {
    printf("%s %s ", move->piece->isWhite ? "White" : "Black", move->piece->type == KING ? "king" : "rook");
    printf("(%d, %d) -> ", move->start.x + 1, move->start.y + 1);
    printf("(%d, %d)", move->end.x + 1, move->end.y + 1);

    if (move->pieceTaken != NULL) {
        printf(" (piece taken: %s %s)", move->pieceTaken->isWhite ? "white" : "black", move->pieceTaken->type == KING ? "king" : "rook");
    }

    printf("\n");
}

void runChessGame(byte boardSize) {
    Vector *board = createBoard(boardSize);

    Vector *pieces = initVector();

    pieces->push(pieces, createPiece(board, 0, 0, ROOK_1, 1));
    pieces->push(pieces, createPiece(board, 0, 0, ROOK_2, 1));
    // ! for testing purposes
    pieces->push(pieces, createPiece(board, 0, 0, ROOK_1, 0));
    pieces->push(pieces, createPiece(board, 0, 0, KING, 0));
    pieces->push(pieces, createPiece(board, 0, 0, KING, 1));

    // testing positions
    // pieces->push(pieces, createPiece(board, 5, 4, ROOK_1, 1));
    // pieces->push(pieces, createPiece(board, 3, 7, ROOK_2, 1));
    // pieces->push(pieces, createPiece(board, 3, 0, KING, 1));
    // pieces->push(pieces, createPiece(board, 5, 1, KING, 0));

    // pieces->push(pieces, createPiece(board, 0, 0, ROOK_1, 1));
    // pieces->push(pieces, createPiece(board, 6, 6, ROOK_2, 1));
    // pieces->push(pieces, createPiece(board, 0, 1, KING, 1));
    // pieces->push(pieces, createPiece(board, 7, 7, KING, 0));

    placePiecesRandomly(board, pieces);

    Vector *moves = initVector();

    while (1) {
        printBoard(board);

        // ! Use a better way to get the black king
        if (isInCheckmate(pieces, pieces->get(pieces, 3), board)) {
            printf("Checkmate\n");
            break;
        }

        if (isInCheck(pieces, pieces->get(pieces, 3), board)) {
            printf("Check\n");
        }

        if (isInStalemate(pieces, pieces->get(pieces, 3), board)) {
            printf("Stalemate\n");
            break;
        }

        makeLegalMove(moves, pieces, board);

        // remove all possible moves
        for (byte i = 0; i < board->length; i++) {
            Vector *row = board->get(board, i);

            for (byte j = 0; j < row->length; j++) {
                Tile *currentTile = row->get(row, j);

                if (currentTile->type == POSSIBLE_MOVE) {
                    currentTile->type = NORMAL;
                }
            }
        }

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