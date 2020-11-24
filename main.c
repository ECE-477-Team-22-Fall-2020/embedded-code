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
	// screen and led pin setup
    begin();

    // piece array reading setup
    Pos_select_enable();
    ADC_enable();
    Button_enable();
    update_board();

    // flash led
//    testLed();

    setupUI();

    // draw graphics on screen for viewing
//    testColorDraw();
    drawUsername("ggargle");
    testDrawTeamPiece();

    display();

    init_Board_White_Bottom();
    timer_enable();
    while(1){
        ;
    }
}

