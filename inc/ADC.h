/*
 * ADC.h
 *
 *  Created on: Jan 11, 2017
 *      Author: Mitchell Larson
 */

#ifndef ADC_H
#define ADC_H

//ADC constants
#define ADC_BASE 	(volatile uint32_t*)	0x40012000
#define ADC_SR 	 	(volatile uint32_t*)	0x40012000
#define ADC_CR1	 	(volatile uint32_t*)	0x40012004
#define ADC_CR2  	(volatile uint32_t*)	0x40012008
#define ADC_SMPR1	(volatile uint32_t*)	0x4001200C
#define ADC_SMPR2	(volatile uint32_t*)	0x40012010
#define ADC_HTR		(volatile uint32_t*)	0x40012024
#define ADC_LTR		(volatile uint32_t*)	0x40012028
#define ADC_SQR1	(volatile uint32_t*)	0x4001202C
#define ADC_SQR2	(volatile uint32_t*)	0x40012030
#define ADC_SQR3	(volatile uint32_t*)	0x40012034
#define ADC_DR		(volatile uint32_t*)	0x4001204C

//RCC constants
#define RCC_BASE	(volatile uint32_t*)	0x40023800
#define APB2ENR		(volatile uint32_t*)	0x40023844

#include <inttypes.h>
#include "gpio.h"

extern void ADC_init();
extern uint32_t take_sample();
extern float get_tempC();
extern float get_tempF();
extern float get_mili_volts();

#endif /* ADC_H */
