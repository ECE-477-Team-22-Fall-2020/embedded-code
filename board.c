/*
 * board.c
 *
 *  Created on: Nov 12, 2020
 *      Author: Matthew
 */

#include "board.h"

/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

int teams[MAX_X][MAX_Y];
struct Piece board[MAX_X][MAX_Y];
struct Piece blank_space = {.type = EMPTY, .team = 0};
int self_team;
int last_en_passant = -1;
int cached_active_piece = -1;
int cached_row = -1;
int cached_col = -1;

struct Space translate(struct Space current){
    current.x = 7 - current.x;
    current.y = 7 - current.y;
    return current;
}

void init_Board_White_Bottom(void){
    for( int x = 0; x < MAX_X; x++){
        for( int y = 0; y < MAX_Y; y++){
            board[x][y] = blank_space;
        }
    }
    self_team = 1;
    struct Piece newPawn;
    newPawn.type = PAWN;
    struct Piece newBishop;
    newBishop.type = BISHOP;
    struct Piece newKnight;
    newKnight.type = KNIGHT;
    struct Piece newRook;
    newRook.type = ROOK;
    struct Piece newQueen;
    newQueen.type = QUEEN;
    struct Piece newKing;
    newKing.type = KING;
    int x;
    for(x=0; x < MAX_X; x++){
        newPawn.team = 1;
        board[x][1] = newPawn;
        newPawn.team = 2;
        board[x][6] = newPawn;
    }
    //White Pieces
    newBishop.team = 1;
    board[2][0] = newBishop;
    board[5][0] = newBishop;
    newKnight.team = 1;
    board[1][0] = newKnight;
    board[6][0] = newKnight;
    newRook.team = 1;
    board[0][0] = newRook;
    board[7][0] = newRook;
    newQueen.team = 1;
    board[4][0] = newQueen;
    newKing.team = 1;
    board[3][0] = newKing;

    //Black Pieces
    newBishop.team = 2;
    board[2][7] = newBishop;
    board[5][7] = newBishop;
    newKnight.team = 2;
    board[1][7] = newKnight;
    board[6][7] = newKnight;
    newRook.team = 2;
    board[0][7] = newRook;
    board[7][7] = newRook;
    newQueen.team = 2;
    board[4][7] = newQueen;
    newKing.team = 2;
    board[3][7] = newKing;
    return;
}

void init_Board_Pawn_Test(void){
    for( int x = 0; x < MAX_X; x++){
        for( int y = 0; y < MAX_Y; y++){
            board[x][y] = blank_space;
        }
    }
    self_team = 1;
    struct Piece newPawn;
    newPawn.type = PAWN;
    newPawn.team = 1;
    board[0][1] = newPawn;
    board[5][1] = newPawn;
}

void init_Board_Black_Bottom(void){
    for( int x = 0; x < MAX_X; x++){
        for( int y = 0; y < MAX_Y; y++){
            board[x][y] = blank_space;
        }
    }
    self_team = 2;
    struct Piece newPawn;
    newPawn.type = PAWN;
    struct Piece newBishop;
    newBishop.type = BISHOP;
    struct Piece newKnight;
    newKnight.type = KNIGHT;
    struct Piece newRook;
    newRook.type = ROOK;
    struct Piece newQueen;
    newQueen.type = QUEEN;
    struct Piece newKing;
    newKing.type = KING;
    int x;
    for(x=0; x < MAX_X; x++){
        newPawn.team = 2;
        board[x][1] = newPawn;
        newPawn.team = 1;
        board[x][6] = newPawn;
    }
    //Black Pieces
    newBishop.team = 2;
    board[2][0] = newBishop;
    board[5][0] = newBishop;
    newKnight.team = 2;
    board[1][0] = newKnight;
    board[6][0] = newKnight;
    newRook.team = 2;
    board[0][0] = newRook;
    board[7][0] = newRook;
    newQueen.team = 2;
    board[3][0] = newQueen;
    newKing.team = 2;
    board[4][0] = newKing;

    //White Pieces
    newBishop.team = 1;
    board[2][7] = newBishop;
    board[5][7] = newBishop;
    newKnight.team = 1;
    board[1][7] = newKnight;
    board[6][7] = newKnight;
    newRook.team = 1;
    board[0][7] = newRook;
    board[7][7] = newRook;
    newQueen.team = 1;
    board[3][7] = newQueen;
    newKing.team = 1;
    board[4][7] = newKing;
    return;
}

void Pos_select_enable(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER &= ~0xffff;
    GPIOB->MODER |= 0x5555;
    GPIOB->ODR |= 1 << 6;

    return;
}

void castling(int changed[]){
    //Consider that a king can castle both left and right
    board[changed[2]][changed[3]] = board[changed[6]][changed[7]];
    board[changed[4]][changed[5]] = board[changed[0]][changed[1]];
    board[changed[0]][changed[1]] = blank_space;
    board[changed[6]][changed[7]] = blank_space;
    return;
}

void en_passant(int changed[]){
    //Will probably pass in an array of changed spaces
    if(changed[1] == changed[3]){
        board[changed[4]][changed[5]] = board[changed[0]][changed[1]];
        board[changed[0]][changed[1]] = blank_space;
        board[changed[2]][changed[3]] = blank_space;
        sendMove(changed[0], changed[1], changed[4], changed[5]);
    }else if(changed[1] == changed[5]){
        if(changed[0] == changed[2]){
            board[changed[2]][changed[3]] = board[changed[4]][changed[5]];
            board[changed[0]][changed[1]] = blank_space;
            board[changed[4]][changed[5]] = blank_space;
            sendMove(changed[4], changed[5], changed[2], changed[3]);
        }else{
            board[changed[2]][changed[3]] = board[changed[0]][changed[1]];
            board[changed[0]][changed[1]] = blank_space;
            board[changed[4]][changed[5]] = blank_space;
            sendMove(changed[0], changed[1], changed[2], changed[3]);
        }
    }else{
        board[changed[0]][changed[1]] = board[changed[4]][changed[5]];
        board[changed[4]][changed[5]] = blank_space;
        board[changed[2]][changed[3]] = blank_space;
        sendMove(changed[4], changed[5], changed[0], changed[1]);
    }
//    board[changed[2]][changed[3]] = board[changed[4]][changed[5]];
//    board[changed[0]][changed[1]] = blank_space;
//    board[changed[4]][changed[5]] = blank_space;
    return;
}

void sendMove(int init_x, int init_y, int end_x, int end_y){
    bluetooth_buffer[0] = colMap(init_x) + 32;
    bluetooth_buffer[1] = rowMap(init_y);
    bluetooth_buffer[2] = colMap(end_x) + 32;
    bluetooth_buffer[3] = rowMap(end_y);

    HAL_UART_Transmit(&huart2, (uint8_t *)bluetooth_buffer, strLen(bluetooth_buffer), 500);
    for (int i = 0; i < BLUETOOTH_BUFFER_SIZE; i++) {
        bluetooth_buffer[i] = 0;
    }
}

void update_position(void){

    int changed[] = {-1, -1, -1, -1, -1, -1, -1, -1};
    struct Piece temp;
    int num_change = 0;
    //Loop through the board and the team map, determine where the changes occur
    for(int x = 0; x < MAX_X; x++){
        for(int y = 0; y < MAX_Y; y++){
            if(teams[x][y] != board[x][y].team){
                changed[2 * num_change] = x;
                changed[(2 * num_change) + 1] = y;
                num_change++;
                //4 changes castling
                //3 changes en passant
            }}}
    if(num_change == 4){
        if(board[changed[0]][changed[1]].type == KING){
            sendMove(changed[0], changed[1], changed[4], changed[5]);
        }else{
            sendMove(changed[6], changed[7], changed[2], changed[3]);
        }
        castling(changed);
        return;
    }
    if(num_change == 3){
        en_passant(changed);
        return;
    }
    if(board[changed[0]][changed[1]].team != 0 && board[changed[2]][changed[3]].team != 0){
        //THIS IS WHERE CAPTURE HAPPENS
        if(teams[changed[0]][changed[1]] == 0){
            board[changed[2]][changed[3]] = board[changed[0]][changed[1]];
            board[changed[0]][changed[1]] = blank_space;
            sendMove(changed[0], changed[1], changed[2], changed[3]);
        }else{
            board[changed[0]][changed[1]] = board[changed[2]][changed[3]];
            board[changed[2]][changed[3]] = blank_space;
            sendMove(changed[2], changed[3], changed[0], changed[1]);
        }
    }else{
        temp = board[changed[0]][changed[1]];
        board[changed[0]][changed[1]] = board[changed[2]][changed[3]];
        board[changed[2]][changed[3]] = temp;
        if(board[changed[0]][changed[1]].type == 0){
            sendMove(changed[0], changed[1], changed[2], changed[3]);
        }else{
            sendMove(changed[2], changed[3], changed[0], changed[1]);
        }
    }
    return;
}

int checkEnemy(int row, int col) {
	return (board[col][row].team == getEnemy(self_team)) ? 1 : 0;
}

int checkEmpty(int row, int col) {
	return (board[col][row].team == 1 || board[col][row].team == 2) ? 0 : 1;
}

void drawPossibleMoves(int piece, int row, int col, int currentTeam) {
    clearExtraInfo();

    drawString("Choices:", EXTRA_INFO_X, EXTRA_INFO_Y);

    struct Space spaces[NUM_SHOWN_SPACES + 1] = {0};
    int numSpaces = 0;

    switch(piece) {
    case PAWN_ID: {
        int newRow;

        // initially, you can move forward two
        if (teamY(row, currentTeam) == 1) {
        	newRow = addRow(row, 2, currentTeam);
            if (checkInBounds(newRow, col) && checkEmpty(newRow, col)) {
                spaces[numSpaces].x = col;
                spaces[numSpaces].y = newRow;
                numSpaces++;
            }
        }

        // en passante
        int diff = row - last_en_passant;
        diff = diff < 0 ? -1 * diff : diff;
        if (teamY(row, currentTeam) == 5 && diff == 1 && last_en_passant != -1) {
        	spaces[numSpaces].x = last_en_passant;
        	spaces[numSpaces].y = addRow(row, 1, currentTeam);
        	numSpaces++;
        }

        // attack move 1
        newRow = addRow(row, 1, currentTeam);
        if (checkInBounds(newRow, col + 1) && checkEnemy(newRow, col + 1)) {
        	spaces[numSpaces].x = col + 1;
        	spaces[numSpaces].y = newRow;
        	numSpaces++;
        }

        // attack move 2
		if (checkInBounds(newRow, col - 1) && checkEnemy(newRow, col - 1)) {
			spaces[numSpaces].x = col - 1;
			spaces[numSpaces].y = newRow;
			numSpaces++;
		}

		// normal forward move
		newRow = addRow(row, 1, currentTeam);
        if (checkInBounds(newRow, col) && checkEmpty(newRow, col)) {
            spaces[numSpaces].x = col;
            spaces[numSpaces].y = newRow;
            numSpaces++;
        }
        break;
    }
    case BISHOP_ID: {
        numSpaces = getSpaces(spaces, row, col, 7, (char[]) {nw, ne, sw, se}, 4);
        break;
    }
    case KNIGHT_ID: {
        int twos[] = {-2, 2};
        int ones[] = {-1, 1};
        int newRow;
        int newCol;
        for(int x = 0; x < 2; x++) {
            for(int y = 0; y < 2; y++) {
            	newCol = col + twos[x];
            	newRow = row + ones[y];
            	if (checkInBounds(newRow, newCol) && (checkEmpty(newRow, newCol) || checkEnemy(newRow, newCol))) {
            		spaces[numSpaces].x = newCol;
					spaces[numSpaces].y = newRow;
					numSpaces++;
            	}

                if (numSpaces >= NUM_SHOWN_SPACES) {
                    x = 3;
                    break;
                }
            }
        }

        for(int x = 0; x < 2; x++){
            for(int y = 0; y < 2; y++){
            	newCol = col + ones[x];
				newRow = row + twos[y];
				if (checkInBounds(newRow, newCol) && (checkEmpty(newRow, newCol) || checkEnemy(newRow, newCol))) {
					spaces[numSpaces].x = newCol;
					spaces[numSpaces].y = newRow;
					numSpaces++;
				}

                if (numSpaces >= NUM_SHOWN_SPACES) {
                    x = 3;
                    break;
                }
            }
        }
        break;
    }
    case ROOK_ID: {
        numSpaces = getSpaces(spaces, row, col, 7, (char[]) {n, s, e, w}, 4);

        break;
    }
    case QUEEN_ID: {
        numSpaces = getSpaces(spaces, row, col, 7, (char[]) {n, s, e, w, nw, ne, sw, se}, 8);
        break;
    }
    case KING_ID: {
        numSpaces = getSpaces(spaces, row, col, 1, (char[]) {n, s, e, w, nw, ne, sw, se}, 8);
        break;
    }
    }

    printPossibleMoves(spaces, numSpaces);
    numSpaces++;
}

int getSpaces(struct Space * spaces, int row, int col, int maxDist, char directions[], int numDirections) {
    int numSpaces = 0;
    int currentDist = 1;
    int currentTeam = WHITE_TEAM; // TODO: Make a global that keeps track of this
    int currentDirection;


    while (currentDist <= maxDist && some(directions, numDirections)) {
        int newRow;
        int newCol;

        for (int i = 0; i < numDirections; i++) {
            currentDirection = directions[i];
            if (currentDirection == nullDirection){ continue;}
            getCoords(&newRow, &newCol, row, col, currentDist, currentDirection);
            if (checkInBounds(newRow, newCol)) {
            	// enemy piece blocking
            	if (checkEnemy(newRow, newCol)) {
            		spaces[numSpaces].x = newCol;
					spaces[numSpaces].y = newRow;
					numSpaces++;
					directions[i] = nullDirection;
				// empty space
            	} else if (checkEmpty(newRow, newCol)) {
            		spaces[numSpaces].x = newCol;
					spaces[numSpaces].y = newRow;
					numSpaces++;
				// own piece blocking
            	} else {
            		directions[i] = nullDirection;
            	}
                if (numSpaces == NUM_SHOWN_SPACES) return numSpaces;
            // outside of board range
            } else {
                directions[i] = nullDirection;
            }
        }
        currentDist++;
    }

    return numSpaces;
}

void EXTI0_IRQHandler(void){
    update_board();
    update_position();
    ledOff();
    EXTI->PR |= EXTI_PR_PR0;
    return;
}

void Button_enable(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] &= ~0xf;
    EXTI->IMR |= 0x1;
    EXTI->RTSR |= 0x1;
    GPIOA->MODER &= ~0x3;
    NVIC->ISER[0] |= 1 << EXTI0_IRQn;
}

void ADC_enable(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= 0xc;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC1->SMPR2 |= 0x8;
    ADC1->SQR3 |= 0x1;
    //ADC1->CR1 |= ADC_CR1_EOCIE;
    ADC1->CR2 |= ADC_CR2_ADON;// | ADC_CR2_CONT ;// | ADC_CR2_SWSTART;
    //NVIC->ISER[0] |= 1<<(ADC_IRQn);
    //ADC1->CR2 |= ADC_CR2_SWSTART;

    return;
}

int ADC_val(void){
    int value;
    int team;
    //do{
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while(!(ADC1->SR & ADC_SR_EOC));
	value = ADC1->DR;
	if(value > 4090){
		value++;
//		ledOn();
	}
    //}while(value >= 4094 || value < 2);
    //return value;
    if(value > 2000){
        //GPIOD->ODR ^= 0x1000;
        team = 1;
    }
    else if(value < 300){
        //GPIOD->ODR ^= 0x2000;
        team = 2;
    }
    else{
        //GPIOD->ODR ^= 0x4000;
        team = 0;
    }
    return team;
}

int check_for_pickup(int * row, int * col){
    int x;
    int y;
    int pos_code = 0;
    int current_team;
    int return_team = -1;
    int placed = 0;
    for(x = 0; x < MAX_X; x++){
        for(y = 0; y < MAX_Y; y++){
            current_team = ADC_val();
            if(current_team == 0 && board[x][y].team != 0){
                //GPIOB->ODR = 0;
                ledOn();
//                drawPossibleMoves(board[x][y].type - 1, y, x, self_team);
                *row = y;
                *col = x;
                //return board[x][y].type;
                return_team = board[x][y].type;
            }
            if(current_team == self_team && board[x][y].team != self_team){
                placed = 1;
            }
            pos_code++;
            //GPIOB->ODR &= ~0x3f;
            GPIOB->ODR = pos_code;

            //int tester = pos_code;
        }
        //pos_code++;
        //GPIOB->ODR = pos_code;
    }
    GPIOB->ODR = 0;
    GPIOB->ODR |= (placed << 6);
    return return_team;
}

void timer_enable(void){
    RCC->APB1ENR |= 0x8;
    TIM5->CR1 &= ~0x10;
    TIM5->ARR = 4999;
    TIM5->PSC = 1599;
    TIM5->DIER |= 0x1;

    TIM5->CR1 |= 0x1;
    NVIC->ISER[1] = 1<<(TIM5_IRQn - 32);

    RCC->APB1ENR |= 0x1;
    TIM2->CR1 &= ~0x10;
    TIM2->ARR = 1999;
    TIM2->PSC = 799;
    TIM2->DIER |= 0x1;

    TIM2->CR1 |= 0x1;
    NVIC->ISER[0] = 1<<(TIM2_IRQn);
}

void usart_enable(void){
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    //USART2->BRR = 9600 << 4;
    //USART2->CR1 |= USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_RE;

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        //Error_Handler();
    }
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    //NVIC->ISER[1] |= 1<<(USART2_IRQn - 32);
}

void TIM2_IRQHandler(void){
    if (strLen(bluetooth_buffer) > 0)
        timer_count++;
    if (timer_count > 5)
        MessageHandler();
    /* USER CODE END TIM2_IRQn 0 */
    //HAL_TIM_IRQHandler(&htim2);
    TIM2->SR &= ~0x1;
}

void USART2_IRQHandler(void){
    HAL_UART_Receive(&huart2, (uint8_t *)&bluetooth_buffer[buffer_index++], 1, 10);
    if(bluetooth_buffer[buffer_index - 1] == '\n')
        MessageHandler();
    HAL_UART_IRQHandler(&huart2);
    //int gamerbois = USART2->DR;
}

void TIM5_IRQHandler(void) {
    int row;
    int col;
    int new_active_piece = check_for_pickup(&row, &col);
    //    clearPiece();
    //    drawPiece(active_piece - 1);
    //    drawSelfPiece()
    if (new_active_piece != cached_active_piece || row != cached_row || col != cached_col) {
        if (new_active_piece == -1) {
            ledOff();
            clearPiece(self_team == 1 ? BLACK : WHITE);
        } else {
            drawSelfPiece(new_active_piece - 1, row, col);
        }
        cached_active_piece = new_active_piece;
        cached_row = row;
        cached_col = col;
        display();
    }
    TIM5->SR &= ~0x1;
}

void update_board(void){
    int x;
    int y;
    int pos_code = 0;
    for(x = 0; x < MAX_X; x++){
        for(y = 0; y < MAX_Y; y++){
            teams[x][y] = ADC_val();
            pos_code++;
            GPIOB->ODR = pos_code;
            //int tester = pos_code;
        }
        //pos_code++;
        //GPIOB->ODR = pos_code;
    }
    GPIOB->ODR = 0;
    return;
}

void exec_external_move(int init_x, int init_y, int end_x, int end_y){
    //struct Piece temp;
    if(board[init_x][init_y].type == KING && (init_x - end_x > 1 || init_x - end_x < -1)){
        //Castling
        int changed[] = {-1, -1, -1, -1, -1, -1, -1, -1};
        if(end_x == 1){
            int changed[] = {0, init_y, 1, init_y, 2, init_y, 4, init_y};
        }else{
            int changed[] = {4, init_y, 5, init_y, 6, init_y, 7, init_y};
        }
        castling(changed);
    }
    else if(board[init_x][init_y].type == PAWN && (init_x - end_x == 1 || init_x - end_x == -1)){
        // En passant
        int changed[] = {-1, -1, -1, -1, -1, -1, -1, -1};
        if(init_x < end_x){
           int changed[] = {init_x, init_y, end_x, end_y + 1, end_x, end_y, -1, -1};
        }else{
           int changed[] = {end_x, end_y + 1, end_x, end_y, init_x, init_y, -1, -1};
        }
        en_passant(changed);
    }
    else{
        board[end_x][end_y] = board[init_x][init_y];
        board[init_x][init_y] = blank_space;
    }
}

int colSwap(char curr_col){
    int init_x;
    switch(curr_col){
        case 'a':
            init_x = 7;
            break;
        case 'b':
            init_x = 6;
            break;
        case 'c':
            init_x = 5;
            break;
        case 'd':
            init_x = 4;
            break;
        case 'e':
            init_x = 3;
            break;
        case 'f':
            init_x = 2;
            break;
        case 'g':
            init_x = 1;
            break;
        case 'h':
            init_x = 0;
            break;
    }
    return init_x;
}

void EnemyMoveSwap(char * move){
    int init_x;
    int init_y;
    int end_x;
    int end_y;

    init_x = colSwap(move[0]);
    init_y = 7 - (move[1] - 49);
    end_x = colSwap(move[2]);
    end_y = 7 - (move[3] - 49);

    int piece_id = board[init_x][init_y].type;
    exec_external_move(init_x, init_y, end_x, end_y);

    drawEnemyPiece(piece_id - 1, init_y, init_x, end_y, end_x);
    display();
}

void MessageHandler(void)
{
    GPIO_TypeDef *port;
    uint16_t pin;

    //int test = strLen(bluetooth_buffer);

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
        //HAL_UART_Transmit(&huart2, (uint8_t *)bluetooth_buffer, strLen(bluetooth_buffer), 500);
    }

    if(strLen(bluetooth_buffer) == 5){
        EnemyMoveSwap(bluetooth_buffer);
    }

    for (int i = 0; i < BLUETOOTH_BUFFER_SIZE; i++) {
        bluetooth_buffer[i] = 0;
    }

    buffer_index = 0;
    timer_count = 0;
}
