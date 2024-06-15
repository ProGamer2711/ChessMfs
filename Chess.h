#pragma once

#include "Constants.h"
#include "Vector.h"

// Board size can't be less than 4 for 4 pieces because of placement
#define MIN_BOARD_SIZE 4
// Could be any size but the best experience is up to 50
#define MAX_BOARD_SIZE 50

typedef struct Coordinate {
    byte x;
    byte y;
} Coordinate;

typedef enum PieceType {
    KING,
    ROOK_1,
    ROOK_2
} PieceType;

typedef enum TileType {
    NORMAL,
    POSSIBLE_MOVE,
} TileType;

typedef struct Tile Tile;

typedef struct Piece {
    PieceType type;
    byte isWhite;
    byte isTaken;
    Tile* tile;
} Piece;

struct Tile {
    TileType type;
    Coordinate position;
    Piece* piece;
};

typedef struct Move {
    Piece* piece;
    Coordinate start;
    Coordinate end;
    Piece* pieceTaken;
} Move;

void printBoard(Vector* board);

void runChessGame(byte boardSize);