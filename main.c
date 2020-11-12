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

#define PAWN_ID 0
#define BISHOP_ID 1
#define KNIGHT_ID 2
#define ROOK_ID 3
#define QUEEN_ID 4
#define KING_ID 5

#define NUM_PIECES 32
#define LAST_PIECE -1
#define NULL_PIECE -2

char * pawn_string = "Pawn";
char * bishop_string = "Bishop";
char * knight_string = "Knight";
char * rook_string = "Rook";
char * queen_string = "Queen";
char * king_string = "King";



typedef struct PieceType {
    int row;
    int col;
    int id;
} Piece;

typedef struct PieceNodeType {
    Piece piece;
    int next; // index of next piece in array
} PieceNode;

PieceNode pieces[NUM_PIECES];
int firstIndex = -1;
int lastIndex = -1;

Piece getPiece(int row, int col, int id) {
    Piece ret;

    ret.row = row;
    ret.col = col;
    ret.id = id;

    return ret;
}

int findPiece(Piece piece) {
    if (firstIndex == -1) return -1;
    int currentIndex = firstIndex;
    PieceNode currentPiece;
    do {
        currentPiece = pieces[currentIndex];
        if (pieceCompare(piece, currentPiece.piece)) return currentIndex;
        currentIndex = currentPiece.next;

    } while (currentPiece.next != LAST_PIECE);

    return -1;
}

void addPiece(Piece piece) {
    if (findPiece(piece) != -1) return; // don't add a piece twice

    int newIndex = pieceAlloc();

    if (firstIndex != -1) {
        pieces[lastIndex].next = newIndex;
    } else {
        firstIndex = newIndex;
    }

    pieces[newIndex].next = LAST_PIECE;
    pieces[newIndex].piece = piece;
    lastIndex = newIndex;
}

void removePiece(Piece piece) {
    if (firstIndex == -1) return;

    PieceNode currentPiece;
    int prevIndex = -1; // not sure how this works
    int currentIndex = firstIndex;
    do {
        currentPiece = pieces[currentIndex];
        if (pieceCompare(piece, currentPiece.piece)) {
            if (currentIndex != firstIndex) {
                pieces[prevIndex].next = currentPiece.next;
                if (currentIndex == lastIndex) {
                    lastIndex = prevIndex;
                    pieces[lastIndex].next = LAST_PIECE; // probably not necessary
                }
            } else {
                firstIndex = currentPiece.next;
                if (currentIndex == lastIndex) lastIndex = prevIndex;
            }

            pieces[currentIndex].next = NULL_PIECE;
            return;
        }
        prevIndex = currentIndex;
        currentIndex = currentPiece.next;
    } while (currentIndex != LAST_PIECE);
}

int pieceCompare(Piece a, Piece b) {
    if (a.row == b.row && a.col == b.col && a.id == b.id) return 1;
    return 0;
}

void resetPieces() {
    for (int i = 0; i < NUM_PIECES; i++) {
        pieces[i].next = NULL_PIECE;
    }
}

int pieceAlloc() {
    for (int i = 0; i < NUM_PIECES; i++) {
        if (pieces[i].next == NULL_PIECE) return i;
    }

    return -1;
}

void clearArea(int x, int y, int width, int height) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            drawPixel(i, j, BLACK);
        }
    }
}

char * string_map(int id) {
    switch(id) {
    case PAWN_ID: return pawn_string;
    case BISHOP_ID: return bishop_string;
    case KNIGHT_ID: return knight_string;
    case ROOK_ID: return rook_string;
    case QUEEN_ID: return queen_string;
    case KING_ID: return king_string;
    }

    return NULL;
}

int scoreMap(int id) {
    switch(id) {
    case PAWN_ID: return 1;
    case BISHOP_ID: return 3;
    case KNIGHT_ID: return 3;
    case ROOK_ID: return 5;
    case QUEEN_ID: return 9;
    case KING_ID: return 0; // king is assigned no value, so don't change score if accidentally passed in
    }

    return 0;
}

char colMap(int row) {
    switch(row) {
    case 1: return 'A';
    case 2: return 'B';
    case 3: return 'C';
    case 4: return 'D';
    case 5: return 'E';
    case 6: return 'F';
    case 7: return 'G';
    case 8: return 'H';
    }

    return NULL;
}

char rowMap(int row) {
    switch(row) {
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    }

    return NULL;
}


void printPieces() {
    clearArea(0, 0, SSD1325_LCDWIDTH, SSD1325_LCDHEIGHT);

    int currentIndex = firstIndex;
    PieceNode currentPiece;
    char displayString[] = "__,"; // placeholder for each value

    int currentX = 1;
    int currentY = 0;
    int count = 0;
    while (currentIndex != LAST_PIECE) {
        currentPiece = pieces[currentIndex];
        displayString[0] = colMap(currentPiece.piece.col);
        displayString[1] = rowMap(currentPiece.piece.row);
        currentX += 6 * drawString(displayString, currentX, currentY);
        count++;
        if (count % 5 == 0) {
            currentX = 1;
            currentY += 10;
        }
        currentIndex = currentPiece.next;
    }

    // remove final comma lol
    if (currentX != 1) {
        clearArea(currentX - 6, currentY, 6, 8);
    } else {
        clearArea(SSD1325_LCDWIDTH - 13, currentY - 10, 12, 8);
    }
}

int main(void)
{
    begin();

    for (int i = 0; i < NUM_PIECES; i++) {
        pieces[i].next = NULL_PIECE;
    }

    // set screen all black as a background
    int color = BLACK;
    for (int x = 0; x < SSD1325_LCDWIDTH; x++) {
        for (int y = 0; y < SSD1325_LCDHEIGHT; y++) {
            drawPixel(x, y, color);
        }
    }

    display();

    addPiece(getPiece(2, 4, PAWN_ID));
    addPiece(getPiece(2, 2, PAWN_ID));
    addPiece(getPiece(1, 1, ROOK_ID));
    addPiece(getPiece(2, 2, PAWN_ID)); // will not get added
    addPiece(getPiece(6, 2, PAWN_ID));
    addPiece(getPiece(6, 4, PAWN_ID));
//  addPiece(getPiece(8, 1, ROOK_ID));

    printPieces();
    display();

    while (1) {
        removePiece(getPiece(6, 4, PAWN_ID));
        printPieces();
        display();
        addPiece(getPiece(6, 4, PAWN_ID));
        printPieces();
        display();
    }

//  int i = 0;
//  int width;
//  while (1) {
//      if (GPIOA->IDR & 0x1) {
//          clearPiece();
//          drawPiece(i);
//          width = drawString(string_map(i));
//          i = (i + 1) % 6;
//          display();
//          clearArea(0, 0, 6 * width, 10);
//      }
//  }
}

