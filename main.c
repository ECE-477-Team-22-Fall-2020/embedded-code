/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "graphics.h"
#include "board.h"

int main(void)
{
    begin();
    //timer_enable();
    Pos_select_enable();
    ADC_enable();
    Button_enable();
    //init_Board_Pawn_Test();
    //init_Board_White_Bottom();
    update_board();
    setupUI();
    display();
    //timer_enable();
    drawPossibleMoves(KING_ID, 1, 1);
    //init_Board_Pawn_Test();
    init_Board_White_Bottom();
    timer_enable();
    while(1){
        ;
    }
}

