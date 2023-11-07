
#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"

// arduino PD3 = shild pin27
#define SOLENOID_PIN PIO_PD6

void solenoid_init();

void solenoid_on();

void solenoid_off();


