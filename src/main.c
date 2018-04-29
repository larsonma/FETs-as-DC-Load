/*
 * main.c
 *
 *  Created on: Jan 11, 2017
 *      Author: Mitchell Larson
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ADC.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"
#include "gpio.h"

const char *help				= " D-hlp";
const char *current_temp_msg 	= "Temp: ";
const char *power_on_temp_msg 	= "On Temp: ";
const char *offset_up_msg 		= "Offset Up: A";
const char *offset_down_msg 	= "Offset Down: B";

typedef enum {INIT, READ, RETRIEVE, DISPLAY} State;
typedef enum {CURRENT, HELP} Mode1;

static void initalize();
static void read_input(Mode1 *mode, int *offset);
static void print_current_temp(float current_temp, float power_on_temp, int offset);
static void print_help();

/**
 * The main method of the file contains the control flow structure for a program
 * that monitors the current temperature of the external world. Based on 3
 * different state variables, which are manipulated by the user, the system's
 * lcd can display the current temperature, the temperature extremes, a
 * troubleshooting mode, as well as a help screen. Program runs until
 * power is no longer available.
 * Inputs:
 * 		none
 * Outputs:
 * 		none
 */
int main(void){
	//State variables
 	State state = INIT;
	Mode1 mode = CURRENT;

	//temperature data
	float current_temp;
	float power_on_temp;
	bool alarmActive = false;
	int offset = 0;

	while(1){
		//state machine
		switch(state){
			case INIT:
				initalize();
				power_on_temp = get_tempF();
				state = READ;
				break;
			case READ:
				read_input(&mode, &offset);	//pass pointers so copies
				state = RETRIEVE;				//are not passed
				break;
			case RETRIEVE:
				//retrieve temp and input voltage adjusting extremes if needed
				current_temp = get_tempF() + offset;
				if(current_temp >= power_on_temp +5 && alarmActive == false){
					//set MOSFET gate pins to 1
					*(GPIOA_ODR)  |= (0x380);
					alarmActive = true;
				}else if(alarmActive && current_temp <= power_on_temp){
					*(GPIOA_ODR)  &= ~(0x380);
					alarmActive = false;
				}
				//mV = get_mili_volts();
				state = DISPLAY;
				break;
			case DISPLAY:
				switch(mode){
					case CURRENT:
						print_current_temp(current_temp, power_on_temp, offset);
						break;
					case HELP:
						print_help();
						delay_ms(2000);
						mode = CURRENT;
				}

				state = READ;
				delay_ms(250);	//wait 250 seconds until the sequence repeats
				break;
		}
	}
	return 0;
}

/**
 * This function will initialize the Analog to digital converter, the keypad,
 * as well as the LCD by calling their respective initialization functions.
 * Inputs:
 * 		none
 * Outputs:
 * 		none
 */
static void initalize(){
	ADC_init();
	key_init();
	lcd_init(C_OFF);

	//set gpio pins for controlling MOSFETs to output mode
	for(int i = 7; i <= 9; i++){
		set_pin_mode('A',i,OUTPUT);
	}

	//initialize pins to 0
	*(GPIOA_ODR)  &= ~(0x380);
}

/**
 * This function will read any input from the key pad, if any, and change one of
 *  the state variables based on the user input.
 * Inputs:
 * 		*mode - pointer to the state variable mode
 * 		*tempMode - pointer to the state variable tempMode
 * Outputs:
 * 		none, but the state variables may change upon running this function
 */
static void read_input(Mode1 *mode, int *offset){
	switch(key_getchar_noblock()){
		case 'A':
			*offset = *offset+1;
			break;
		case 'B':
			*offset = *offset -1;
			break;
		case 'D':
			*mode = HELP;
			break;
		default:
			break;
	}
}

/**
 * This helper function prints the current temperature to the LCD. If the
 * program is in fahrenheit temperature mode, then the temperature will be
 * converted an then displayed.
 * Inputs:
 * 		current_temp - temperature to display
 * 		tempMode - the temperature mode the program is in
 */
static void print_current_temp(float current_temp, float power_on_temp, int offset){
	lcd_reset();
	char buffer1[5];
	sprintf(buffer1,"%4.1f",current_temp);
	
	char buffer2[5];
	sprintf(buffer2,"%4.1f", power_on_temp);

	char buffer3[3];
	itoa(offset, buffer3, 10);

	lcd_print_string(current_temp_msg);
	lcd_print_string(buffer1);
	lcd_print_string(help);
	lcd_row1();
	lcd_print_string(power_on_temp_msg);
	lcd_print_string(buffer2);
	lcd_print_string(" ");
	lcd_print_string(buffer3);
}

static void print_help(){
	lcd_reset();
	lcd_print_string(offset_up_msg);
	lcd_row1();
	lcd_print_string(offset_down_msg);

}
