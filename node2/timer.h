#pragma once
#include "sam.h"
#include "sam3x8e.h"
#include <stdint.h>

/* SysTick->CTRL */
#define SysTick_ENABLE (1u << 0)
#define SysTick_TICKINT (1u << 1)
#define SysTick_CLKSOURCE (1u << 2)
#define SysTick_COUNTFLAG (1u << 16)

/* uint32_t get_systick(); */
/* void sysTick_inti(); */

// get Ms 
uint32_t getTimeMs();

// Ticks with MCK/8 Hz
uint32_t getTicks();

void SysTick_Handler();

void _ms_delay(uint32_t delay);

// Pid interput
/* void TC0_Handler(); */
