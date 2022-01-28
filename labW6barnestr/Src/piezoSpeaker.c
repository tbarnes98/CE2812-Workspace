/**
 * @file piezoSpeaker.c
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
#include "piezoSpeaker.h"
#include "delay.h"


void piezo_init(){

	//enable GPIOB and Timer 3 RCC
	*RCC_AHB1ENR |= (1<<GPIOBEN);
	*RCC_APB1ENR |= (1<<TIM3_EN);

	//set GPIO B to alternate function (0b10<<9)
	//clears the two bits and then set it
	*GPIOB_MODER = (*GPIOB_MODER&~(0b11<<8)) | (PB4_AF_V<<8);

	//set alternate function low register to TIM3
	*GPIOB_AFRL |= (1<<AFRL_TIM3_CH1_EN);

	//Configure capture/compare mode register configuration
	//to enable preload and set to pwm
	*TIM3_CCMR1 |= OC1M_PWM2;
	*TIM3_CCMR1 |= (1<<OC1PE);

	//Configure CCER to enable timer 3 as output capture
	*TIM3_CCER |= CCER_CC1E;

	//Configure control register to enable preload
	*TIM3_CR1 |= (1<<CR_ARPE_EN);

}

play_note(Note noteToPlay) {

//void play_note(double playFrequency, double playDuration) {

	double freq = noteToPlay.noteFrequency;
	double dur = noteToPlay.noteDuration;

	*TIM3_PSC = 15;
	//Divisor controls pitch
	*TIM3_ARR = mil/freq;

	//Loudness (Smaller dividend = louder sound)
	freq = freq/10;

	//clear ccr1
	*TIM3_CCR1 = (*TIM3_CCR1&~(0xFFFF));
	*TIM3_CCR1 = freq;

	//set EGR (accept only a byte of info so steps)
	*TIM3_EGR |= EGR_UG;

	//~~~Plays the notes
	//Enables enable bit control register
	*TIM3_CR1 |= 1;
	//delay that leaves the speaker on for desired amount of time
	delay_1ms(dur);
	//Disables enable bit
	*TIM3_CR1 &= ~1;
}


void play_song(Note *songToPlay){
	int i = 0;
	double freq = songToPlay[i].noteFrequency;
	while(freq != T) {
		play_note(songToPlay[i]);
		i++;
	}
}
