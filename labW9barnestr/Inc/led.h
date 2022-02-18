/*
 * led.h
 *
 * Created on: Dec 10, 2021
 * Author: Trevor Barnes
 */

#ifndef LED_H_
#define LED_H_

#include <inttypes.h>

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define GPIOBEN 1
#define GPIOB_MODER (volatile uint32_t*) 0x40020400
#define GPIOB_PUPDR (volatile uint32_t*) 0x4002040C
#define GPIOB_IDR (volatile uint32_t*) 0x40020410
#define GPIOB_ODR (volatile uint32_t*) 0x40020414
#define GPIOB_BSRR (volatile uint32_t*) 0x40020418

#define ALL_LEDS (0b11110111111<<5)

/*
 * led_init()
 * This function should:
 * 		1. Enable the GPIOB in RCC_AHB1ENR
 * 		2. Turn on to set LED0 - LED9 to output mode ("01")
 */
void led_init();

/*
 * led_allOn()
 * 		1. Turn on all leds (hint use ODR or BSRR)
 * 		Note you should not effect other pins on PortB
 */
void led_allOn();

/*
 * led_allOff()
 * 		1. Turn off all leds (hint use ODR or BSRR)
 * 		Note you should not effect other pins on PortB
 */
void led_allOff();

/*
 * led_on()
 * 		Args: 0-9 to turn on specific led
 * 		print error message is arg is out of range
 */
void led_on(uint8_t ledIndex);

/*
 * led_off()
 * 		Args: 0-9 to turn off specific led
 * 		print error message is arg is out of range
 */
void led_off(uint8_t ledIndex);


/*
 * led_scan()
 * 		Scan the light across and back at the current speed
 */
void led_scan();

/*
 * led_flash()
 * 		flash all of the lights 10 times at the current speed
 */
void led_flash();

/*
 * led_setSpeed (uint8_t speed)
 * 		arg: speed (0 slow - 9 fast)
 * 		Args out of range should print error to console
 */
void led_setSpeed(uint8_t speed);

/*
 * led_incSpeed()
 * 		increases the speed by one
 * 		if maxed out leaves the speed at the max value
 */
void led_incSpeed();

/*
 * led_decSpeed()
 * 		decreases the speed by one
 * 		if at zero should stay at zero
 */
void led_decSpeed();

/*
 * getCurrentSpeed
 * 		returns the current speed
 */
uint8_t getCurrentSpeed();



#endif
