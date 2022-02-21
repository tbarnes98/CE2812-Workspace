/**
 * @file tasker.c
 * @author Trevor Barnes
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <inttypes.h>
#include "tasker.h"

void SysTick_Init() {

}

void init_task(uint32_t task_num, uint32_t stacksize, void(*entry_point)(void), uint32_t ticks) {
    tasks[task_num].stack_pointer = (uint32_t*)malloc(stacksize*sizeof(uint32_t));
    tasks[task_num].stack_pointer += stacksize;
    *(--tasks[task_num].stack_pointer) = 0x01000000; // PSR
    *(--tasks[task_num].stack_pointer) = ((uint32_t)entry_point);
    *(--tasks[task_num].stack_pointer) = 0xFFFFFFFF;
    *(--tasks[task_num].stack_pointer) = 0; // R12
    *(--tasks[task_num].stack_pointer) = 0; // R3
    *(--tasks[task_num].stack_pointer) = 0; // R2
    *(--tasks[task_num].stack_pointer) = 0; // R1
    *(--tasks[task_num].stack_pointer) = 0; // R0
    *(--tasks[task_num].stack_pointer) = 0xFFFFFFF9; // ISR LR
    *(--tasks[task_num].stack_pointer) = 0; // R11
    *(--tasks[task_num].stack_pointer) = 0; // R10
    *(--tasks[task_num].stack_pointer) = 0; // R9
    *(--tasks[task_num].stack_pointer) = 0; // R8
    *(--tasks[task_num].stack_pointer) = 0; // R7
    *(--tasks[task_num].stack_pointer) = 0; // R6
    *(--tasks[task_num].stack_pointer) = 0; // R5
    *(--tasks[task_num].stack_pointer) = 0; // R4
    tasks[task_num].state = ACTIVE;
    tasks[task_num].ticks_starting = ticks;
    tasks[task_num].ticks_remaining = 0;
}

void init_tasker(uint32_t total_tasks, uint32_t main_ticks) {
    num_tasks = total_tasks;
    *tasks = calloc(total_tasks, sizeof(Task));
}


void tasker_tick() {
    tasks[current_task].ticks_remaining--;

    if(tasks[current_task].ticks_remaining == 0) {
        int i = 1;
        while(tasks[(next_task = (current_task + i)%num_tasks)].state != ACTIVE) {
            i++;
        }
        tasks[next_task].ticks_remaining = tasks[next_task].ticks_starting;
        //*scb_icsr |= 1<<PENDSVSET;
    }
}

void SysTick_Handler(void) {
    
}

void PendSV_Handler(void) {
    register uint32_t* stack_pointer asm("r13");
    asm volatile("push {r4-r11,lr}");
    tasks[current_task].stack_pointer = stack_pointer;
    current_task = next_task;
    stack_pointer = tasks[current_task].stack_pointer;
    asm volatile("pop {r4-r11,lr}\n\t"
                 "bx lr");
}
