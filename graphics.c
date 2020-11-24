#include "graphics.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

char * versusString = "Vs:";
char * phoneString = "Phone:";

// sets up the basic background UI
void setupUI() {
    clearArea(0, 0, SSD1325_LCDWIDTH, SSD1325_LCDHEIGHT);

    // will want to eventually initialize this to "No active match"; will need width and height of the region to reset to that
    drawString(versusString, PLAYER_X, PLAYER_Y);

    drawString(phoneString, PHONE_STATE_X, PHONE_STATE_Y);
    drawDisconnected();

    drawString("Score:", SCORE_HEADER_X, SCORE_HEADER_Y);
    resetScoreString();
    drawScore();

    drawXBitmap(0, 0, initial_ui_bits, SSD1325_LCDWIDTH, SSD1325_LCDHEIGHT, WHITE);
}

void drawConnected(void) {
	clearArea(ICON_X, ICON_Y, CHAR_WIDTH, LINE_HEIGHT);
    drawXBitmap(ICON_X, ICON_Y, check_bitmap_bits, 9, 9, WHITE);
}

void drawDisconnected(void) {
	clearArea(ICON_X, ICON_Y, 9, 9);
    drawString("X", ICON_X, ICON_Y);
}

void ledOn(void) {
	GPIOA->BSRR |= 1 << led;
}

void ledOff(void) {
	GPIOA->BSRR |= 1 << (led + 16);
}

void testLed(void) {
	for (int i = 0; i < 10; i++) {
		ledOn();
		delay(100);
		ledOff();
		delay(100);
	}
}

void testConnectedGraphic(void) {
	for (int i = 0; i < 10; i++) {
		drawConnected();
		display();
		delay(100);
		drawDisconnected();
		display();
		delay(100);
	}
}

void clearExtraInfo(void) {
	clearArea(EXTRA_INFO_X, EXTRA_INFO_Y, EXTRA_INFO_W, EXTRA_INFO_H);
}

char addRow(int row, int b, int team) {
	if (team == WHITE_TEAM) return row + b;
	else return row - b;
}

void printPossibleMoves(struct Space * spaces, int numSpaces) {
	struct Space currentSpace;
    char displayString[] = "__,";
    int currentX = EXTRA_INFO_X;
    int currentY = EXTRA_INFO_Y + LINE_HEIGHT;
    int count = 0;

    if (numSpaces > NUM_SHOWN_SPACES) {
    	char dumb[] = "DUMB, AS EXPECTED";
    	numSpaces = NUM_SHOWN_SPACES;
    }

    for (int currentIndex = 0; currentIndex < numSpaces; currentIndex++) {
    	currentSpace = spaces[currentIndex];
        displayString[0] = colMap(currentSpace.y);
        displayString[1] = rowMap(currentSpace.x);
        currentX += CHAR_WIDTH * drawString(displayString, currentX, currentY);
        count++;

        // draw three possible moves per line
        if (count % 2 == 0) {
            currentX = EXTRA_INFO_X;
            currentY += LINE_HEIGHT;
        }
    }

    // remove final comma
    if (count == 0) return;
    else if (currentX != EXTRA_INFO_X) {
        clearArea(currentX - CHAR_WIDTH, currentY, CHAR_WIDTH, LINE_HEIGHT);
    } else {
        clearArea(EXTRA_INFO_X + (7 * CHAR_WIDTH), currentY - LINE_HEIGHT, CHAR_WIDTH, LINE_HEIGHT);
    }
}


//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {
  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (1 << (i % 8))) {
        drawPixel(x+i, y+j, color);
      }
    }
  }
}

void clearArea(int x, int y, int width, int height) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            drawPixel(i, j, BLACK);
        }
    }
}

void fillArea(int x, int y, int width, int height) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            drawPixel(i, j, WHITE);
        }
    }
}


int drawString(char * string, int x, int y) {
	if (string == NULL) return 0;
	char current_char;
	int i = 0;
	do {
		current_char = string[i];
		drawChar(x + (CHAR_WIDTH * i), y, current_char, WHITE, WHITE, 1, 1);
		i++;
	} while(current_char != 0);

	return i + 1;
}

void drawUsername(char * username) {
	drawString(username, USERNAME_X, USERNAME_Y);
}

void drawScore() {
	drawString(scoreString, SCORE_X, SCORE_Y);
}

void drawPiece(int id) {
	switch (id) {
	case PAWN_ID: return drawPawn();
	case BISHOP_ID: return drawBishop();
	case KNIGHT_ID: return drawKnight();
	case ROOK_ID: return drawRook();
	case QUEEN_ID: return drawQueen();
	case KING_ID: return drawKing();
	default: return clearPiece();
	}
}

void drawPawn() {
	drawXBitmap(PIECE_REGION_X, PIECE_REGION_Y, pawn_bitmap_bits, PIECE_REGION_W, PIECE_REGION_H, WHITE);
}

void drawBishop(void) {
	drawXBitmap(PIECE_REGION_X, PIECE_REGION_Y, bishop_bitmap_bits, PIECE_REGION_W, PIECE_REGION_H, WHITE);

}
void drawKnight(void) {
	drawXBitmap(PIECE_REGION_X, PIECE_REGION_Y, knight_bitmap_bits, PIECE_REGION_W, PIECE_REGION_H, WHITE);


}
void drawRook(void) {
	drawXBitmap(PIECE_REGION_X, PIECE_REGION_Y, rook_bitmap_bits, PIECE_REGION_W, PIECE_REGION_H, WHITE);
}

void drawQueen(void) {
	drawXBitmap(PIECE_REGION_X, PIECE_REGION_Y, queen_bitmap_bits, PIECE_REGION_W, PIECE_REGION_H, WHITE);


}
void drawKing(void) {
	drawXBitmap(PIECE_REGION_X, PIECE_REGION_Y, king_bitmap_bits, PIECE_REGION_W, PIECE_REGION_H, WHITE);
}

void clearPiece() {
	drawXBitmap(PIECE_REGION_X, PIECE_REGION_Y, blank_bitmap_bits, PIECE_REGION_W, PIECE_REGION_H, BLACK);
}

/**************************************************************************/
/*!
   @brief    Start a display-writing routine, overwrite in subclasses.
*/
/**************************************************************************/
void startWrite() {}

/**************************************************************************/
/*!
   @brief    End a display-writing routine, overwrite in subclasses if
   startWrite is defined!
*/
/**************************************************************************/
void endWrite() {}

/**************************************************************************/
/*!
   @brief    Write a pixel, overwrite in subclasses if startWrite is defined!
    @param   x   x coordinate
    @param   y   y coordinate
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void writePixel(int16_t x, int16_t y, uint16_t color) {
  drawPixel(x, y, color);
}

/**************************************************************************/
/*!
   @brief    Write a rectangle completely with one color, overwrite in
   subclasses if startWrite is defined!
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  // Overwrite in subclasses if desired!
  fillRect(x, y, w, h, color);
}

/**************************************************************************/
/*!
   @brief    Fill a rectangle completely with one color. Update in subclasses if
   desired!
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  startWrite();
  for (int16_t i = x; i < x + w; i++) {
    writeFastVLine(i, y, h, color);
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief    Write a perfectly vertical line, overwrite in subclasses if
   startWrite is defined!
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  // Overwrite in subclasses if startWrite is defined!
  // Can be just writeLine(x, y, x, y+h-1, color);
  // or writeFillRect(x, y, 1, h, color);
  drawFastVLine(x, y, h, color);
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly vertical line (this is often optimized in a
   subclass!)
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  startWrite();
  writeLine(x, y, x, y + h - 1, color);
  endWrite();
}

/**************************************************************************/
/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
#if defined(ESP8266)
  yield();
#endif
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      writePixel(y0, x0, color);
    } else {
      writePixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
   no background)
    @param    size_x  Font magnification level in X-axis, 1 is 'original' size
    @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
*/
/**************************************************************************/
void drawChar(int16_t x, int16_t y, unsigned char c,
                            uint16_t color, uint16_t bg, uint8_t size_x,
                            uint8_t size_y) {

  if ((x >= SSD1325_LCDWIDTH) ||    // Clip right
      (y >= SSD1325_LCDHEIGHT) ||   // Clip bottom
      ((x + 6 * size_x - 1) < 0) || // Clip left
      ((y + 8 * size_y - 1) < 0))   // Clip top
    return;

  if (c >= 176) c++; // Handle 'classic' charset behavior

  startWrite();
  for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
    uint8_t line = pgm_read_byte(&font[c * 5 + i]);
    for (int8_t j = 0; j < 8; j++, line >>= 1) {
      if (line & 1) {
        if (size_x == 1 && size_y == 1)
          writePixel(x + i, y + j, color);
        else
          writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y,
                        color);
      } else if (bg != color) {
        if (size_x == 1 && size_y == 1)
          writePixel(x + i, y + j, bg);
        else
          writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
      }
    }
  }
  if (bg != color) { // If opaque, draw vertical line for last column
    if (size_x == 1 && size_y == 1)
      writeFastVLine(x + 5, y, 8, bg);
    else
      writeFillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
  }
  endWrite();
}
