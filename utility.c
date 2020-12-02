#include "utility.h"

char * pawn_string = "Pawn";
char * bishop_string = "Bishop";
char * knight_string = "Knight";
char * rook_string = "Rook";
char * queen_string = "Queen";
char * king_string = "King";

char scoreString[] = "00-00";

char bluetooth_buffer[BLUETOOTH_BUFFER_SIZE];
uint8_t timer_count = 0;
uint8_t buffer_index = 0;

// Only works for the bluetooth buffer
int strLen(char buf[]) {
	char current_char;
	int i = 0;
	do {
		current_char = buf[i++];
	} while (current_char != 0 && i < (BLUETOOTH_BUFFER_SIZE - 1));

	return i - 1;
}

uint8_t stringCompare(char array1[], char array2[], uint16_t length)
{
    uint8_t comVAR = 0, i;
    for (i = 0; i < length; i++)
    {
        if (array1[i] == array2[i])
            comVAR++;
        else
            comVAR = 0;
    }
    if (comVAR == length)
        return 1;
    else
        return 0;
}

void MessageHandler(void)
{
    GPIO_TypeDef *port;
    uint16_t pin;

    //int test = strLen("Matt");

    switch (bluetooth_buffer[0])
    {
        case 'B':
            port = GPIOD;
            pin = (1 << 15);
            break;
        case 'R':
            port = GPIOD;
            pin = (1 << 14);
            break;
        case 'O':
            port = GPIOD;
            pin = (1 << 13);
            break;
        case 'G':
            port = GPIOD;
            pin = (1 << 12);
            break;
    }
    if (stringCompare(bluetooth_buffer + 2, "LED ON", strLen("LED ON")))
    {
        HAL_GPIO_WritePin(port, pin, SET);
        HAL_UART_Transmit(&huart2, (uint8_t *)"LED is ON.\n",
                          strLen("LED is ON.\n"), 500);
    }
    else if (stringCompare(bluetooth_buffer + 2, "LED OFF", strLen("LED OFF")))
    {
        HAL_GPIO_WritePin(port, pin, RESET);
        HAL_UART_Transmit(&huart2, (uint8_t *)"LED is OFF.\n",
                          strLen("LED is OFF.\n"), 500);
    }
    else
    {
        bluetooth_buffer[strLen(bluetooth_buffer)] = '\n';
		HAL_UART_Transmit(&huart2, (uint8_t *)bluetooth_buffer, strLen(bluetooth_buffer), 500);
    }

    for (int i = 0; i < BLUETOOTH_BUFFER_SIZE; i++) {
    	bluetooth_buffer[i] = 0;
    }

    buffer_index = 0;
    timer_count = 0;
}

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

int getEnemy(int team) {
	if (team == 0) return 0;
	return team == 1 ? 2 : 1;
}

int checkInBounds(int row, int col) {
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
