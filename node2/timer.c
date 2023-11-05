
#include "timer.h"
#include "sam.h"
#include "sam3x8e.h"
#include <stdint.h>

volatile uint32_t ticks = 0;

// void sysTick_inti(){
//     // set clock source to MCK/8
//     SysTick->CTRL &= ~SysTick_CLKSOURCE;
//     // Set countflag to 1 if timer has counted to 0 since last read
//     SysTick->CTRL &= ~SysTick_TICKINT;
//     // load max value in reload
//     SysTick->LOAD = 0x00FFFFFF;
//     // Enable systick
//     SysTick->CTRL |= SysTick_ENABLE;
// }

// uint32_t get_systick(){
//     return SysTick->VAL;
// }

void SysTick_Handler(){
    ticks++;
}

uint32_t getTicks(){
    return ticks;
}

uint32_t getTimeMs(){
    return getTicks()/10;
}

void _ms_delay(uint32_t delay){
    uint32_t time_now = getTimeMs();
    while (getTimeMs() < (time_now + delay));
}

