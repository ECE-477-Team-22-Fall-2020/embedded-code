/*
 * board.c
 *
 *  Created on: Nov 12, 2020
 *      Author: Matthew
 */

#include "board.h"

/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

int teams[MAX_X][MAX_Y];
struct Piece board[MAX_X][MAX_Y];
struct Piece blank_space = {.type = EMPTY, .team = 0};
int self_team;


struct Space translate(struct Space current){
    current.x = 7 - current.x;
    current.y = 7 - current.y;
    return current;
}

void init_Board_White_Bottom(void){
    self_team = 1;
    struct Piece newPawn;
    newPawn.type = PAWN;
    struct Piece newBishop;
    newBishop.type = BISHOP;
    struct Piece newKnight;
    newKnight.type = KNIGHT;
    struct Piece newRook;
    newRook.type = ROOK;
    struct Piece newQueen;
    newQueen.type = QUEEN;
    struct Piece newKing;
    newKing.type = KING;
    int x;
    for(x=0; x < MAX_X; x++){
        newPawn.team = 1;
        board[x][1] = newPawn;
        newPawn.team = 2;
        board[x][6] = newPawn;
    }
    //Black Pieces
    newBishop.team = 1;
    board[2][0] = newBishop;
    board[5][0] = newBishop;
    newKnight.team = 1;
    board[1][0] = newKnight;
    board[6][0] = newKnight;
    newRook.team = 1;
    board[0][0] = newRook;
    board[7][0] = newRook;
    newQueen.team = 1;
    board[3][0] = newQueen;
    newKing.team = 1;
    board[4][0] = newKing;

    //White Pieces
    newBishop.team = 2;
    board[2][7] = newBishop;
    board[5][7] = newBishop;
    newKnight.team = 2;
    board[1][7] = newKnight;
    board[6][7] = newKnight;
    newRook.team = 2;
    board[0][7] = newRook;
    board[7][7] = newRook;
    newQueen.team = 2;
    board[3][7] = newQueen;
    newKing.team = 2;
    board[4][7] = newKing;
    return;
}

void init_Board_Pawn_Test(void){
    for( int x = 0; x < MAX_X; x++){
        for( int y = 0; y < MAX_Y; y++){
            board[x][y] = blank_space;
        }
    }
    struct Piece newPawn;
    newPawn.type = PAWN;
    newPawn.team = 1;
    board[0][1] = newPawn;
    board[5][1] = newPawn;
}

void init_Board_Black_Bottom(void){
    self_team = 2;
    struct Piece newPawn;
    newPawn.type = PAWN;
    struct Piece newBishop;
    newBishop.type = BISHOP;
    struct Piece newKnight;
    newKnight.type = KNIGHT;
    struct Piece newRook;
    newRook.type = ROOK;
    struct Piece newQueen;
    newQueen.type = QUEEN;
    struct Piece newKing;
    newKing.type = KING;
    int x;
    for(x=0; x < MAX_X; x++){
        newPawn.team = 2;
        board[x][1] = newPawn;
        newPawn.team = 1;
        board[x][6] = newPawn;
    }
    //White Pieces
    newBishop.team = 2;
    board[2][0] = newBishop;
    board[5][0] = newBishop;
    newKnight.team = 2;
    board[1][0] = newKnight;
    board[6][0] = newKnight;
    newRook.team = 2;
    board[0][0] = newRook;
    board[7][0] = newRook;
    newQueen.team = 2;
    board[4][0] = newQueen;
    newKing.team = 2;
    board[3][0] = newKing;

    //Black Pieces
    newBishop.team = 1;
    board[2][7] = newBishop;
    board[5][7] = newBishop;
    newKnight.team = 1;
    board[1][7] = newKnight;
    board[6][7] = newKnight;
    newRook.team = 1;
    board[0][7] = newRook;
    board[7][7] = newRook;
    newQueen.team = 1;
    board[4][7] = newQueen;
    newKing.team = 1;
    board[3][7] = newKing;
    return;
}

void Pos_select_enable(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER &= ~0xffff;
    GPIOB->MODER |= 0x5555;

    return;
}

void castling(int changed[]){
    //Consider that a king can castle both left and right
    board[changed[2]][changed[3]] = board[changed[6]][changed[7]];
    board[changed[4]][changed[5]] = board[changed[0]][changed[1]];
    board[changed[0]][changed[1]] = blank_space;
    board[changed[6]][changed[7]] = blank_space;
    return;
}

void en_passant(int changed[]){
    //Will probably pass in an array of changed spaces
    board[changed[2]][changed[3]] = board[changed[4]][changed[5]];
    board[changed[0]][changed[1]] = blank_space;
    board[changed[4]][changed[5]] = blank_space;
    return;
}

void update_position(void){

    int changed[] = {-1, -1, -1, -1, -1, -1, -1, -1};
    struct Piece temp;
    int num_change = 0;
    //Loop through the board and the team map, determine where the changes occur
    for(int x = 0; x < MAX_X; x++){
        for(int y = 0; y < MAX_Y; y++){
            if(teams[x][y] != board[x][y].team){
                changed[2 * num_change] = x;
                changed[(2 * num_change) + 1] = y;
                num_change++;
                //4 changes could be either en passant or castling
            }}}
    if(num_change == 4){
        castling(changed);
        return;
    }
    if(num_change == 3){
        en_passant(changed);
        return;
    }
    if(board[changed[0]][changed[1]].team != 0 && board[changed[2]][changed[3]].team != 0){
        //THIS IS WHERE CAPTURE HAPPENS
        if(teams[changed[0]][changed[1]] == 0){
            board[changed[2]][changed[3]] = board[changed[0]][changed[1]];
            board[changed[0]][changed[1]] = blank_space;
        }else{
            board[changed[0]][changed[1]] = board[changed[2]][changed[3]];
            board[changed[2]][changed[3]] = blank_space;
        }
    }else{
        temp = board[changed[0]][changed[1]];
        board[changed[0]][changed[1]] = board[changed[2]][changed[3]];
        board[changed[2]][changed[3]] = temp;
    }
    return;
}

void EXTI0_IRQHandler(void){
    //update_board();
    //update_position();
    EXTI->PR |= EXTI_PR_PR0;
    return;
}

void Button_enable(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] &= ~0xf;
    EXTI->IMR |= 0x1;
    EXTI->RTSR |= 0x1;
    GPIOA->MODER &= ~0x3;
    NVIC->ISER[0] |= 1 << EXTI0_IRQn;
}

void ADC_enable(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= 0xc;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC1->SMPR2 |= 0x8;
    ADC1->SQR3 |= 0x1;
    //ADC1->CR1 |= ADC_CR1_EOCIE;
    ADC1->CR2 |= ADC_CR2_ADON;// | ADC_CR2_CONT ;// | ADC_CR2_SWSTART;
    //NVIC->ISER[0] |= 1<<(ADC_IRQn);
    //ADC1->CR2 |= ADC_CR2_SWSTART;

    return;
}

int ADC_val(void){
    int value;
    int team;
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while(!(ADC1->SR & ADC_SR_EOC));
    value = ADC1->DR;
    //return value;
    if(value > 2000){
        //GPIOD->ODR ^= 0x1000;
        team = 1;
    }
    else if(value < 200){
        //GPIOD->ODR ^= 0x2000;
        team = 2;
    }
    else{
        //GPIOD->ODR ^= 0x4000;
        team = 0;
    }
    return team;
}

int check_for_pickup(void){
    int x;
    int y;
    int pos_code = 0;
    int current_team;
    for(x = 0; x < MAX_X; x++){
        for(y = 0; y < MAX_Y; y++){
            current_team = ADC_val();
            if(current_team == 0 && teams[x][y] == self_team){
                GPIOB->ODR = 0;
                return board[x][y].type;
            }
            pos_code++;
            GPIOB->ODR = pos_code;
            //int tester = pos_code;
        }
        //pos_code++;
        //GPIOB->ODR = pos_code;
    }
    GPIOB->ODR = 0;
    return -1;
}

void update_board(void){
    int x;
    int y;
    int pos_code = 0;
    for(x = 0; x < MAX_X; x++){
        for(y = 0; y < MAX_Y; y++){
            teams[x][y] = ADC_val();
            pos_code++;
            GPIOB->ODR = pos_code;
            //int tester = pos_code;
        }
        //pos_code++;
        //GPIOB->ODR = pos_code;
    }
    GPIOB->ODR = 0;
    return;
}


//int main(void)
//{
//    Pos_select_enable();
//    ADC_enable();
//    Button_enable();
//    init_Board_Pawn_Test();
//    int test = 0;
//    int active_piece = -1;
//    for(;;){
//        active_piece = check_for_pickup();
//        if(active_piece != -1){
//            //Do Something related to the screen
//        }
//        //update_board();
//        //update_position();
//        test++;
//    }


//}

