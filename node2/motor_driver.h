#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"


// Initialize motorbox confgiureations
void moto_init();

// move the motor
void move_motor();

// Read value from the motorbox's encoder
uint16_t read_encoder();




