/*
 * main.c
 *	CE2812
 *  Created on: Dec 10, 2021
 *  Author: Trevor Barnes
 *  Description: This program allows the user to control the led bar on the dev board. Users can
 *  flash the leds or scan them back and forth like the lights on KITT from Knight Rider. The speed
 *  of the lights can also be adjusted and returned back. This is all controlled through a serial
 *  terminal using the usart.
 */
#include <stdio.h>
#include <stdlib.h>
#include "uart_driver.h"
#include "led.h"

#define F_CPU 16000000UL
#define ARSIZE 10

void printHelp() {
		printf("*Commands*\n\r");
		printf("'f' - Flash Lights 10 Times\n\r");
		printf("'s' - Scan Lights (Once back and forth)\n\r");
		printf("'i' - Scan Lights (Infinitely)\n\r");
		printf("'p' - Print Current Speed Interval (0-9)\n\r");
		printf("'+' - Increase Speed\n\r");
		printf("'-' - Decrease Speed\n\r");
		printf("'x' - Turn Off Lights\n\r");
		printf("'o' - Turn On Lights\n\r");
	}

// main
int main(){

	init_usart2(19200,F_CPU);

	// Initialize LEDs
	led_init();

	char selection, dummy;

	// Selection Prompt
	printf("Choose an option ('h' for help)\n\r");
	while (1) {
		selection = getchar();
		switch (selection) {
		case 'o':
			led_allOn();
			break;
		case 'x':
			led_allOff();
			break;
		case 'h':
			printHelp();
			break;
		case 'f':
			led_flash();
			break;
		case 's':
			led_scan();
			break;
		case 'i':
			while(1){
				led_scan();
			}
			break;
		case 'p':
			printf("Current Interval: %d\n\r", getCurrentSpeed());
			break;
		case '+':
			led_incSpeed();
			break;
		case '-':
			led_decSpeed();
			break;
		default:
			printf("????\n\r");
			break;
		}
		dummy = getchar();
		dummy = dummy; // No more warning
	}
	return 0;
}
