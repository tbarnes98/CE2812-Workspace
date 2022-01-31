/**
 * @file piezoSpeaker.c
 * @author Trevor Barnes
 * @brief Provides funtionality for initializing, playing notes, and playing songs on the piezo speaker.
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


void piezo_init(){
	// GPIOB/Timer3 enable in RCC
	*RCC_AHB1ENR |= (1<<GPIOBEN);
	*RCC_APB1ENR |= (1<<TIM3_EN);

	// Set to "alternate function" mode
	*GPIOB_MODER = (*GPIOB_MODER&~(0b11<<8)) | (PB4_AF_V<<8);
	// Set AF to low
	*GPIOB_AFRL |= (1<<AFRL_TIM3_CH1_EN);

	// Set to output capture
	*TIM3_CCMR1 |= OC1M_PWM2;
	*TIM3_CCMR1 |= (1<<OC1PE);
	*TIM3_CCER |= CCER_CC1E;
	
	// Enable Preload
	*TIM3_CR1 |= (1<<CR_ARPE_EN);
}

void play_note(Note noteToPlay) {
	*TIM3_PSC = 15;
	// Pitch divisor to scale with timer
	*TIM3_ARR = (pitchDivisor)/(noteToPlay.noteFrequency);

	// Volume (Smaller dividend = louder sound)
	unsigned int freq = (noteToPlay.noteFrequency/10);

	// Clear CCR
	*TIM3_CCR1 = (*TIM3_CCR1&~(0xFFFF));
	*TIM3_CCR1 = freq;

	// Set EGR
	*TIM3_EGR |= EGR_UG;

	// Playing note
	// Enables timer
	*TIM3_CR1 |= 1;
	// Delay for duration of note
	delay_1ms(noteToPlay.noteDuration);
	// Disables timer
	*TIM3_CR1 &= ~1;
}

void play_song(Note *songToPlay){
	int i = 0;
	// Iterate through song and play each note till "END" value
	while(songToPlay[i].noteFrequency != END) {
		play_note(songToPlay[i]);
		i++;
	}
}
