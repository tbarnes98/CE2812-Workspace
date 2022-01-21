/*
 * delay.c
 *
 */

#include <inttypes.h>
#include "delay.h"  //include declaration header file

void delay_ms(uint32_t n){

	// 1ms = 16,000 ticks
	for (int i = n ; i > 0 ; i--) {
		// Clear value register
		*STK_VAL = 0x0000;
		// Store 16,000 in STK_LOAD
		*STK_LOAD = 16000;
		// Enable clock, no prescaler, no interrupt
		*STK_CTRL |= CLKSOURCE;
		*STK_CTRL |= EN;
		// Loop n times: Wait for countflag high
		int flag;
		do {
			flag = ((*STK_CTRL & (1<<16))>>16);
		} while (flag != 1);
	}

}

void delay_us(uint32_t n) {
		// 1us = 16 ticks
		for (int i = n ; i > 0 ; i--) {
			// Clear value register
			*STK_VAL = 0x0000;
			// Store 16 in STK_LOAD
			*STK_LOAD = 16;
			// Enable clock, no prescaler, no interrupt
			*STK_CTRL |= CLKSOURCE;
			*STK_CTRL |= EN;
			// Loop n times: Wait for countflag high
			int flag;
			do {
				flag = ((*STK_CTRL & (1<<16))>>16);
			} while (flag != 1);
		}
}
