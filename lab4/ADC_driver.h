
#pragma once
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ADC_START_ADRESS 0x1400

// define ADC channels
typedef enum Channel {
    JOYSTICK_CHANNEL_X,
    JOYSTICK_CHANNEL_y,
    SLIDER_CHANNEL_LEFT,
    SLIDER_CHANNEL_RIGHT
} Channel;

extern uint8_t joy_zeropoint_x;
extern uint8_t joy_zeropoint_y;

// initialize adc clock
void ADC_timer_init();

// read raw channel data from adc
/* channels */
/*     0 : joystick x  */
/*     1 : joystick Y */
/*     2 : slider left */
/*     3 : slider right */
uint8_t read_channel(Channel channel);

// read joystck channel and convert to persentage
int8_t read_joystick_channel_transformed(Channel channel, int8_t offset);

// calibrate zero point
// calibration_cycle: number of cycels to tage average from
void calibrate_zero_point(uint8_t calibration_cycle);
uint8_t get_zeropoint_from_channel(Channel channel);
