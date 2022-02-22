/**
 * @file piezoSpeaker.c
 * @author Trevor Barnes
 * @brief Provides functionality for initializing, playing notes, and playing 
 * songs on the piezo speaker.
 * Added interrupt functionality for background playing
 * @version 0.2
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include "piezoSpeaker.h"
#include "delay.h"

volatile RCC* const rcc = (RCC*) 0x40023800;
volatile GPIO* const gpiob = (GPIO*) 0x40020400;
// PWM Timer
volatile TIM* const tim3 = (TIM*) 0x40000400;
// Interrupt Timer for Duration
volatile TIM* const tim4 = (TIM*) 0x40000800;
volatile SYSCFG* const syscfg = (SYSCFG*) 0x40013800;
volatile EXTI* const exti4 = (EXTI*) 0x40013C00;

volatile Note* currentSong;
volatile int currentNoteIndex;

typedef enum {PLAY, STOP}songStatus;

// Imperial March - Star Wars
Note songIM[138] = {
	{A4, Q}, {A4, Q}, {A4, Q}, {F4, S*3}, {C5, S}, 
	{A4,  Q}, {F4, S*3}, {C5, S}, {A4, H},
	{E5, Q}, {E5, Q}, {E5, Q}, {F5, S*3}, {C5, S}, 
	{Ab4, Q}, {F4, S*3}, {C5, S}, {A4, H},
	{A5, Q}, {A4, S*3}, {A4, S}, {A5, Q}, {Ab5, S*3}, {G5, S},
	{Gb5, S}, {F5, S}, {Gb5, E}, {r, E}, 
	{Bb4, E}, {Eb5, Q}, {D5, S*3}, {Db5, S},
	{C5, S}, {B4, S}, {C5, E}, {r, E}, 
	{F4, E}, {Ab4, Q}, {F4, S*3}, {A4, S},
	{C5, Q}, {A4, S*3}, {C5, S}, {E5, H},
	{A5, Q}, {A4, S*3}, {A4, S}, {A5, Q}, {Ab5, S*3}, {G5, S},
	{Gb5, S}, {F5, S}, {Gb5, E}, {r, E}, 
	{Bb4, E}, {Eb5, Q}, {D5, S*3}, {Db5, S},
	{C5, S}, {B4, S}, {C5, E}, {r, E}, 
	{F4, E}, {Ab4, Q}, {F4, S*3}, {C5, S},
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
	{r , E}, {F5, E}, {E5, E}, {F5, E}, {E5, E}, {C5, E}, {A4, E}, 
	{G4, E+(Q*5)}, {r,  E},
	{C5, E}, {B4, E}, {D5, E}, {A5, E}, {G5, E+W+E}, {r,  E},
	{A5, E}, {B5, E}, {A5, E}, {G5, E}, {F5, E}, {E5, E}, {D5, E}, {E5, Q}, 
	{C5, E}, {G4, E+(Q*3)}, {r,  E},
	{C5, E}, {B4, E}, {C5, E}, {D5, E}, {E5, E+Q}, {G5, Q}, {C5, Q}, {E5, Q},
	{F5, E}, {E5, E}, {F5, E}, {D5, E*2}, {C5, E}, {B4, E}, {C5, E+W},
	{END} 
};

int volumeDivisor = 10;

void piezo_init(){
	// GPIOB/Timer3/Timer4 enable in RCC
	(*rcc).AHB1ENR |= (1<<GPIOBEN);
	(*rcc).APB1ENR |= (1<<TIM3_EN);
	(*rcc).APB1ENR |= (1<<TIM4_EN);

	// Set to "alternate function" mode
	(*gpiob).MODER = ((*gpiob).MODER&~(0b11<<8)) | (PB4_AF_V<<8);
	// Set AF to low
	(*gpiob).AFRL |= (1<<AFRL_TIM3_CH1_EN);

	// Set to output capture
	(*tim3).CCMR1 |= OC1M_PWM2;
	(*tim3).CCMR1 |= (1<<OC1PE);
	(*tim3).CCER |= CCER_CC1E;
	
	// Enable Preload
	(*tim3).CR1 |= (1<<CR_ARPE_EN);

	// Configure Timer 4 for Interrupt
	(*tim4).DIER |= (1<<DIER_TIE);
}

void play_note(Note noteToPlay) {
	(*tim3).PSC = 15;
	// Pitch divisor to scale with timer
	(*tim3).ARR = (pitchDivisor)/(noteToPlay.noteFrequency);

	// Volume (Smaller dividend = louder sound)
	unsigned int freq = (noteToPlay.noteFrequency/volumeDivisor);

	// Clear CCR
	(*tim3).CCR1 = ((*tim3).CCR1&~(0xFFFF));
	(*tim3).CCR1 = freq;

	// Set EGR
	(*tim3).EGR |= EGR_UG;

	// Playing note
	// Enables timer
	(*tim3).CR1 |= 1;
	// Delay for duration of note
	delay_1ms(noteToPlay.noteDuration);
	// Disables timer
	(*tim3).CR1 &= ~1;
}

void play_song(Note *songToPlay){
	int i = 0;
	// Iterate through song and play each note till "END" value
	while(songToPlay[i].noteFrequency != END) {
		play_note(songToPlay[i]);
		i++;
	}
}

void TIM4_IRQHandler(void) {
	//temp
	tim4->SR = 0;
	if(PLAY) {
		(*tim3).PSC = 15;
		// Pitch divisor to scale with timer
		//current note frequency global variable
		(*tim3).ARR = 
			(pitchDivisor)/(currentSong[currentNoteIndex].noteFrequency);

		// Volume (Smaller dividend = louder sound)
		//current note frequency global variable
		unsigned int freq = 
			(currentSong[currentNoteIndex].noteFrequency/volumeDivisor);

		// Clear CCR
		(*tim3).CCR1 = ((*tim3).CCR1&~(0xFFFF));
		(*tim3).CCR1 = freq;

		// Set EGR
		(*tim3).EGR |= EGR_UG;
		// Set timer 4 count to duration of note
		(*tim4).CNT = (currentSong[currentNoteIndex].noteDuration);
		(*tim4).EGR |= EGR_UG;
		// Start timer 4
		(*tim4).CR1 |= 1;
		// Playing note
		// Enables timer
		(*tim3).CR1 |= 1;
		// Delay for duration of note

		//delay_1ms(noteToPlay.noteDuration);
		// Instead of delay, have timer count to loaded value
		// Then trigger interrupt when count value has been reached

		// Load timer with value of note duration
		//currentNote = currentSong[currentNoteIndex];
	} else {
		while(currentSong[currentNoteIndex].noteFrequency != END) {

		}
		//currentNote = currentSong
	}
	currentNoteIndex++;
}

// Might be unneccessary
void play_song_br(Note *songToPlay) {
	// Set current song global variable
	currentSong = songToPlay;
	// Configure Interrupt
	// PB4 Connected to EXTI4
	(*syscfg).EXTICR2 &= ~(0xF);
	(*syscfg).EXTICR2 &= ~(0x1);

}
