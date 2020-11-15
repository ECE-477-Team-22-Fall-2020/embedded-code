/*
 * piece_list.h
 *
 *  Created on: Nov 15, 2020
 *      Author: Ian Haggerty
 */

#ifndef PIECE_LIST_H_
#define PIECE_LIST_H_

#include "graphics.h"

#define NUM_PIECES 32
#define LAST_PIECE -1
#define NULL_PIECE -2

// need to rename eventually to not confilct with Matt's struct
typedef struct PieceType {
	int row; // zero-indexed
	int col; // zero-indexed
	int id;
	int team;
} Piece;

typedef struct PieceNodeType {
	Piece piece;
	int next; // index of next piece in array
} PieceNode;

PieceNode pieces[NUM_PIECES];
int firstIndex = -1;
int lastIndex = -1;

Piece getPiece(int row, int col, int id);
int findPiece(Piece piece);
void addPiece(Piece piece);
void removePiece(Piece piece);
void resetPieces();
int pieceAlloc();
int pieceCompare(Piece a, Piece b);

#endif /* PIECE_LIST_H_ */
