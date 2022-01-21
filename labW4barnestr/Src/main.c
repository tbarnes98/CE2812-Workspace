/*
 * main.c
 *	CE2812
 *  Created on: January 7, 2022
 *  Author: Trevor Barnes
 *  Description: This program allows a user to interact with different memory operations through
 *  a terminal connected to the serial port. Different operations include: reading memory at a
 *  given address, writing an in value to a given address, and providing a memory dump of a certain
 *  byte length at a given memory address.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart_driver.h"
#include "led.h"
#include "delay.h"

#define F_CPU 16000000UL

/**
 * Reads and prints the memory value at address provided: "addr"
 */
void readMem(uint32_t addr) {
	// Assign and casts a new int pointer the value of addr
	uint32_t * memPtr = (uint32_t *)addr;
	// Formatted print with both hex and decimal values
	printf("Memory Value at %#08x\n\r"
		   "Hex: %#08x\n\r"
		   "Decimal: %d\n\r", addr, *memPtr, *memPtr);
	return;
}
/**
 * Writes the provided "data" value as an unsigned 32-bit word at the provided address: "addr"
 */
void writeMem(uint32_t addr, uint32_t data) {
	// Assign and casts a new int pointer the value of addr
	uint32_t * memPtr = (uint32_t *)addr;
	// Write data
	*memPtr = data;
	// Confirmation printout showing the new value and address
	printf("Value written at  %#08x: %u \n\r", addr, data);
	return;
}

/**
 * Prints out formatted, hexadecimal memory values in "byte-sized" chunks starting at the provided
 * memory address: "addr". The length of the memory dump is provided by "length".
 */
void dumpMem(uint32_t addr, int length) {
	// Set length to default value if length is negative
	// (No limit or protection for large, overflow values yet)
	if(length <= 0) {
		length = 16;
		printf("Length set to default! (16)\n\r");
	}
	// Assign and casts a new int pointer the value of addr
	uint8_t * memPtr = (uint8_t *)addr;
	// Loop that executes each read and print operation
	for(int i=0 ; i < length ; i++) {
		// Print newline and memory location every 16 bytes
		if((i % 16) == 0) {
			printf("\n\r%p:", memPtr);
		}
		// Print each byte
		printf(" %02X", *memPtr);
		// Iterate pointer to next byte
		memPtr++;
		}
	printf("\n\r");
	return;
}

/**
 * Prints a help dialog that provides the user the list of available commands
 */
void printHelp() {
	printf("*Commands*\n\r");
	printf("'rmw {hex address}' - Reads mem at a given address\n\r");
	printf("'wmw {hex address} {value}' - Writes the given value as a word to the given address\n\r");
	printf("'dm {hex address} {length}' - Dumps the memory at a given address. Defaults to 16 B if no "
			"length is given\n\r");
}

/**
 * Main function that handles usart/led initialization and command input/parsing from the user
 */
int main(void) {
	init_usart2(57600,F_CPU);
	led_init();
	char line[50];
	char command[10];
	uint32_t address;
	uint32_t data;
	int length;
	for(;;) {
		// Get command from user
		fgets(line, 100, stdin);
		// Parse only the command for strcmp
		sscanf(line, "%s", command);
		if (!strcmp(command, "help")) {
			printHelp();
		} else if (!strcmp(command,"rmw")) {
			// Parse line again with expected format and values
			sscanf(line, "%s %X", command, &address);
			//printf("Command: %s Address: %x\n\r", command, address);
			readMem(address);
		} else if (!strcmp(command, "wmw")) {
			// Parse line again with expected format and values
			sscanf(line, "%s %X %u", command, &address, &data);
			//printf("Command: %s Address: %x Data: %u\n\r", command, address, data);
			writeMem(address, data);
		} else if (!strcmp(command, "dm")) {
			// Parse line again with expected format and values
			sscanf(line, "%s %X %u", command, &address, &length);
			//printf("Command: %s Address: %x Length: %d\n\r", command, address, length);
			dumpMem(address, length);
		} else {
			printf("Invalid input, type 'help' for instructions\n\r");
		}
	}
}
