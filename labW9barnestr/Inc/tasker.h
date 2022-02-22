/**
 * @file tasker.h
 * @author Trevor Barnes
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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

// GPIO
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

// Will have to include timer for task switching interrupt

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

typedef enum{PAUSED, ACTIVE} task_state;

#define SCB_ICSR 0xE000D04
#define PENDSVSET 28

extern int num_tasks;
extern int current_task;
extern int next_task;
extern Task tasks[];


typedef struct {
    uint32_t* stack_pointer;
    uint32_t ticks_starting;
    uint32_t ticks_remaining;
    task_state state;
} Task;

void tasker_tick();

void init_tasker(uint32_t total_tasks, uint32_t main_ticks);

void init_task(uint32_t task_num, uint32_t stacksize, 
               void(*entry_point)(void), uint32_t ticks);

void PendSV_Handler(void __attribute__((naked)));