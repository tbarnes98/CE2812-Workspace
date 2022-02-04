/**
 * @file piezoSpeaker.c
 * @author Trevor Barnes
 * @brief Provides functionality for initializing, playing notes, and playing songs on the piezo speaker.
 * @version 0.1
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include "piezoSpeaker.h"
#include "delay.h"

volatile RCC* const rcc = 0x40023800;
volatile GPIO* const gpiob = 0x40020400;
volatile TIM* const tim3 = 0x40000400;
volatile SYSCFG* const syscfg = 0x40013800;
volatile EXTI* const exti4 = 0x40013C00;


void piezo_init(){
	// GPIOB/Timer3 enable in RCC
	(*rcc).AHB1ENR |= (1<<GPIOBEN);
	(*rcc).APB1ENR |= (1<<TIM3_EN);

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
}

void play_note(Note noteToPlay) {
	(*tim3).PSC = 15;
	// Pitch divisor to scale with timer
	(*tim3).ARR = (pitchDivisor)/(noteToPlay.noteFrequency);

	// Volume (Smaller dividend = louder sound)
	unsigned int freq = (noteToPlay.noteFrequency/10);

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

void play_note_br(Note noteToplay) {

}

void TIM3_IRQHandler(void) {
	(*tim3).PSC = 15;
	// Pitch divisor to scale with timer
	(*tim3).ARR = (pitchDivisor)/(noteToPlay.noteFrequency);
	// Volume (Smaller dividend = louder sound)
	unsigned int freq = (noteToPlay.noteFrequency/10);

	// Clear CCR
	(*tim3).CCR1 = ((*tim3).CCR1&~(0xFFFF));
	(*tim3).CCR1 = freq;

	// Set EGR
	(*tim3).EGR |= EGR_UG;

	// Playing note
	// Enables timer
	(*tim3).CR1 |= 1;
	// Delay for duration of note
	// This is gonna have to change
	delay_1ms(noteToPlay.noteDuration);
	// Disables timer
	(*tim3).CR1 &= ~1;
}

void play_song_br(Note *songToPlay) {
	// PB4 Connected to EXTI4
	(*syscfg).EXTICR2 &= ~(0xF);
	(*syscfg).EXTICR2 &= ~(0x1);

}
