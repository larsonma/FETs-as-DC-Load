/*
 * keypad.c
 *
 *  Created on: Jan. 6 2017
 *      Author: Mitchell Larson
 *
 * This file implements an API for using the keypad. Several functions are implemented
 * to make use easy.
 */

#include "keypad.h"

const char keys[] = "123A456B789C*0#D";
const int integers[] = {1,2,3,10,4,5,6,11,7,8,9,12,14,0,15,13};

static void setRows_clearCol();
static void setCol_clearRows();
static uint8_t getRow(uint8_t rows);
static uint8_t getCol(uint8_t cols);

/*
 * This function initializes the keyboard by enabling the clock the keyboard
 * resides on and setting all pins associated with the keyboard to pull up.
 * This will cause pins to default to logic high 1 when reading pins. The
 * pins are then set to 0 in case they were previously changed prior to use
 * by the keyboard
 * Inputs:
 * 		none
 * Outputs:
 * 		none
 */
void key_init(){
	//enable clock
	enable_clock('C');
	
	//set pins to pullup
	for(int i=0;i<=7;i++){
		set_pin_PUPDR('C',i,PULLUP);
	}
	
	//write 0's to key pins(0-7)
	*(GPIOC_ODR) &= ~(0xFF);
}

/*
 * This function will retrieve the key being pressed when the function is called.
 * This function will not block if no key is pressed, returning a 0 if no key is
 * pressed.
 * Inputs:
 * 		none
 * Outputs:
 * 		number 0-16 corresponding to key pressed. starts from top left and runs left to
 * 			right, top to bottom.
 */
uint8_t key_getkey_noblock(){
	setRows_clearCol();								//read coloumns, write rows
	uint8_t cols = (*(GPIOC_IDR) &= 0b1111);		//save the column encoding
	setCol_clearRows();								//switch to read rows, write columns
	uint8_t rows = ((*(GPIOC_IDR) &= 0xF0) >> 4);	//save the row encoding
	
	uint8_t row = getRow(rows);						//get the int representation of the row
	uint8_t col = getCol(cols);						//get the int representation of the col
	
	if(row*col == 0){
		return 0;									//If no key is pressed, return 0
	}else{
		return ((row-1)*4)+col;						//return the number associated with key(1-16)
	}
}

/*
 * This function returns a number corresponding to the key that was pressed on the
 * keypad. If no key is pressed, the function will block until a keypress is detected.
 * Then, the function will block until the key is released.
 * Inputs:
 * 		none
 * Outputs:
 * 		number 1-16 corresponding to key pressed. See previous funtion for description.
 */
uint8_t key_getkey(){
	setRows_clearCol();								//read coloumns, write rows
	while((*(GPIOC_IDR) &= 0b1111) == 0b1111){}		//wait until column detected
	uint8_t cols = (*(GPIOC_IDR) &= 0b1111);		//save the column encoding
	setCol_clearRows();								//switch to read rows, write columns
	uint8_t rows = ((*(GPIOC_IDR) &= 0xF0) >> 4);	//save the row encoding
	while(((*(GPIOC_IDR) &= 0xF0) >> 4) == rows){}	//wait until key is released.
	
	uint8_t row = getRow(rows);						//get the int representation of the row
	uint8_t col = getCol(cols);						//get the int representation of the col
	
	return ((row-1)*4)+col;							//return the number associated with key(1-16)	
}

/*
 * This function returns the ascii character corresponding to the key pressed. This function will
 * block until a key is pressed and released. The ascii character is determined by indexing the
 * character array, keyPressed.
 * Inputs:
 * 		none
 * Outputs:
 * 		ascii character corresponding to key pressed.
 */
char key_getchar(){
	uint8_t keyPressed = key_getkey();
	return keys[keyPressed-1];
}

/*
 * This function returns the ascii character corresponding to the key pressed. This function wont
 * block, with a 0 indicating no key was pressed. The ascii character is determined by indexing the
 * character array, keyPressed.
 * Inputs:
 * 		none
 * Outputs:
 * 		ascii character corresponding to key pressed.
 */
char key_getchar_noblock(){
	uint8_t keyPressed = key_getkey_noblock();
	if(keyPressed != 0){
			return keys[keyPressed-1];
	}
	return 0;
}

/*
 * This function returns the interger number associated with the key. This is different from
 * the regular getKey as pressing 9 will return the value 9 in this function. This function
 * will block until a key is pressed.
 * Inputs:
 * 		none
 * Outputs:
 * 		integer corresponding to the key pressed.
 */
uint8_t key_getint(){
	uint8_t keyPressed = key_getkey();
	return integers[keyPressed-1];
}


static void setRows_clearCol(){
	//set the rows to output mode
	for(int i=4;i<=7;i++){
		set_pin_mode('C',i,OUTPUT);
	}
	
	//set columns to input
	for(int i=0;i<=3;i++){
		set_pin_mode('C',i,INPUT);
	}
}

static void setCol_clearRows(){
	//set the rows to input mode
	for(int i=4;i<=7;i++){
		set_pin_mode('C',i,INPUT);
	}
	
	//set columns to output
	for(int i=0;i<=3;i++){
		set_pin_mode('C',i,OUTPUT);
	}
}

static uint8_t getRow(uint8_t rows){
	uint8_t row = -1;
	
	//return row number corresponding to the row encoding
	switch(rows){
		case 0b1111:
			row=0;
			break;
		case 0b1110:
			row=1;
			break;
		case 0b1101:
			row=2;
			break;
		case 0b1011:
			row=3;
			break;
		case 0b0111:
			row=4;
			break;
		default:
			row=0;
			break;			
	}
	
	return row;
}

static uint8_t getCol(uint8_t cols){
	int col = -1;
	
	//return column corresponding to column encoding
	switch(cols){
		case 0b1111:
			col=0;
			break;
		case 0b1110:
			col=1;
			break;
		case 0b1101:
			col=2;
			break;
		case 0b1011:
			col=3;
			break;
		case 0b0111:
			col=4;
			break;
		default:
			col=0;
			break;			
	}
	
	return col;
}
