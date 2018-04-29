/*
 * keypad.h
 *
 *  Created on: Jan. 6 2017
 *      Author: Mitchell Larson
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include <inttypes.h>
#include "gpio.h"

//global functions
void key_init();
uint8_t key_getkey_noblock();
uint8_t key_getkey();
char key_getchar();
char key_getchar_noblock();
uint8_t key_getint();

#endif /* KEYPAD_H */
