#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"

void pwm_init();

void set_pwn_duty_cycle(uint8_t prosentage);

void joy_test(int x);
