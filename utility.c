#include "utility.h"

char * pawn_string = "Pawn";
char * bishop_string = "Bishop";
char * knight_string = "Knight";
char * rook_string = "Rook";
char * queen_string = "Queen";
char * king_string = "King";

char scoreString[] = "00-00";

// doesn't actually wait one ms
void delay(int ms) {
	int i;
	int j = 0;
	while (j < ms) {
		i = 10000;
		while (i > 0) i--;
		j++;
	}
}

int checkAvailable(int row, int col, int team) {
	if (row < 0 || row > 7 || col < 0 || col > 7) return 0;
	return 1;
}

// returns true if at least one element is truthy
int some(char list[], int length) {
	for (int i = 0; i < length; i++) {
		if (list[i]) return 1;
	}

	return 0;
}

// don't need to do fancy adding for anything other than pawns and knights, since they are all symmetrical; might consider changing to prioritize certain directions appearing first
void getCoords(int * newRow, int * newCol, int row, int col, int dist, int direction) {
	switch(direction) {
	case n: {
		*newRow = row + dist;
		*newCol = col;
		break;
	}
	case s: {
		*newRow = row - dist;
		*newCol = col;
		break;
	}
	case e: {
		*newRow = row;
		*newCol = col + dist;
		break;
	}
	case w: {
		*newRow = row;
		*newCol = col - dist;
		break;
	} case nw: {
		*newRow = row + dist;
		*newCol = col - dist;
		break;
	} case ne: {
		*newRow = row + dist;
		*newCol = col + dist;
		break;
	} case sw: {
		*newRow = row - dist;
		*newCol = col - dist;
		break;
	} case se: {
		*newRow = row - dist;
		*newCol = col + dist;
		break;
	}
	}
}

char * stringMap(int id) {
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

int teamY(int y, int team) {
	return (team == WHITE_TEAM) ? y : 7 - y;
}

void resetScoreString() {
	scoreString[0] = '0';
	scoreString[1] = '0';
	scoreString[3] = '0';
	scoreString[4] = '0';
}
