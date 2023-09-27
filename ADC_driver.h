
#pragma once
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

#define ADC_START_ADRESS 0x1400

void ADC_timer_init();


uint8_t read_channel(uint8_t channel);


enum x {NONE=0, UP=1, DOWN=2, LEFT=3, RIGHT=4};
struct Position {
    uint8_t x;
    uint8_t y;
};



struct Position calculate_direction_joy(int8_t percentage_x, int8_t percentage_y);

int8_t analog_to_persentage_joy(uint8_t input, int8_t offset);