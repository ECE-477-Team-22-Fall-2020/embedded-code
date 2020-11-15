/*
 * utility.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Ian Haggerty
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "stm32f4xx.h"

#define WHITE_TEAM 1
#define BLACK_TEAM 0

#define PAWN_ID 0
#define BISHOP_ID 1
#define KNIGHT_ID 2
#define ROOK_ID 3
#define QUEEN_ID 4
#define KING_ID 5

char * pawn_string = "Pawn";
char * bishop_string = "Bishop";
char * knight_string = "Knight";
char * rook_string = "Rook";
char * queen_string = "Queen";
char * king_string = "King";

char scoreString[] = "00-00";

void delay(int ms);
char * string_map(int id);
int scoreMap(int id);
char colMap(int row);
char rowMap(int row);
int teamY(int y, int team);

void resetScoreString(void);

#endif
