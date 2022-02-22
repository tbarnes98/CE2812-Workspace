/**
 * @file piezoSpeaker.h
 * @author Trevor Barnes
 * @brief Header file for the piezo speaker's C file that provides all of the 
 * necessary timer, RCC, and GPIOB values for working with the respective 
 * components. Note frequency values and durations are also defined. 
 * Registers addresses converted to structs in this new version
 * @version 0.2
 * @date 2022-01-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PIEZOSPEAKER_H_
#define PIEZOSPEAKER_H_

#include <inttypes.h>
// RCC
typedef struct {
	uint32_t CR;
	uint32_t PLL_CFGR;
	uint32_t CFGR;
	uint32_t CIR;
	uint32_t AHB1RSTR;
	uint32_t AHB2RSTR;
	uint32_t AHB3RSTR;
	uint32_t _RESERVED_1_;
	uint32_t APB1RSTR;
	uint32_t APB2RSTR;
	uint32_t _RESERVED_2_;
	uint32_t _RESERVED_3_;
	uint32_t AHB1ENR;
	uint32_t AHB2ENR;
	uint32_t AHB3ENR;
	uint32_t _RESERVED_4_;
	uint32_t APB1ENR;
	uint32_t APB2ENR;
	uint32_t _RESERVED_5_;
	uint32_t _RESERVED_6_;
} RCC;

// GPIOB
#define GPIOBEN 1
#define PB4_AF_V 0b10
#define PB4_AF_P 9

typedef struct {
	uint32_t MODER;
	uint32_t OTYPER;
	uint32_t OSPEEDER;
	uint32_t PUPDR;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t LCKR;
	uint32_t AFRL;
	uint32_t AFRH;
} GPIO;


typedef struct {
	uint32_t CR1;     // Control Register 1
	uint32_t CR2;     // Control Register 2
	uint32_t SMCR;    // Slave Mode Control Register 
	uint32_t DIER;    // DMA/Interrupt Enable Register
	uint32_t SR;      // Status Register
	uint32_t EGR;     // Event Generation Register
	uint32_t CCMR1;   // Capture/Compare Mode Register 1
	uint32_t CCMR2;   // Capture/Compare Mode Register 2
	uint32_t CCER;    // Capture/Compare Enable Register 
	uint32_t CNT;     // Counter
	uint32_t PSC;     // Prescaler
	uint32_t ARR;     // Auto-Reload Register
	uint32_t _RESERVED_1_; 
	uint32_t CCR1;    // Capture/Compare Register 1
	uint32_t CCR2;    // Capture/Compare Register 2
	uint32_t CCR3;    // Capture/Compare Register 3
	uint32_t CCR4;    // Capture/Compare Register 4
	uint32_t _RESERVED_2_;
	uint32_t DCR;     // DMA Control Register
	uint32_t DMAR;    // DMA Address for Full Transfer
	uint32_t TIM2_OR; // TIM2 Option Register
	uint32_t TIM5_OR; // TIM5 Option Register
} TIM;

// Timers 3 & 4
#define TIM3_EN 1
#define TIM4_EN 2
#define AFRL_TIM3_CH1_EN 17
#define OC1PE 3
#define OC1M_PWM2 0b1110000
#define CCER_CC1E 1
#define EGR_UG 1
#define CR_ARPE_EN 7
#define CR_CEN 1
#define DIER_TIE 6


#define pitchDivisor 1000000
// SYSCFG
typedef struct {
	uint32_t MEMRMP;
	uint32_t PMC;
	uint32_t EXTICR1;
	uint32_t EXTICR2;
	uint32_t EXTICR3;
	uint32_t EXTICR4;
	uint32_t CMPCR;
	uint32_t CFGR;
} SYSCFG;

// Interrupt 
#define NVIC_ISER1 (volatile uint32_t*) 0xE000E104
typedef struct {
	uint32_t IMR;
	uint32_t EMR;
	uint32_t RTSR;
	uint32_t FTSR;
	uint32_t SWIER;
	uint32_t PR;
} EXTI;

typedef struct {
	unsigned int noteFrequency;
	unsigned int noteDuration;
} Note;

extern Note songIM[];
extern Note songMT[];
extern Note songFG[];

// Initializes the piezo speaker to be used with timer 3. 
// Also initializes timer 4 for note duration interrupt
void piezo_init();

// Plays a given note at a certain frequency for a certain duration
void play_note(Note noteToPlay);

// Iterates through an array of note structs 
// and ends at the termination value "END"
void play_song(Note *songToPlay);

void play_song_br(Note *songToPlay);

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
