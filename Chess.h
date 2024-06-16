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
    KING = 1,
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

#define PIECE_COUNT 4

// Our current order of placement:
// white rook 1, white rook 2, black king, white king
PieceType pieceTypes[PIECE_COUNT] = {ROOK_1, ROOK_2, KING, KING};
byte isWhite[PIECE_COUNT] = {1, 1, 0, 1};

void freeBoard(Vector* board);

Coordinate* createCoordinate(byte x, byte y);

Tile* getTileFromBoard(Vector* board, byte x, byte y);

byte movesPerPiece(Vector* moves, Piece* piece);

byte capturedPieces(Vector* moves);

Vector* getLegalMoves(Vector* pieces, Piece* piece, Vector* board);

byte isInCheck(Vector* pieces, Piece* piece, Vector* board);

byte isInStalemate(Vector* pieces, Piece* piece, Vector* board);

byte isInCheckmate(Vector* pieces, Piece* piece, Vector* board);

Piece* getPieceByName(Vector* pieces, char* pieceName);

void makeMove(Vector* moves, Vector* board, Piece* piece, Coordinate end);

void undoMove(Vector* moves, Vector* board);

void runChessGame(byte boardSize);

void initializeReplay(byte boardSize, Vector* pieceStartPositions, Vector** boardVector, Vector** piecesVector, Vector** movesVector);