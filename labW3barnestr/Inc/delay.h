/*
 * delay.h
 *
 * Author: Trevor Barnes
 */

//include guards
#ifndef DELAY_H_
#define DELAY_H_

#define STK_CTRL (volatile uint32_t*) 0xE000E010
#define STK_LOAD (volatile uint32_t*) 0xE000E014
#define STK_VAL (volatile uint32_t*) 0xE000E018

#define EN 1
#define TICKINT (1<<1)
#define CLKSOURCE (1<<2)
#define COUNTFLAG (1<<16)

/*
 * delay_ms
 * 		Busy wait for n ms
 *
 * 		For n iterations
 * 			load number of cycles for 1 ms
 * 			set one to enable and clock source
 *
 * 			wait for countflag to be set
 */
void delay_ms(uint32_t n);

/*
 * delay_ms
 * 		Busy wait for n us
 *
 * 		For n iterations
 * 			load number of cycles for 1 us
 * 			set one to enable and clock source
 *
 * 			wait for countflag to be set
 */
void delay_us(uint32_t n);

#endif /* DELAY_H_ */
