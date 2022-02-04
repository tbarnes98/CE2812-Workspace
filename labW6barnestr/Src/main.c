/**
 * @file main.c
 * @author Trevor Barnes
 * @brief Main Driver for the Week 5 "Play a Tune" Lab. This program uses a similar menu style to the previous
 * lab. The user is given a new option to play 3 different hard coded songs in arrays of "Note" structs,
 * provided by the piezoSpeaker.h file.
 * Experience: As a former music student I enjoyed working on this lab very much, to the point that I became 
 * sort of a perfectionist with its implementation. Getting the timer configured and working with the piezo speaker
 * was certainly the most difficult and time consuming part. Once I had that figured out though figuring
 * out the best note implementation and songs to play was very enjoyable.
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

// Imperial March - Star Wars
Note songIM[138] = { 
					{A4, Q}, {A4, Q}, {A4, Q}, {F4, S*3}, {C5, S}, {A4,  Q}, {F4, S*3}, {C5, S}, {A4, H},
					{E5, Q}, {E5, Q}, {E5, Q}, {F5, S*3}, {C5, S}, {Ab4, Q}, {F4, S*3}, {C5, S}, {A4, H},
					{A5, Q}, {A4, S*3}, {A4, S}, {A5, Q}, {Ab5, S*3}, {G5, S},
					{Gb5, S}, {F5, S}, {Gb5, E}, {r, E}, {Bb4, E}, {Eb5, Q}, {D5, S*3}, {Db5, S},
					{C5, S}, {B4, S}, {C5, E}, {r, E}, {F4, E}, {Ab4, Q}, {F4, S*3}, {A4, S},
					{C5, Q}, {A4, S*3}, {C5, S}, {E5, H},
					{A5, Q}, {A4, S*3}, {A4, S}, {A5, Q}, {Ab5, S*3}, {G5, S},
					{Gb5, S}, {F5, S}, {Gb5, E}, {r, E}, {Bb4, E}, {Eb5, Q}, {D5, S*3}, {Db5, S},
					{C5, S}, {B4, S}, {C5, E}, {r, E}, {F4, E}, {Ab4, Q}, {F4, S*3}, {C5, S},
					{A4, Q}, {F4, S*3}, {C5, S}, {A4, H},
					{END}
				   };

// Metropolis Theme - Ratchet & Clank
Note songMT[33] = {
					{B5,  E}, {G5, E}, {E5, E}, {G5, E}, {B5,  E}, {G5, E}, {E5, E}, {B5,  E},
					{Bb5, E}, {F5, E}, {D5, E}, {F5, E}, {Bb5, E}, {F5, E}, {D5, E}, {Bb5, E},
					{B5,  E}, {G5, E}, {E5, E}, {G5, E}, {B5,  E}, {G5, E}, {E5, E}, {B5,  E},
					{Bb5, E}, {F5, E}, {D5, E}, {F5, E}, {Bb5, E}, {F5, E}, {D5, E}, {Bb5, E},
					{END}
					};
					
// Flower Garden - Yoshi's Island
Note songFG[77] = {
					{E4, E}, {r, E}, {G4, E}, {r, S}, {G4, S}, {E4, E}, {C4, E}, {r, Q},
					{A3, E}, {r, E}, {C4, E}, {r, S}, {A3, S}, {D4, E}, {E4, E}, {r, Q},
					{E4, E}, {r, E}, {G4, E}, {r, S}, {G4, S}, {E4, E}, {C4, E}, {r, Q},
					{A3, E}, {r, E}, {C4, E}, {r, S}, {A3, S}, {E4, E}, {D4, E}, {r, Q},
					{G5, S}, {Gb5, S}, {G5, E+(Q*3)},
					{r , E}, {F5, E}, {E5, E}, {F5, E}, {E5, E}, {C5, E}, {A4, E}, {G4, E+(Q*5)}, {r,  E},
					{C5, E}, {B4, E}, {D5, E}, {A5, E}, {G5, E+H+Q}, {r,  E},
					{A5, E}, {B5, E}, {A5, E}, {G5, E}, {F5, E}, {E5, E}, {D5, E}, {E5, Q}, {C5, E}, {G4, E+(Q*3)}, {r,  E},
					{C5, E}, {B4, E}, {C5, E}, {D5, E}, {E5, E+Q}, {G5, Q}, {C5, Q}, {E5, Q},
					{F5, E}, {E5, E}, {F5, E}, {D5, E*2}, {C5, E}, {B4, E}, {C5, E+W},
					{END} 
					};

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
			sscanf(line, "%s %u %c", command, &songSelection, background);
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
