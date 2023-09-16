
#pragma once
#include <avr/io.h>
#include <stdlib.h>


void ADC_timer_init();


uint8_t read_channel(uint8_t channel);