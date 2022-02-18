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

void SysTick_Init();

void tasker_tick();

void init_tasker(uint32_t total_tasks, uint32_t main_ticks);

void init_task(uint32_t task_num, uint32_t stacksize, 
               void(*entry_point)(void), uint32_t ticks);

void PendSV_Handler(void __attribute__((naked)));