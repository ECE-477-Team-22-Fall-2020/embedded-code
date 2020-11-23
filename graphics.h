/*
 * graphics.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Ian Haggerty
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "oled_driver.h"

#include "board.h"
#include "gfxfont.h"
#include "glcdfont.c"
#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "utility.h"

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#define CHAR_WIDTH 6
#define LINE_HEIGHT 10

/**SCREEN REGION DEFINITIONS**/

#define PIECE_REGION_X 50
#define PIECE_REGION_Y 14
#define PIECE_REGION_W 27
#define PIECE_REGION_H 47

#define PLAYER_X 0
#define PLAYER_Y 0

#define USERNAME_X (PLAYER_X + (CHAR_WIDTH * 9)) // Playing: [username]
#define USERNAME_Y 0

#define PHONE_STATE_X (SSD1325_LCDWIDTH - (10 + (CHAR_WIDTH * 7))) // Phone: [check/x icon]
#define PHONE_STATE_Y 0

#define SCORE_HEADER_X (PIECE_REGION_X + PIECE_REGION_W + 3)
#define SCORE_HEADER_Y (PIECE_REGION_Y - 1)

#define SCORE_X SCORE_HEADER_X
#define SCORE_Y (SCORE_HEADER_Y + LINE_HEIGHT)

// may eventually want to put an elapsed time after this section
#define EXTRA_INFO_X 1
#define EXTRA_INFO_Y (PIECE_REGION_Y - 1)
#define EXTRA_INFO_W (PIECE_REGION_X - EXTRA_INFO_X - 4)
#define EXTRA_INFO_H (SSD1325_LCDHEIGHT - EXTRA_INFO_Y - 5)

#define NUM_SHOWN_SPACES 4

/*****************************/

const static uint8_t initial_ui_bits[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x04,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00,
	0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
	0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xc0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x03, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x60, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x60, 0xc0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
	0x00, 0x0c, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
	0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x0c, 0x18,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
	0x80, 0x01, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00,
	0x00, 0x0e, 0x00, 0x00, 0xc0, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x80, 0x01, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0xc0, 0x80, 0x01 };

#define piece_width 27
#define piece_height 47
const static unsigned char pawn_bitmap_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xf8, 0x00, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xfe, 0x03, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00, 0x80, 0xff, 0x0f, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
   0xe0, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f, 0x00,
   0xe0, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0xf8, 0xff, 0xff, 0x00, 0xf8, 0xff, 0xff, 0x00,
   0xf8, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 };


const static unsigned char bishop_bitmap_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
   0x00, 0xfc, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0xff, 0x03, 0x00,
   0x00, 0xfc, 0x07, 0x00, 0x80, 0xf9, 0x0f, 0x00, 0x80, 0xf3, 0x0f, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xf8, 0x00, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
   0xc0, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xe0, 0xff, 0x3f, 0x00,
   0xe0, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00 };

const static unsigned char knight_bitmap_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xf0, 0x07, 0x00,
   0x00, 0xf0, 0x1f, 0x00, 0x00, 0xff, 0x1f, 0x00, 0x00, 0xff, 0x7f, 0x00,
   0x80, 0xff, 0x7f, 0x00, 0xc0, 0xff, 0x7f, 0x00, 0xe0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0xff, 0x00, 0xf8, 0xff, 0xff, 0x00, 0xf8, 0xff, 0xff, 0x00,
   0xf8, 0xff, 0xff, 0x00, 0xf8, 0xff, 0xff, 0x00, 0xf8, 0xf8, 0xff, 0x00,
   0x00, 0xe0, 0xff, 0x00, 0x00, 0xe0, 0xff, 0x00, 0x00, 0xe0, 0xff, 0x00,
   0x00, 0xf0, 0x7f, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0xfc, 0x7f, 0x00,
   0x00, 0xfc, 0x3f, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0xfc, 0x3f, 0x00,
   0x00, 0xfc, 0x3f, 0x00, 0x00, 0xfe, 0x3f, 0x00, 0x00, 0xfe, 0x3f, 0x00,
   0x00, 0xff, 0x3f, 0x00, 0x00, 0xff, 0x3f, 0x00, 0x80, 0xff, 0x1f, 0x00,
   0x80, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
   0xe0, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f, 0x00,
   0xe0, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0xf8, 0xff, 0xff, 0x00, 0xf8, 0xff, 0xff, 0x00,
   0xf8, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 };

const static unsigned char rook_bitmap_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xfe, 0x73, 0x00,
   0x70, 0xfe, 0x73, 0x00, 0x70, 0xfe, 0x73, 0x00, 0x70, 0xfe, 0x73, 0x00,
   0x70, 0xfe, 0x73, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
   0xc0, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xe0, 0xff, 0x3f, 0x00,
   0xe0, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00 };

const static unsigned char queen_bitmap_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x20, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0xc0, 0xf8, 0x18, 0x00,
   0xc0, 0x73, 0x1e, 0x00, 0x80, 0x27, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xf8, 0x00, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
   0xc0, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xe0, 0xff, 0x3f, 0x00,
   0xe0, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00 };


const static unsigned char king_bitmap_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
   0x00, 0xf8, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xf8, 0x00, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xff, 0x07, 0x00, 0x00, 0xff, 0x07, 0x00,
   0x80, 0xff, 0x0f, 0x00, 0x80, 0xff, 0x0f, 0x00, 0xc0, 0xff, 0x1f, 0x00,
   0xc0, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0xe0, 0xff, 0x3f, 0x00,
   0xe0, 0xff, 0x3f, 0x00, 0xe0, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0x7f, 0x00,
   0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00 };

const static unsigned char blank_bitmap_bits[] = {
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x07 };


// check/x icons are 9x9
const static unsigned char x_bitmap_bits[] = {
   0x00, 0x00, 0x44, 0x00, 0x44, 0x00, 0x6c, 0x00, 0x38, 0x00, 0x38, 0x00,
   0x6c, 0x00, 0x44, 0x00, 0x44, 0x00 };

const static unsigned char check_bitmap_bits[] = {
   0x00, 0x00, 0x80, 0x00, 0xc0, 0x00, 0x40, 0x00, 0x60, 0x00, 0x26, 0x00,
   0x34, 0x00, 0x1c, 0x00, 0x08, 0x00 };

void setupUI(void);
void clearExtraInfo(void);
char addRow(int row, int b, int team);
void printPossibleMoves(struct Space * spaces, int num_spaces);
void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void clearArea(int x, int y, int width, int height);
int drawString(char * string, int x, int y);
void drawUsername(char * username);
void drawScore(void);
void drawPiece(int id);
void drawPawn(void);
void drawBishop(void);
void drawKnight(void);
void drawRook(void);
void drawQueen(void);
void drawKing(void);
void clearPiece(void);

void startWrite();
void endWrite();

void writePixel(int16_t x, int16_t y, uint16_t color);
void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);

#endif
