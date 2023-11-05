#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"

// 1 - 8
const static uint32_t MJ2_pins = 0b111111110;

// Initialize motorbox confgiureations
void motor_init();

// move the motor 
void move_motor(uint16_t value, uint8_t direction);
// stop motor
void stop_motor();
// enable motor
void enable_motor();

// write to dac
void dac_write(uint16_t bin_voltage);
// Write to dac prosentage value (0-100)
void dac_write_percentage(uint8_t percent);

//Reset encoder
void reset_encoder(); 

// (returns uint16_t) Read enocder value from motorbox.
uint16_t read_encoder();






