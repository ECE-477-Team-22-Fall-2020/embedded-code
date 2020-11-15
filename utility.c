#include "utility.h"

// doesn't actually wait one ms lol
void delay(int ms) {
	int i;
	int j = 0;
	while (j < ms) {
		i = 10000;
		while (i > 0) i--;
		j++;
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
    case 0: return 'A';
    case 1: return 'B';
    case 2: return 'C';
    case 3: return 'D';
    case 4: return 'E';
    case 5: return 'F';
    case 6: return 'G';
    case 7: return 'H';
    }

    return 0x00;
}

char rowMap(int row) {
    switch(row) {
    case 0: return '1';
    case 1: return '2';
    case 2: return '3';
    case 3: return '4';
    case 4: return '5';
    case 5: return '6';
    case 6: return '7';
    case 7: return '8';
    }

    return 0x00;
}
