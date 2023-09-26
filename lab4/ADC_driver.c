#pragma once
#include "ADC_driver.h"
#include "USART_driver.h"
#include "util.h"
#include <avr/common.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

// calibration variables
uint8_t joy_zeropoint_x = 0;
uint8_t joy_zeropoint_y = 0;

void ADC_timer_init() {
    DDRD |= (1 << PD5);
    TCCR1A = (1 << COM1A0);              // Toggle-mode
    TCCR1B = (1 << WGM12) | (1 << CS10); // CTSmode, No scaler
    OCR0 = 1;
}

uint8_t read_channel(Channel channel) {

    volatile char *adc = (char *)ADC_START_ADRESS;

    adc[0] = 0x00; // initialize reading
    _delay_ms(50); // delay need to be more than t_CONV (see adc datasheet)

    // store reading in array
    uint8_t readings[4];
    for (int i = 0; i < 4; i++) {
        readings[i] = adc[i];
    }
    return readings[channel];
}

void calibrate_zero_point(uint8_t calibration_cycle) {
    printf("Starting joystick calibration\r\n");
    int n = 10;
    long sum_joy_x = 0;
    long sum_joy_y = 0;
    for (uint8_t i = 0; i < n; i++) {
        sum_joy_x += read_channel(JOYSTICK_CHANNEL_X);
        sum_joy_y += read_channel(JOYSTICK_CHANNEL_y);
        _delay_ms(1000);
        printf(".");
    }
    joy_zeropoint_x = sum_joy_x / n;
    joy_zeropoint_y = sum_joy_y / n;
    printf("\r\nCalibration commplete. x : %2d, y : %2d \r\n", joy_zeropoint_x,
           joy_zeropoint_y);
}

uint8_t get_zeropoint_from_channel(Channel channel) {
    uint8_t calibrated_value;
    switch (channel) {
    case 0:
        calibrated_value = joy_zeropoint_x;
        break;
    case 1:
        calibrated_value = joy_zeropoint_y;
        break;
    default:
        calibrated_value = 0;
        break;
    }
    return calibrated_value;
}

int8_t read_joystick_channel_transformed(Channel channel, int8_t offset) {

    uint8_t channel_data = read_channel(channel);
    uint8_t zeropoint;
    int8_t output;
    uint8_t min_raw_value = 3;

    zeropoint = get_zeropoint_from_channel(channel);

    // map up
    if (channel_data > zeropoint + offset) {
        output = map(channel_data, zeropoint + offset, 255, 100, 0);
    }
    // map down
    else if (channel_data < zeropoint - offset) {
        output = map(channel_data, zeropoint - offset, min_raw_value, -100, 0);
    } else {
        output = 0;
    }

    return output;
}
