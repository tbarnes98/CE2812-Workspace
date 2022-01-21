/*
 * keypad.h
 *
 * Created on: Dec 17, 2021
 * Author: Trevor Barnes
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <inttypes.h>


#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define GPIOCEN 2
#define GPIOC_MODER (volatile uint32_t*) 0x40020800
#define GPIOC_PUPDR (volatile uint32_t*) 0x4002080C
#define GPIOC_IDR (volatile uint32_t*) 0x40020810
#define GPIOC_ODR (volatile uint32_t*) 0x40020814
#define GPIOC_BSRR (volatile uint32_t*) 0x40020818

extern void keypad_init();

//Returns the code for the current key pressed
//	0 - No key
//  1  2   3  4
//
//  5  6  7  8
//
//  9 10  11 12
//
//  13 14 15 16
// 255 - Error
// This function does not block the flow of the program
extern uint8_t keypad_getKeyNoBlock();

//Returns the code for the next pressed
//	0 - No key
//  1  2   3  4
//
//  5  6  7  8
//
//  9 10  11 12
//
//  13 14 15 16
// 255 - Error
// This function will block the flow of the program until a
//   key is pressed.
extern uint8_t keypad_getKey();

#endif /* KEYPAD_H_ */
