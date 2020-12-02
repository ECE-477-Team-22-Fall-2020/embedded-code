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

#define STRLEN(s) (sizeof(s)/sizeof(s[0]))

#define BLUETOOTH_BUFFER_SIZE 50

char * pawn_string;
char * bishop_string;
char * knight_string;
char * rook_string;
char * queen_string;
char * king_string;

char scoreString[6];

enum directions {n, s, e, w, nw, ne, sw, se, nullDirection};

char bluetooth_buffer[BLUETOOTH_BUFFER_SIZE];
uint8_t timer_count;
uint8_t buffer_index;
UART_HandleTypeDef huart2;

int strLen(char[]);
uint8_t stringCompare(char array1[], char array2[], uint16_t length);
void MessageHandler(void);
void delay(int ms);
int getEnemy(int team);
int checkInBounds(int row, int col);
int some(char list[], int length);
void getCoords(int * newRow, int * newCol, int row, int col, int dist, int direction);
char * stringMap(int id);
int scoreMap(int id);
char colMap(int row);
char rowMap(int row);
int teamY(int y, int team);
void resetScoreString(void);

#endif
