/*
 * board_interface.h
 *
 *  Created on: Nov 12, 2020
 *      Author: Matthew
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "graphics.h"
#include "utility.h"

#define MAX_X  8
#define MAX_Y  8

#define EMPTY 0
#define PAWN 1
#define BISHOP 2
#define KNIGHT 3
#define ROOK 4
#define QUEEN 5
#define KING 6

struct Piece{
    char type;
    char team;
};

struct Space{
    char x;
    char y;
};

int self_team;

struct Space translate(struct Space current);
// Used to translate a space used by the opponent to be read as the player's orientation

void init_Board_White_Bottom(void);
// Initializes the Board matrix with the correct pieces with player using white

void init_Board_Pawn_Test(void);
// Basic two-piece setup for initial testing

void init_Board_Black_Bottom(void);
// Initializes the Board matrix with the correct pieces with player using black

void Pos_select_enable(void);
// Enables the pins to be used for selecting the current active hall effect sensor

void castling(int changed[]);
// Translates board movement into castling movement in the board matrix

void en_passant(int changed[]);
// Translates board movement in en passant movement in the board matrix

void timer_enable(void);
// Enables the timer for use with drawing screens

void usart_enable(void);

void TIM2_IRQHandler(void);

void USART2_IRQHandler(void);

void TIM5_IRQHandler(void);
// Handles redrawing of the Active Piece

void update_position(void);
// Uses the "teams" matrix to update the positions of the virtual board to match
// the physical board. Calls castling() and en_passant() as needed

int checkEnemy(int row, int col);

int checkEmpty(int row, int col);

void drawPossibleMoves(int piece, int row, int col, int currentTeam);

int getSpaces(struct Space * spaces, int row, int col, int maxDist, char directions[], int numDirections);


void EXTI0_IRQHandler(void);
// Sends moves when signaled by the external button

void Button_enable(void);
// Enables the user Pushbutton on PA0 for interrupts

void ADC_enable(void);
// Initializes the ADC for use in reading Hall Effect sensor values

int ADC_val(void);
// Gets the current value of the ADC and returns the team that would be represented
// by that magnetic polarity, or lack thereof

int check_for_pickup(int * row, int * col);
// Checks the board to see what positions have been modified, and returns
// the type of piece that has been picked up by the player

void update_board(void);
// Updates the "teams" matrix with the values given by ADC_val() at every space

void exec_external_move(int init_x, int init_y, int end_x, int end_y);


#endif /* BOARD_H_ */
