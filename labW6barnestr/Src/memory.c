/**
 * @file memory.c
 * @author Trevor Barnes
 * @brief 
 * @version 0.1
 * @date 2022-01-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include "memory.h"

#define F_CPU 16000000UL

void initMemConsole() {
    init_usart2(57600, F_CPU);
    printf("Memory Console Initialized! Type 'help' for info.\n\r");
}

void readMem(uint32_t addr) {
	// Assign and casts a new int pointer the value of addr
	uint32_t * memPtr = (uint32_t *)addr;
	// Formatted print with both hex and decimal values
	printf("Memory Value at %#08x\n\r"
		   "Hex: %#08x\n\r"
		   "Decimal: %d\n\r", addr, *memPtr, *memPtr);
	return;
}

void writeMem(uint32_t addr, uint32_t data) {
	// Assign and casts a new int pointer the value of addr
	uint32_t * memPtr = (uint32_t *)addr;
	// Write data
	*memPtr = data;
	// Confirmation printout showing the new value and address
	printf("Value written at  %#08x: %u \n\r", addr, data);
	return;
}


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
