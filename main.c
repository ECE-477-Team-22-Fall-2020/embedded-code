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
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin = (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    usart_enable();
    timer_enable();

    // flash led
//    testLed();

    setupUI();

    // draw graphics on screen for viewing
//    testColorDraw();
    drawUsername("ggargle");
//    testDrawTeamPiece();

    display();

    init_Board_White_Bottom();
    timer_enable();
    while(1){
        ;
    }
}

