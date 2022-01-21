/*
 * led.c
 *
 *  Created on: Dec 10, 2021
 *  Author: Trevor Barnes
 */

#include "led.h"
#include "delay.h"
#include <inttypes.h>
#include <stdio.h>

int ledSpeed = 5;

void led_init(){
	// Initialize corresponding RCC and GPIO registers
	*RCC_AHB1ENR |= (1<<GPIOBEN);

	*GPIOB_MODER &= ~(0x3FFF<<10);
	*GPIOB_MODER |= (0x555<<10);

	*GPIOB_MODER &= ~(0xFF<<24);
	*GPIOB_MODER |= (0x55<<24);
}

void led_allOn(){
	// Set all LED Bits
	*GPIOB_ODR |= ALL_LEDS;
}


void led_allOff(){
	// Reset all LED bits
	*GPIOB_ODR &= ~(ALL_LEDS);

}

void led_on(uint8_t ledIndex){
	// Set individual LED based on passed in index
	switch (ledIndex) {
	case 0:
		*GPIOB_BSRR = (1<<5);
		break;
	case 1:
		*GPIOB_BSRR = (1<<6);
		break;
	case 2:
		*GPIOB_BSRR = (1<<7);
		break;
	case 3:
		*GPIOB_BSRR = (1<<8);
		break;
	case 4:
		*GPIOB_BSRR = (1<<9);
		break;
	case 5:
		*GPIOB_BSRR = (1<<10);
		break;
	case 6:
		*GPIOB_BSRR = (1<<12);
		break;
	case 7:
		*GPIOB_BSRR = (1<<13);
		break;
	case 8:
		*GPIOB_BSRR = (1<<14);
		break;
	case 9:
		*GPIOB_BSRR = (1<<15);
		break;
	default:
		printf("LED index out of range\n\r");
		break;
	}
}

void led_off(uint8_t ledIndex){
	// Reset individual LED based on passed in index
	if (ledIndex < 6) {
		*GPIOB_BSRR = (1<<(21+ledIndex));
	} else if (ledIndex >= 6) {
		// Add pin offset to index
		*GPIOB_BSRR = (1<<(22+ledIndex));
	} else {
		printf("LED index out of range\n\r");
	}
}

void led_scan(){
	led_allOff();
	// Right to left each LED
	for (int i = 0; i <= 9 ; i++) {
		// Scaled Delay
		delay_1ms(50+(ledSpeed*50));
		if(i != 0){
			led_off(i-1);
		}
		led_on(i);
	}
	// Left to right each LED
	for (int i = 9; i >= 0; i--) {
		if(i != 9){
			led_off(i+1);
		}
		led_on(i);
		// Scaled Delay
		delay_1ms(50+(ledSpeed*50));
	}
	led_off(0);
}

void led_flash(){
	// Flash LED on and off 10 times at a speed between 0-1 seconds
	for (int i = 0; i < 10; i++) {
		led_allOn();
		delay_1ms(100+(ledSpeed*100));
		led_allOff();
		delay_1ms(100+(ledSpeed*100));
	}
}

void led_setSpeed(uint8_t speed){
	ledSpeed = speed;
}

void led_incSpeed(){
	if (ledSpeed == 0){
			printf("Speed too fast\n\r");
		} else {
			ledSpeed--;
		}
}

void led_decSpeed(){
	if (ledSpeed == 9){
		printf("Speed too slow\n\r");
	} else {
		ledSpeed++;
	}
}

uint8_t getCurrentSpeed()
{
	return ledSpeed;
}


