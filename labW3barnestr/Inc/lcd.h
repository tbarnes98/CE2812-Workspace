/*
 * LCD.H
 *
 * Created on: Dec 17, 2020
 * Author: Trevor Barnes
 */

//Include Guards
#ifndef LCD_H_
#define LCD_H_

#include <inttypes.h>

#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830

// GPIO A Addresses and Values
#define GPIOA_MODER (volatile uint32_t*) 0x40020000
#define GPIOA_IDR (volatile uint32_t*) 0x40020010
#define GPIOA_ODR (volatile uint32_t*) 0x40020014
#define GPIOA_BSRR (volatile uint32_t*) 0x40020018
#define GPIOAEN (1<<0)

// GPIO C Addresses and Values
#define GPIOC_MODER (volatile uint32_t*) 0x40020800
#define GPIOC_IDR (volatile uint32_t*) 0x40020810
#define GPIOC_ODR (volatile uint32_t*) 0x40020814
#define GPIOC_BSRR (volatile uint32_t*) 0x40020818
#define GPIOCEN 2

#define RS (1<<8)
#define RW (1<<9)
#define E (1<<10)

#define FUNCTION_SET 0x38
#define DISPLAY_TOGGLE 0x0F
#define DISPLAY_CLEAR 0x01
#define ENTRY_MODE_SET 0x06


/*
 * lcdInit()
 * Initializes the lcd by:
 * - Setting DB and Control Ports to Outputs
 * - Turns on clears the display
 */
void lcd_init();

/*
 * lcdClear()
 * Clears the lcd
 */
void lcd_clear();

/*
 * lcdHome()
 * Sets the cursor to the home position
 */
void lcd_home();

/*
 * lcdSetPosition()
 * Sets the cursor to the index passed in (1-32)
 */
void lcd_set_position(uint8_t posIndex);

/*
 * lcdPrintString()
 *
 */
int lcd_print_string(char* str);

/*
 * lcdPrintChar()
 *
 */
void lcd_print_char(char c);

/*
 * lcdPrintNum()
 *
 */
int lcd_print_num(int i);

#endif /* LCD_H */
