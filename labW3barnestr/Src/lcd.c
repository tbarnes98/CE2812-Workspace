/*
 * lcd.c
 *
 *  Created on: Dec 17, 2020
 *  Author: Trevor Barnes
 */

#include "lcd.h"
#include "delay.h"
#include <inttypes.h>
#include <stdio.h>



void writeInstruction(uint8_t inst) {
	// Reset RS, RW, E
	*GPIOC_ODR &= ~(RS);
	*GPIOC_ODR &= ~(RW);
	*GPIOC_ODR &= ~(E);
	// Set E
	*GPIOC_ODR |= (E);
	// Clear databus
	*GPIOA_ODR &= ~(0xFF<<4);
	// Set databus to passed in instruction value
	*GPIOA_ODR |= (inst<<4);
	// Reset E
	*GPIOC_ODR &= ~(E);
	delay_us(37);
}

void writeData(uint8_t inst) {
	// Reset RW, E
	*GPIOC_ODR &= ~(RW);
	*GPIOC_ODR &= ~(E);
	// Set RS
	*GPIOC_ODR |= (RS);
	// Set E
	*GPIOC_ODR |= (E);
	// Clear databus
	*GPIOA_ODR &= ~(0xFF<<4);
	// Set databus to passed in instruction value
	*GPIOA_ODR |= (inst<<4);
	// Reset E
	*GPIOC_ODR &= ~(E);
	delay_us(37);
}

void lcd_init(){

// Port Setup
	// Enable GPIO A and C in RCC
	*RCC_AHB1ENR |= (GPIOAEN||GPIOCEN);
	// Set DB pins to outputs
	*GPIOA_MODER &= ~(0xFFFF<<8);
	*GPIOA_MODER |= (0x5555<<8);
	// Set RS, RW, and E pins to outputs
	*GPIOC_MODER &= ~(0x3F<<16);
	*GPIOC_MODER |= (0x15<<16);
	// Delay for 40 ms
	delay_ms(40);

// Display Setup
	// Write Function Set
	writeInstruction(FUNCTION_SET);
	delay_us(37);
	// Write Function Set
	writeInstruction(FUNCTION_SET);
	delay_us(37);
	// Write Display On
	writeInstruction(DISPLAY_TOGGLE);
	delay_us(37);
	// Write Display Clear
	writeInstruction(DISPLAY_CLEAR);
	delay_ms(2);
	// Write Entry Mode Set
	writeInstruction(ENTRY_MODE_SET);
	delay_us(37);

}

void lcd_clear(){
	writeInstruction(DISPLAY_CLEAR);
	delay_ms(2);
}

void lcd_home() {
	writeInstruction(0x80);
	delay_ms(2);
}

void lcd_set_position(uint8_t posIndex) {
	posIndex--;
	int inst = 0x80;
	if (posIndex > 15) {
		inst |= (1<<6);
		posIndex -= 16;
	}
	inst |= posIndex;
	writeInstruction(inst);
}

int lcd_print_string(char* str) {
	int index = 0;
	while (str[index] != 0x00) {
		writeData(str[index]);
		index++;
	}
	return index;
}

void lcd_print_char(char c) {
	writeData(c);
}


int lcd_print_num(int i){
	char numString[17];
	sprintf(numString, "%d", i);
	return lcd_print_string(numString);
}
