#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"

#define UPPER_THRESHOLD 3200
#define LOWER_THRESHOLD 400




void ir_init();

uint16_t read_ir_raw();

void detect_goal(uint8_t *goals);

