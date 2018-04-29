/*
 * gpio.c
 *
 *  Created on: Dec 16, 2016
 *      Author: larsonma
 *  This is an API designed to make GPIO initialization and modification easy to
 *  use, while minimizing risk of mistakes. Currently works for ports A,B, and C
 */
 
#include "gpio.h"

static void set_low_reg_alt_func(char port, uint8_t pin, uint8_t altFunc);
static void set_high_reg_alt_func(char port, uint8_t pin,uint8_t altFunc);

/*
 * The function allows the clock for a specific GPIO port to be initialized. The port to
 * be specified is not case sensitive.
 * Inputs:
 * 		char port - port to enable clock for
 * Outputs:
 * 		none
 */
void enable_clock(char port){
	switch(port){
		case 'A':
		case 'a':
			*(RCC_AHB1ENR) |= (1<<GPIOA_RCCEN_F);
			break;
		case 'B':
		case 'b':
			*(RCC_AHB1ENR) |= (1<<GPIOB_RCCEN_F);
			break;
		case 'C':
		case 'c':
			*(RCC_AHB1ENR) |= (1<<GPIOC_RCCEN_F);
			break;			
	}
}

/*
 * This function allows a user to set any pin on a port to one of the 4 modes. The modes
 * are specified by enumerated types INPUT, OUTPUT, ALTFUNC, and ANALOG. Protection against
 * setting invalid pins is implemented.
 * Inputs:
 * 		char port - port to modify pins on
 * 		uint8_t pin - pin to be modified
 * 		Mode mode - mode to set pin to.
 * Outputs:
 * 		none
 */
void set_pin_mode(char port, uint8_t pin, Mode mode){
	if(pin >=0 && pin <= 15){
		
		volatile uint32_t* MODER;
		
		//assign the MODER according to the proper port selected
		switch(port){
			case 'A':
			case 'a':
				MODER = GPIOA_MODER;
				break;
			case 'B':
			case 'b':
				MODER = GPIOB_MODER;
				break;
			case 'C':
			case 'c':
				MODER = GPIOC_MODER;
				break;
			default:
				return;		//error
		}
		
		//clear pins associated with pin mode
		*(MODER) &= ~(0b11<<(pin*2));
		
		//set pin according to MODE specified
		switch(mode){
			case INPUT:
				*(MODER) |= (0b00<<(pin*2));
				break;
			case OUTPUT:
				*(MODER) |= (0b01<<(pin*2));
				break;
			case ALTFUNC:
				*(MODER) |= (0b10<<(pin*2));
				break;
			case ANALOG:
				*(MODER) |= (0b11<<(pin*2));
				break;
			default:
				return;
		}
	}
}

/*
 * This function allows a user to set any pin on a port to one of the 2 output types. The types
 * are specified by enumerated types PULLUP and OPEN_DRAIN. Protection against
 * setting invalid pins is implemented.
 * Inputs:
 * 		char port - port to modify pins on
 * 		uint8_t pin - pin to be modified
 * 		OutputType outtype - type to set pin to.
 * Outputs:
 * 		none
 */
void set_pin_output_type(char port, uint8_t pin, OutputType outtype){
	if(pin >= 0 && pin <= 15){
		volatile uint32_t* OTYPER;
	
		//assign the OSPEEDR according to the proper port selected
		switch(port){
			case 'A':
			case 'a':
				OTYPER = GPIOA_OTYPER;
				break;
			case 'B':
			case 'b':
				OTYPER = GPIOB_OTYPER;
				break;
			case 'C':
			case 'c':
				OTYPER = GPIOC_OTYPER;
				break;
			default:
				return;		//error
		}

		//clear pin specified
		*(OTYPER) &= ~(1<<pin);

		//switch output type of pin to type specified.
		switch(outtype){
			case PUSH_PULL:
				*(OTYPER) |= (0<<pin);
				break;
			case OPEN_DRAIN:
				*(OTYPER) |= (1<<pin);
				break;
			default:
				return;
		}
	}		
}

/*
 * This function allows a user to set any pin on a port to one of the 4 speeds. The speeds
 * are specified by enumerated types LOW, MED, FAST, and HIGH. Protection against
 * setting invalid pins is implemented.
 * Inputs:
 * 		char port - port to modify pins on
 * 		uint8_t pin - pin to be modified
 * 		Speed speed - speed to set pin to.
 * Outputs:
 * 		none
 */
void set_output_speed(char port, uint8_t pin, Speed speed){
	if(pin >= 0 && pin <= 15){
		volatile uint32_t* OSPEEDR;
	
		//assign the OSPEEDR according to the proper port selected
		switch(port){
			case 'A':
			case 'a':
				OSPEEDR = GPIOA_OSPEEDR;
				break;
			case 'B':
			case 'b':
				OSPEEDR = GPIOB_OSPEEDR;
				break;
			case 'C':
			case 'c':
				OSPEEDR = GPIOC_OSPEEDR;
				break;
			default:
				return;		//error
		}

		//clear speed set at specified pin
		*(OSPEEDR) &= ~(0b11<<(pin*2));

		//set pin to specified speed
		switch(speed){
			case LOW:
				*(OSPEEDR) |= (0b00<<(pin*2));
				break;
			case MED:
				*(OSPEEDR) |= (0b01<<(pin*2));
				break;
			case FAST:
				*(OSPEEDR) |= (0b10<<(pin*2));
				break;
			case HIGH:
				*(OSPEEDR) |= (0b11<<(pin*2));
				break;
			default:
				return;
		}
	}		
}

/*
 * This function allows a user to set any pin on a port to one of the 3 pull types. The types
 * are specified by enumerated types NONE, PULLUP, and PULLDOWN. Protection against
 * setting invalid pins is implemented.
 * Inputs:
 * 		char port - port to modify pins on
 * 		uint8_t pin - pin to be modified
 * 		PullType pulltype - pulltype to set pin to.
 * Outputs:
 * 		none
 */
void set_pin_PUPDR(char port, uint8_t pin, PullType pulltype){
	if(pin >= 0 && pin <= 15){
		volatile uint32_t* PUPDR;
	
		//assign the PUPDR according to the proper port selected
		switch(port){
			case 'A':
			case 'a':
				PUPDR = GPIOA_PUPDR;
				break;
			case 'B':
			case 'b':
				PUPDR = GPIOB_PUPDR;
				break;
			case 'C':
			case 'c':
				PUPDR = GPIOC_PUPDR;
				break;
			default:
				return;		//error
		}

		//clear pull type set at specified pin
		*(PUPDR) &= ~(0b11<<(pin*2));

		//set pin the the specified pull type
		switch(pulltype){
			case NONE:
				*(PUPDR) |= (0b00<<(pin*2));
				break;
			case PULLUP:
				*(PUPDR) |= (0b01<<(pin*2));
				break;
			case PULLDOWN:
				*(PUPDR) |= (0b10<<(pin*2));
				break;
			default:
				return;
		}
	}		
}

/*
 * This function allows a user to set any pin on a port to one of the 15 alternate modes. The modes
 * are specified by their respective number specified in the programmers manual. Protection against
 * setting invalid pins is implemented.
 * Inputs:
 * 		char port - port to modify pins on
 * 		uint8_t pin - pin to be modified
 * 		uint8_t altFunc - alternate function to set pin to.
 * Outputs:
 * 		none
 */
void set_alt_func(char port, uint8_t pin, uint8_t altFunc){
	if(pin >= 0 && pin <=7){
		set_low_reg_alt_func(port,pin,altFunc);		//if pin corresponds to pin 0-7, use the alternate function low register
	}else if(pin >=8 && pin <= 15){
		set_high_reg_alt_func(port,pin,altFunc);	//if pin corresponds to pin 8-15, use the alternate function high register
	}else{
		return;
	}
}

static void set_low_reg_alt_func(char port, uint8_t pin, uint8_t altFunc){
	volatile uint32_t* AFRL;
	
	switch(port){
			case 'A':
			case 'a':
				AFRL = GPIOA_AFRL;
				break;
			case 'B':
			case 'b':
				AFRL = GPIOB_AFRL;
				break;
			case 'C':
			case 'c':
				AFRL = GPIOC_AFRL;
				break;
			default:
				return;		//error
		}
				
		*(AFRL) &= ~(0b1111<<(pin*4));

		switch(altFunc){
			case 0:
				*(AFRL) |= (0b0000<<(pin*4));
				break;
			case 1:
				*(AFRL) |= (0b0001<<(pin*4));
				break;
			case 2:
				*(AFRL) |= (0b0010<<(pin*4));
				break;
			case 3:
				*(AFRL) |= (0b0011<<(pin*4));
				break;
			case 4:
				*(AFRL) |= (0b0100<<(pin*4));
				break;
			case 5:
				*(AFRL) |= (0b0101<<(pin*4));
				break;
			case 6:
				*(AFRL) |= (0b0110<<(pin*4));
				break;
			case 7:
				*(AFRL) |= (0b0111<<(pin*4));
				break;
			case 8:
				*(AFRL) |= (0b1000<<(pin*4));
				break;
			case 9:
				*(AFRL) |= (0b1001<<(pin*4));
				break;
			case 10:
				*(AFRL) |= (0b1010<<(pin*4));
				break;
			case 11:
				*(AFRL) |= (0b1011<<(pin*4));
				break;
			case 12:
				*(AFRL) |= (0b1100<<(pin*4));
				break;
			case 13:
				*(AFRL) |= (0b1101<<(pin*4));
				break;
			case 14:
				*(AFRL) |= (0b1110<<(pin*4));
				break;
			case 15:
				*(AFRL) |= (0b1111<<(pin*4));
				break;
			default:
				return;
		}
}

static void set_high_reg_alt_func(char port, uint8_t pin,uint8_t altFunc){
	volatile uint32_t* AFRH;
	pin -= 8;
	
	switch(port){
			case 'A':
			case 'a':
				AFRH = GPIOA_AFRH;
				break;
			case 'B':
			case 'b':
				AFRH = GPIOB_AFRH;
				break;
			case 'C':
			case 'c':
				AFRH = GPIOC_AFRH;
				break;
			default:
				return;		//error
		}
				
		*(AFRH) &= ~(0b1111<<(pin*4));

		switch(altFunc){
			case 0:
				*(AFRH) |= (0b0000<<(pin*4));
				break;
			case 1:
				*(AFRH) |= (0b0001<<(pin*4));
				break;
			case 2:
				*(AFRH) |= (0b0010<<(pin*4));
				break;
			case 3:
				*(AFRH) |= (0b0011<<(pin*4));
				break;
			case 4:
				*(AFRH) |= (0b0100<<(pin*4));
				break;
			case 5:
				*(AFRH) |= (0b0101<<(pin*4));
				break;
			case 6:
				*(AFRH) |= (0b0110<<(pin*4));
				break;
			case 7:
				*(AFRH) |= (0b0111<<(pin*4));
				break;
			case 8:
				*(AFRH) |= (0b1000<<(pin*4));
				break;
			case 9:
				*(AFRH) |= (0b1001<<(pin*4));
				break;
			case 10:
				*(AFRH) |= (0b1010<<(pin*4));
				break;
			case 11:
				*(AFRH) |= (0b1011<<(pin*4));
				break;
			case 12:
				*(AFRH) |= (0b1100<<(pin*4));
				break;
			case 13:
				*(AFRH) |= (0b1101<<(pin*4));
				break;
			case 14:
				*(AFRH) |= (0b1110<<(pin*4));
				break;
			case 15:
				*(AFRH) |= (0b1111<<(pin*4));
				break;
			default:
				return;
		}
}



