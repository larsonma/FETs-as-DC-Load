/*
 * ADC.c
 *
 *  Created on: Jan 11, 2017
 *      Author: Mitchell Larson
 */

#include "ADC.h"

/**
 * This function initializes the ADC by enable the clock for the ADC and
 * GPIO port A. Pin 6 for port A is then set to analog mode, the channel
 * for the converter is selected, and the converter is turned on.
 * The converter is set up to analyze the temperature sensor
 * Inputs:
 * 		none
 * Outputs:
 * 		none
 */
void ADC_init(){
	//enable clock for ADC1
	*(APB2ENR) |= 1<<8;
	
	//enable clock for GPIOA
	enable_clock('A');
	
	//enable analog mode for GPIOA, pin6
	set_pin_mode('A', 6, ANALOG);
	
	//SELECT CHANNEL
	*(ADC_SQR3) |= 6;
	
	//turn on ADC1
	*(ADC_CR2) |= 1;
}

/**
 * This function will start a conversion on the ADC when called and then
 * return the data in the data register. The upper half word is cleared
 * in the DR to ensure no garbage data is returned.
 * Inputs:
 * 		none
 * Outputs:
 * 		data in the DR register
 */
uint32_t take_sample(){
	//start conversion by setting SWSTART bit in ADC_CR2
	*(ADC_CR2) |= (1<<30);
	
	//wait for EOC bit to be set
	while((*(ADC_SR) & (1<<1))==0){}
	
	//return data in ADC_DR
	return (*(ADC_DR) & 0xFFFF);
}

/**
 * This function will return a temperature representation of the data in
 * the data register in celcius using previously defined functions
 * Inputs:
 * 		none
 * Outputs:
 * 		temperature in celcius
 */
float get_tempC(){
	return 25+((get_mili_volts()-750)/10);
}

/**
 * This function will return a temperature representation of the data in
 * the data register in ferenheit using previously defined functions
 * Inputs:
 * 		none
 * Outputs:
 * 		temperature in Ferenheit
 */
float get_tempF(){
	return (get_tempC()*1.8)+32;
}

/**
 * This function will return the milivolts of the incoming signal from
 * the temperature sensor.
 * Inputs:
 * 		none
 * Outputs:
 * 		milivolts
 */
float get_mili_volts(){
	return (((take_sample()*3.3)/4095)*1000);
}
