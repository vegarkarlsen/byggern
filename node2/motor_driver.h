#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"


// Initialize motorbox confgiureations
void motor_init();

// move the motor
void move_motor(uint16_t value, uint8_t direction);

// write to dac
void dac_write(uint16_t bin_voltage);

//Reset encoder
void reset_encoder(); 

// Read value from the motorbox's encoder
uint16_t read_encoder();






