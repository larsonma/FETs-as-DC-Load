/*
 * gpio.h
 *
 *  Created on: Dec 16, 2016
 *      Author: Mitchell Larson
 */

#ifndef GPIO_H
#define GPIO_H
 
#include <inttypes.h>

//RCC constants 
#define RCC_AHB1ENR (volatile uint32_t*) 	0x40023830
#define GPIOA_RCCEN_F 0
#define GPIOB_RCCEN_F 1
#define GPIOC_RCCEN_F 2

//GPIO constants
#define GPIOA_MODER	(volatile uint32_t*) 	0x40020000
#define GPIOA_OTYPER (volatile uint32_t*) 	0x40020004
#define GPIOA_OSPEEDR (volatile uint32_t*) 	0x40020008
#define GPIOA_PUPDR	(volatile uint32_t*) 	0x4002000C
#define GPIOA_IDR (volatile uint32_t*) 		0x40020010
#define GPIOA_ODR (volatile uint32_t*) 		0x40020014
#define GPIOA_BSSR (volatile uint32_t*) 	0x40020018
#define GPIOA_LCKR (volatile uint32_t*) 	0x4002001C
#define GPIOA_AFRL (volatile uint32_t*) 	0x40020020
#define GPIOA_AFRH (volatile uint32_t*) 	0x40020024

#define GPIOB_MODER	(volatile uint32_t*) 	0x40020400
#define GPIOB_OTYPER (volatile uint32_t*) 	0x40020404
#define GPIOB_OSPEEDR (volatile uint32_t*) 	0x40020408
#define GPIOB_PUPDR	(volatile uint32_t*) 	0x4002040C
#define GPIOB_IDR (volatile uint32_t*) 		0x40020410
#define GPIOB_ODR (volatile uint32_t*) 		0x40020414
#define GPIOB_BSSR (volatile uint32_t*) 	0x40020418
#define GPIOB_LCKR (volatile uint32_t*) 	0x4002041C
#define GPIOB_AFRL (volatile uint32_t*) 	0x40020420
#define GPIOB_AFRH (volatile uint32_t*) 	0x40020424

#define GPIOC_MODER	(volatile uint32_t*) 	0x40020800
#define GPIOC_OTYPER (volatile uint32_t*) 	0x40020804
#define GPIOC_OSPEEDR (volatile uint32_t*) 	0x40020808
#define GPIOC_PUPDR	(volatile uint32_t*) 	0x4002080C
#define GPIOC_IDR (volatile uint32_t*) 		0x40020810
#define GPIOC_ODR (volatile uint32_t*) 		0x40020814
#define GPIOC_BSSR (volatile uint32_t*) 	0x40020818
#define GPIOC_LCKR (volatile uint32_t*) 	0x4002081C
#define GPIOC_AFRL (volatile uint32_t*) 	0x40020820
#define GPIOC_AFRH (volatile uint32_t*) 	0x40020824

//enumerated types
typedef enum {INPUT, OUTPUT, ALTFUNC, ANALOG} Mode;
typedef enum {PUSH_PULL, OPEN_DRAIN} OutputType;
typedef enum {LOW, MED, FAST, HIGH} Speed;
typedef enum {NONE, PULLUP, PULLDOWN} PullType;

//global functions
extern void enable_clock(char port);
extern void set_pin_mode(char port, uint8_t pin, Mode mode);
extern void set_pin_output_type(char port, uint8_t pin, OutputType outtype);
extern void set_output_speed(char port, uint8_t pin, Speed speed);
extern void set_pin_PUPDR(char port, uint8_t pin, PullType pulltype);
extern void set_alt_func(char port, uint8_t pin, uint8_t altFunc);

#endif /* GPIO_H */
