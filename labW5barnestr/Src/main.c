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

// Imperial March
Note t1n1  ={A3,  Q}, t1n2  ={r,   Q}, t1n3  ={A3,   Q}, t1n4  ={r,    Q}, t1n5  ={A3,   Q}, t1n6  ={r,   Q},
	 t1n7  ={F3,E+S}, t1n9  ={r, E+S}, t1n10 ={C4,   S}, t1n11 ={r,    S}, t1n12 ={A3,   Q}, t1n13 ={r,   Q},
	 t1n14 ={F3,E+S}, t1n15 ={r, E+S}, t1n16 ={C4,   S}, t1n17 ={r,    S}, t1n18 ={A3,   H}, t1n19 ={r,   H},
	 t1n20 ={E4,  Q}, t1n21 ={r,   Q}, t1n22 ={E4,   Q}, t1n23 ={r,    Q}, t1n24 ={E4,   Q}, t1n25 ={r,   Q},
	 t1n26 ={F4,E+S}, t1n27 ={r, E+S}, t1n28 ={C4,   S}, t1n29 ={r,    S}, t1n30 ={Ab3,  Q}, t1n31 ={r,   Q},
	 t1n32 ={F3,E+S}, t1n33 ={r, E+S}, t1n34 ={C4,   S}, t1n35 ={r,    S}, t1n36 ={A3,   H}, t1n37 ={r,   H},
	 t1n38 ={A4,  Q}, t1n39 ={r,   Q}, t1n40 ={A3, E+S}, t1n41 ={r,  E+S}, t1n42 ={A3,   S}, t1n43 ={r,   S},
	 t1n44 ={A4,  Q}, t1n45 ={r,   Q}, t1n46 ={Ab4,E+S}, t1n47 ={r,  E+S}, t1n48 ={G4,   S}, t1n49 ={r,   S},
	 t1n50 ={Gb4, Q}, t1n51 ={r,   S}, t1n52 ={E4,   S}, t1n53 ={r,    S}, t1n54 ={F4,   E}, t1n55 ={r,   E},
	 t1n56 ={r,   E}, t1n57 ={Bb3, E}, t1n58 ={r,    E}, t1n59 ={Eb4,  Q}, t1n60 ={r,    Q}, t1n61 ={D4,E+S},
	 t1n62 ={r, E+S}, t1n63 ={Db4, S}, t1n137={r,    H}, t1n64 ={C4,   S}, t1n65 ={r,    S}, t1n66 ={B3,  S},
	 t1n67 ={r,   S}, t1n68 ={C4,  E}, t1n69 ={r,    E}, t1n70 ={r,    E}, t1n71 ={F3,   E}, t1n72 ={r,   E},
	 t1n73 ={Ab3, Q}, t1n74 ={r,   Q}, t1n75 ={F3, E+S}, t1n76 ={r,  E+S}, t1n77 ={A3,   S}, t1n78 ={r,   S},
	 t1n79 ={C4,  Q}, t1n80 ={r,   Q}, t1n81 ={A3, E+S}, t1n82 ={r,  E+S}, t1n83 ={C4,   S}, t1n84 ={r,   S},
	 t1n85 ={E4,  H}, t1n86 ={r,   H}, t1n87 ={A4,   Q}, t1n88 ={r,    Q}, t1n89 ={A3, E+S}, t1n90 ={r, E+S},
	 t1n91 ={A3,  S}, t1n92 ={r,   S}, t1n93 ={A4,   Q}, t1n94 ={r,    S}, t1n95 ={Ab4,E+S}, t1n96 ={r, E+S},
	 t1n97 ={G4,  S}, t1n98 ={r,   S}, t1n99 ={Gb4,  S}, t1n100={r,    S}, t1n101={E4,   S}, t1n102={r,   S},
	 t1n103={F4,  E}, t1n104={r,   E}, t1n105={r,    E}, t1n106={Bb3,  E}, t1n107={r,    E}, t1n108={Eb4, Q},
	 t1n109={r,   Q}, t1n110={D4,E+S}, t1n111={r,  E+S}, t1n112={Db4,  S}, t1n113={r,    S}, t1n114={C4,  S},
	 t1n115={r,   S}, t1n116={B3,  S}, t1n117={r,    S}, t1n118={C4,   E}, t1n119={r,    E}, t1n120={r,   E},
	 t1n121={F3,  E}, t1n122={r,   E}, t1n123={Ab3,  Q}, t1n124={r,    Q}, t1n125={F3, E+S}, t1n126={r, E+S},
	 t1n127={C4,  S}, t1n128={r,   S}, t1n129={A3,   Q}, t1n130={r,    Q}, t1n131={F3, E+S}, t1n132={r, E+S},
	 t1n133={C4,  S}, t1n134={r,   S}, t1n135={A3,   H}, t1n136={r,    H};

// Super Mario "Flagpole Fanfare"
Note t2n1 ={G3, S/3}, t2n46={Ab3,S/3}, t2n2 ={A3, S/3}, t2n47={Bb3,S/3}, t2n3 ={B3,S/3},
	 t2n4 ={C4, S/3}, t2n48={Db4,S/3}, t2n5 ={D4, S/3}, t2n49={Eb4,S/3}, t2n6 ={E4,S/3},
	 t2n7 ={F4, S/3}, t2n50={Gb4,S/3}, t2n8 ={G4, S/3}, t2n51={Ab4,S/3}, t2n9 ={A4,S/3},
	 t2n52={Bb4,S/3}, t2n10={B4, S/3}, t2n11={C5, S/3}, t2n53={Db5,S/3}, t2n12={D5,S/3},
	 t2n54={Eb5,S/3}, t2n13={E5, S/3}, t2n14={F5, S/3}, t2n55={Gb5,S/3}, t2n15={G5,S/3},
	 t2n56={Ab5,S/3}, t2n16={A5, S/3}, t2n57={Bb5,S/3}, t2n17={B5, S/3}, t2n18={C6,S/3},
	 t2n58={Db6,S/3}, t2n19={D6, S/3}, t2n59={Eb6,S/3}, t2n20={E6, S/3}, t2n21={F6,S/3},
	 t2n60={Eb6,S/3}, t2n22={G6, S/3}, t2n23={r,  7*S}, t2n24={G3,   Q}, t2n25={C4,  Q},
	 t2n26={E4,   Q}, t2n27={G4,   Q}, t2n28={C5,   Q}, t2n29={E5,   Q}, t2n30={G5,  H},
	 t2n31={E5,   H}, t2n32={Ab3,  Q}, t2n33={C4,   Q}, t2n34={Eb4,  Q}, t2n35={Ab4, Q},
	 t2n36={C5,   Q}, t2n37={Eb5,  Q}, t2n38={Ab6,  H}, t2n39={Eb5,  H}, t2n40={Bb3, Q},
	 t2n41={D4,   Q}, t2n42={F4,   Q}, t2n43={Bb4,  Q}, t2n44={D5,   Q}, t2n45={F5,  Q},
	 t2n61={Bb5,  W}, t2n62={B5,   Q}, t2n63={B5,   Q}, t2n64={B5,   Q}, t2n65={C6,  W};

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
	// Inititialize Imperial March Note Array
	Note song1[137]={t1n1,  t1n2,  t1n3,  t1n4,  t1n5,  t1n6,  t1n7,  t1n9,  t1n10, t1n11, t1n12, t1n13, t1n14, t1n15,
				     t1n16, t1n17, t1n18, t1n19, t1n20, t1n21, t1n22, t1n23, t1n24, t1n25, t1n26, t1n27, t1n28, t1n29,
					 t1n30, t1n31, t1n32, t1n33, t1n34, t1n35, t1n36, t1n37, t1n38, t1n39, t1n40, t1n41, t1n42, t1n43,
					 t1n44, t1n45, t1n46, t1n47, t1n48, t1n49, t1n50, t1n51, t1n52, t1n53, t1n54, t1n55, t1n56, t1n57,
					 t1n58, t1n59, t1n60, t1n61, t1n62, t1n63, t1n137,t1n64, t1n65, t1n66, t1n67, t1n68, t1n69, t1n70,
					 t1n71, t1n72, t1n73, t1n74, t1n75, t1n76, t1n77, t1n78, t1n79, t1n80, t1n81, t1n82, t1n83, t1n84,
					 t1n85, t1n86, t1n87, t1n88, t1n89, t1n90, t1n91, t1n92, t1n93, t1n94, t1n95, t1n96, t1n97, t1n98,
					 t1n99, t1n100,t1n101,t1n102,t1n103,t1n104,t1n105,t1n106,t1n107,t1n108,t1n109,t1n110,t1n111,t1n112,
					 t1n113,t1n114,t1n115,t1n116,t1n117,t1n118,t1n119,t1n120,t1n121,t1n122,t1n123,t1n124,t1n125,t1n126,
					 t1n127,t1n128,t1n129,t1n130,t1n131,t1n132,t1n133,t1n134,t1n135,t1n136};
	int song1Size = sizeof(song1)/sizeof(song1[0]);
	// Initialize Super Mario Note Array
	Note song2[65]={t2n1, t2n46,t2n2, t2n47,t2n3, t2n4, t2n48,t2n5, t2n49,t2n6, t2n7, t2n50,t2n8, t2n51,t2n9, t2n52,
			  t2n10,t2n11,t2n53,t2n12,t2n54,t2n13,t2n14,t2n55,t2n15,t2n56,t2n16,t2n57,t2n17,t2n18,t2n58,t2n19,t2n59,
			  t2n20,t2n21,t2n60,t2n22,t2n23,t2n24,t2n25,t2n26,t2n27,t2n28,t2n29,t2n30,t2n31,t2n32,t2n33,t2n34,t2n35,
			  t2n36,t2n37,t2n38,t2n39,t2n40,t2n41,t2n42,t2n43,t2n44,t2n45,t2n61,t2n62,t2n63,t2n64,t2n65};
	int song2Size = sizeof(song2)/sizeof(song2[0]);
	char line[50];
	char command[10];
	int address;
	int data;
	static int length;
	static int songSelection;
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
					play_song(&song1[0], song1Size);
					break;
				case 2:
					printf("Playing Super Mario Bros Flagpole Fanfare\n\r");
					play_song(&song2[0], song2Size);
					break;
				default:
					break;
			}
		} else {
			printf("Invalid input, type 'help' for instructions\n\r");
		}
	}
}
