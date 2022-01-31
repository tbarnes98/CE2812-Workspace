/**
 * @file piezoSpeaker.h
 * @author Trevor Barnes
 * @brief Header file for the piezo speaker's C file that provides all of the necessary timer, RCC,
 * and GPIOB values for working with the respective components. Note frequency values and durations
 * are also defined.
 * @version 0.1
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PIEZOSPEAKER_H_
#define PIEZOSPEAKER_H_

// RCC
#define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
#define RCC_APB1ENR (volatile uint32_t*) 0x40023840

// GPIOB
#define GPIOB_MODER (volatile uint32_t*) 0x40020400
#define GPIOBEN 1
#define GPIOB_AFRL (volatile uint32_t*) 0x40020420
#define AFRL_TIM3_CH1_EN 17

// Timer 3
#define TIM3_EN 1
#define TIM3_CCMR1 (volatile uint32_t*) 0x40000418
#define OC1PE 3
#define OC1M_PWM2 0b1110000
#define TIM3_CCER (volatile uint32_t*) 0x40000420
#define CCER_CC1E 1
#define TIM3_EGR (volatile uint32_t*) 0x40000414
#define EGR_UG 1
#define TIM3_PSC (volatile uint32_t*) 0x40000428
#define TIM3_ARR (volatile uint32_t*) 0x4000042C
#define TIM3_CCR1 (volatile uint32_t*) 0x40000434
#define TIM3_CR1 (volatile uint32_t*) 0x40000400
#define CR_ARPE_EN 7
#define CR_CEN 1

#define PB4_AF_V 0b10
#define PB4_AF_P 9

#define pitchDivisor 1000000

typedef struct{
	unsigned int noteFrequency;
	unsigned int noteDuration;
} Note;

// Initializes the piezo speaker to be used with timer 3
void piezo_init();

// Plays a given note at a certain frequency for a certain duration
void play_note(Note noteToPlay);

// Iterates through an array of note structs and ends at the termination value "END"
void play_song(Note *songToPlay);

// Frequency Value Signifying a rest
#define r	1

// Note Frequency Symbols (Rounded to nearest integer value)
#define	C0  16
#define	Db0	17
#define	D0	18
#define	Eb0	19
#define	E0	21
#define	F0	22
#define	Gb0	23
#define	G0	25
#define	Ab0	26
#define	A0	28
#define	Bb0	29
#define	B0	31
#define	C1	33
#define	Db1	35
#define	D1	37
#define	Eb1	39
#define	E1	41
#define	F1	44
#define	Gb1	46
#define	G1	49
#define	Ab1	52
#define	A1	55
#define	Bb1	58
#define	B1	62
#define	C2	65
#define	Db2	69
#define	D2	73
#define	Eb2	78
#define	E2	82
#define	F2	87
#define	Gb2	93
#define	G2	98
#define	Ab2	104
#define	A2	110
#define	Bb2	117
#define	B2	123
#define	C3	131
#define	Db3	139
#define	D3	147
#define	Eb3	156
#define	E3	165
#define	F3	175
#define	Gb3	185
#define	G3	196
#define	Ab3	208
#define	A3	220
#define	Bb3	233
#define	B3	247
#define	C4	262
#define	Db4	277
#define	D4	294
#define	Eb4	311
#define	E4	330
#define	F4	349
#define	Gb4	370
#define	G4	392
#define	Ab4	415
#define	A4	440
#define	Bb4	466
#define	B4	494
#define	C5	523
#define	Db5	554
#define	D5	587
#define	Eb5	622
#define	E5	659
#define	F5	698
#define	Gb5	740
#define	G5	784
#define	Ab5	831
#define A5	880
#define	Bb5	932
#define	B5	988
#define	C6	1047
#define	Db6	1109
#define	D6	1175
#define	Eb6	1245
#define	E6	1319
#define	F6	1397
#define	Gb6	1480
#define	G6	1568
#define	Ab6	1661
#define	A6	1760
#define	Bb6	1865
#define	B6	1976
#define	C7	2093
#define	Db7	2217
#define	D7	2349
#define	Eb7	2489
#define	E7	2637
#define	F7	2794
#define	Gb7	2960
#define	G7	3136
#define	Ab7	3322
#define A7	3520
#define	Bb7	3730
#define	B7	3951
#define	C8	4186
#define	Db8	4435
#define	D8	4699
#define	Eb8	4978

// Value that indicates the end of a song
#define END 0

// Tempo value set at 120 beats per min
#define BPM 120
// Quarter Note
#define Q 60000/BPM
// Half Note
#define H 2*Q
// Whole Note
#define W 4*Q
// Eighth Note
#define E Q/2
// Sixteenth Note
#define S Q/4


#endif
