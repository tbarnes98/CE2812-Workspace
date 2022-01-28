/**
 * @file memory.h
 * @author Trevor Barnes
 * @brief 
 * @version 0.1
 * @date 2022-01-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//include guards
#ifndef MEMORY_H_
#define MEMORY_H_

#include <inttypes.h>
#include "uart_driver.h"

void initMemConsole();

/**
 * Reads and prints the memory value at address provided: "addr"
 */
void readMem(uint32_t addr);
/**
 * Writes the provided "data" value as an unsigned 32-bit word at the provided address: "addr"
 */
void writeMem(uint32_t addr, uint32_t data);
/**
 * Prints out formatted, hexadecimal memory values in "byte-sized" chunks starting at the provided
 * memory address: "addr". The length of the memory dump is provided by "length".
 */
void dumpMem(uint32_t addr, int length);

#endif /* MEMORY_H_ */
