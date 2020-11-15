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
#include "oled_driver.h"
#include "utility.h"
#include "graphics.h"
#include "board.h"

int main(void)
{
    begin();
    //timer_enable();
    Pos_select_enable();
    ADC_enable();
    Button_enable();
    init_Board_Pawn_Test();
    update_board();

    clearArea(0, 0, SSD1325_LCDWIDTH, SSD1325_LCDHEIGHT);

    timer_enable();
    drawXBitmap(0, 0, initial_ui_bits, SSD1325_LCDWIDTH, SSD1325_LCDHEIGHT, WHITE);
    while(1){
        ;
    }

    display();
}

