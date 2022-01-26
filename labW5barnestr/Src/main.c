/*
 * main.c
 *
 *  Created on: January 12, 2022
 *  Author: Trevor Barnes
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

// Super Mario "Flagpole Fanfare"
Note songSMFF[66]= { {G3, S/3}, {Ab3,S/3}, {A3, S/3}, {Bb3,S/3}, {B3,S/3},
	 			 	 {C4, S/3}, {Db4,S/3}, {D4, S/3}, {Eb4,S/3}, {E4,S/3},
	 			 	 {F4, S/3}, {Gb4,S/3}, {G4, S/3}, {Ab4,S/3}, {A4,S/3},
	 			 	 {Bb4,S/3}, {B4, S/3}, {C5, S/3}, {Db5,S/3}, {D5,S/3},
	 			 	 {Eb5,S/3}, {E5, S/3}, {F5, S/3}, {Gb5,S/3}, {G5,S/3},
	 			 	 {Ab5,S/3}, {A5, S/3}, {Bb5,S/3}, {B5, S/3}, {C6,S/3},
	 			 	 {Db6,S/3}, {D6, S/3}, {Eb6,S/3}, {E6, S/3}, {F6,S/3},
	 			 	 {Eb6,S/3}, {G6, S/3}, {r,  7*S}, {G3,   Q}, {C4,  Q},
	 			 	 {E4,   Q}, {G4,   Q}, {C5,   Q}, {E5,   Q}, {G5,  H},
	 			 	 {E5,   H}, {Ab3,  Q}, {C4,   Q}, {Eb4,  Q}, {Ab4, Q},
	 			 	 {C5,   Q}, {Eb5,  Q}, {Ab6,  H}, {Eb5,  H}, {Bb3, Q},
	 			 	 {D4,   Q}, {F4,   Q}, {Bb4,  Q}, {D5,   Q}, {F5,  Q},
	 			 	 {Bb5,  W}, {B5,   Q}, {B5,   Q}, {B5,   Q}, {C6,  W}, {T,  Q} };

// Imperial March
Note songIM[138] = { {A3,  Q}, {r,   Q}, {A3,   Q}, {r,    Q}, {A3,   Q}, {r,   Q},
 					 {F3,E+S}, {r, E+S}, {C4,   S}, {r,    S}, {A3,   Q}, {r,   Q},
 					 {F3,E+S}, {r, E+S}, {C4,   S}, {r,    S}, {A3,   H}, {r,   H},
 					 {E4,  Q}, {r,   Q}, {E4,   Q}, {r,    Q}, {E4,   Q}, {r,   Q},
 					 {F4,E+S}, {r, E+S}, {C4,   S}, {r,    S}, {Ab3,  Q}, {r,   Q},
 					 {F3,E+S}, {r, E+S}, {C4,   S}, {r,    S}, {A3,   H}, {r,   H},
 					 {A4,  Q}, {r,   Q}, {A3, E+S}, {r,  E+S}, {A3,   S}, {r,   S},
 					 {A4,  Q}, {r,   Q}, {Ab4,E+S}, {r,  E+S}, {G4,   S}, {r,   S},
 					 {Gb4, Q}, {r,   S}, {E4,   S}, {r,    S}, {F4,   E}, {r,   E},
 					 {r,   E}, {Bb3, E}, {r,    E}, {Eb4,  Q}, {r,    Q}, {D4,E+S},
 					 {r, E+S}, {Db4, S}, {r,    H}, {C4,   S}, {r,    S}, {B3,  S},
 					 {r,   S}, {C4,  E}, {r,    E}, {r,    E}, {F3,   E}, {r,   E},
 					 {Ab3, Q}, {r,   Q}, {F3, E+S}, {r,  E+S}, {A3,   S}, {r,   S},
 					 {C4,  Q}, {r,   Q}, {A3, E+S}, {r,  E+S}, {C4,   S}, {r,   S},
 					 {E4,  H}, {r,   H}, {A4,   Q}, {r,    Q}, {A3, E+S}, {r, E+S},
 					 {A3,  S}, {r,   S}, {A4,   Q}, {r,    S}, {Ab4,E+S}, {r, E+S},
 					 {G4,  S}, {r,   S}, {Gb4,  S}, {r,    S}, {E4,   S}, {r,   S},
 					 {F4,  E}, {r,   E}, {r,    E}, {Bb3,  E}, {r,    E}, {Eb4, Q},
 					 {r,   Q}, {D4,E+S}, {r,  E+S}, {Db4,  S}, {r,    S}, {C4,  S},
 					 {r,   S}, {B3,  S}, {r,    S}, {C4,   E}, {r,    E}, {r,   E},
 					 {F3,  E}, {r,   E}, {Ab3,  Q}, {r,    Q}, {F3, E+S}, {r, E+S},
 					 {C4,  S}, {r,   S}, {A3,   Q}, {r,    Q}, {F3, E+S}, {r, E+S},
 					 {C4,  S}, {r,   S}, {A3,   H}, {r,    H}, {T,    Q} };

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
	printf("Song 1: Imperial March\n\r");
	printf("Song 2: Super Mario Bros Flagpole Fanfare\n\r");
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
	// play_note(261.63, 10000.00);
	for(;;) {
		// Get command from user
		fgets(line, 100, stdin);
		// Parse only the command for strcmp
		sscanf(line, "%s", command);
		if (!strcmp(command, "help")) {
			printHelp();
		} else if (!strcmp(command, "songs")) {
			songInfo();
		} else if (!strcmp(command, "rmw")) {
			sscanf(line, "%s %X", command, &address);
			readMem(address);
		} else if (!strcmp(command, "wmw")) {
			sscanf(line, "%s %X %u", command, &address, &data);
			writeMem(address, data);
		} else if (!strcmp(command, "dm")) {
			sscanf(line, "%s %X %u", command, &address, &length);
			dumpMem(address, length);
		} else if (!strcmp(command, "ps")) {
			sscanf(line, "%s %u", command, &songSelection);
			switch(songSelection) {
				case 1:
					printf("Playing Imperial March\n\r");
					play_song(&songIM[0]);
					break;
				case 2:
					printf("Playing Super Mario Bros Flagpole Fanfare\n\r");
					play_song(&songSMFF[0]);
					break;
				default:
					break;
			}
		} else {
			printf("Invalid input, type 'help' for instructions\n\r");
		}
	}
}
