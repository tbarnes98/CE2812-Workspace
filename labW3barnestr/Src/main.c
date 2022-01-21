/*
 * main.c
 *
 *  Created on: Dec 17, 2021
 *  Author: Trevor Barnes
 *  A driver file to demonstrate the implemented functionality of the LCD and keypad. This file
 *  contains functionality for a simple 4-operation calculator with two operands.
 */
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "keypad.h"

char mathKeys[] = {'1','2','3','+','4','5','6','-','7','8','9','x','*','0','=','/'};
char mathNums[] = {'1','2','3','_','4','5','6','_','7','8','9','_','_','0','_','_'};
char mathOper[] = {'_','_','_','+','_','_','_','-','_','_','_','x','*','_','_','/'};
char calculation[5];

void calculate(int operand1, int operand2, char operation) {
	int result;
	switch (operation) {
		case '+':
		result = operand1 + operand2;
		break;
		case '-':
		result = operand1 - operand2;
		break;
		case 'x':
		result = operand1 * operand2;
		break;
		default:
		if (operand2 == 0) {
			lcd_clear();
			char divError[] = " Err: Undefined ";
			lcd_print_string(divError);
			return;
		} else {
			result = operand1 / operand2;
		}
	}
	lcd_print_num(result);
}

void inputSequence() {
	int currentKeyIndex;
	lcd_clear();
	int seqIndex = 0;

	// While key != number, getKey
	do {
		currentKeyIndex = keypad_getKey()-1;
		if (mathKeys[currentKeyIndex] == '*') {
			return;
		}
	} while (mathKeys[currentKeyIndex] != mathNums[currentKeyIndex]);
	calculation[seqIndex] = mathKeys[currentKeyIndex];
	lcd_print_char(mathKeys[currentKeyIndex]);
	seqIndex++;

	// While key != operation, getKey
	do {
		currentKeyIndex = keypad_getKey()-1;
		if(mathKeys[currentKeyIndex] == '*') {
			return;
		}
	} while (mathKeys[currentKeyIndex] != mathOper[currentKeyIndex]);
	calculation[seqIndex] = mathKeys[currentKeyIndex];
	lcd_print_char(mathKeys[currentKeyIndex]);
	seqIndex++;

	// While key != number, getKey
	do {
		currentKeyIndex = keypad_getKey()-1;
		if(mathKeys[currentKeyIndex] == '*') {
			return;
		}
	} while (mathKeys[currentKeyIndex] != mathNums[currentKeyIndex]);
	calculation[seqIndex] = mathKeys[currentKeyIndex];
	lcd_print_char(mathKeys[currentKeyIndex]);
	seqIndex++;

	// While key != '='
	do {
		currentKeyIndex = keypad_getKey()-1;
		if(mathKeys[currentKeyIndex] == '*') {
			return;
		}
	} while (mathKeys[currentKeyIndex] != '=');
	lcd_print_char(mathKeys[currentKeyIndex]);
	// Calculate
	calculate(calculation[0] -= '0', calculation[2] -= '0', calculation[1]);
	do {
		currentKeyIndex = keypad_getKey()-1;
	} while (mathKeys[currentKeyIndex] != '*');
}

// main
int main(){

	// Initialize Components
	keypad_init();
	lcd_init();

	// Prompt Message
	char prompt1[]= "+- Calculator x/";
	char prompt2[]= "  Push any key  ";
	lcd_print_string(prompt1);
	lcd_set_position(17);
	lcd_print_string(prompt2);
	keypad_getKey();

	while(1) {
		inputSequence();
	}
	return 0;
}



