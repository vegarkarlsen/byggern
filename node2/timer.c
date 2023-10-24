
#include "timer.h"
#include "sam.h"
#include "sam3x8e.h"
#include <stdint.h>

volatile uint32_t ticks = 0;

void SysTick_Handler(){
    ticks++;
}

uint32_t getTimeMs(){
    return ticks;
}

