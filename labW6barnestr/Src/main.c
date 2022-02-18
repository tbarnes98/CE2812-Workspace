/**
 * @file main.c
 * @author Trevor Barnes
 * @brief 
 * @version 0.1
 * @date 2022-01-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "uart_driver.h"
#include "memory.h"
#include "led.h"
#include "delay.h"
#include "piezoSpeaker.h"

#define F_CPU 16000000UL

void printHelp() {
	printf("*Commands*\n\r");
	printf("'rmw {hex address}' - Reads mem at a given address\n\r");
	printf("'wmw {hex address} {value}' - Writes the given value as a word to the given address\n\r");
	printf("'dm {hex address} {length}' - Dumps the memory at a given address. Defaults to 16 B if no "
			"length is given\n\r");
	printf("'ps {song choice}' - Plays a song with the given selection\n\r");
	printf("'songs' - Prints info about each song selection\n\r");
}
void songInfo() {
	printf("Type 1 or 2 to play a song!\n\r");
	printf("Song 1: Imperial March from Star Wars\n\r");
	printf("Song 2: Metropolis Theme from Ratchet & Clank\n\r");
	printf("Song 3: Flower Garden from Yoshi's Island\n\r");
}

int main(void) {
	init_usart2(57600, F_CPU);
	piezo_init();
	led_init();
	char line[50];
	char command[10];
	int address;
	int data;
	int length;
	int songSelection;
	char background;
	for(;;) {
		// Get command from user
		fgets(line, 100, stdin);
		// Parse only the command for strcmp
		sscanf(line, "%s", command);
		if (!strcmp(command, "help")) {
			// Print Help
			printHelp();
		} else if (!strcmp(command, "songs")) {
			// Print Song Info
			songInfo();
		} else if (!strcmp(command, "rmw")) {
			// Read Memory
			sscanf(line, "%s %X", command, &address);
			readMem(address);
		} else if (!strcmp(command, "wmw")) {
			// Write Memory
			sscanf(line, "%s %X %u", command, &address, &data);
			writeMem(address, data);
		} else if (!strcmp(command, "dm")) {
			// Dump Memory
			sscanf(line, "%s %X %u", command, &address, &length);
			dumpMem(address, length);
		} else if (!strcmp(command, "ps")) {
			// Song Selection Command Format:
			// "ps {songSelection} {background}"
			sscanf(line, "%s %u %c", command, &songSelection, &background);
			if (background == 'b') {
				switch(songSelection) {
				case 1:
					printf("Playing Imperial March in the background\n\r");
					play_song_br(songIM);
					break;
				case 2:
					printf("Playing Metropolis Theme in the background\n\r");
					play_song_br(songMT);
					break;
				case 3:
					printf("Playing Flower Garden in the background\n\r");
					play_song_br(songFG);
					break;
				default:
					break;
					printf("Invalid song selection\n\r");
				}
			} else {
				// Without valid flag, program plays songs in foreground
				switch(songSelection) {
				case 1:
					printf("Playing Imperial March\n\r");
					play_song(songIM);
					break;    
				case 2:
					printf("Playing Metropolis Theme\n\r");
					play_song(songMT);
					break;
				case 3:
					printf("Playing Flower Garden\n\r");
					play_song(songFG);
					break;
				default:
					break;
					printf("Invalid song selection\n\r");
				}
			}
			
		} else {
			printf("Invalid input, type 'help' for instructions\n\r");
		}
	}
}
