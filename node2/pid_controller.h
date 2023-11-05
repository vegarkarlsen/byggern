#pragma once
#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"
#include "motor_driver.h"

typedef struct PID_const {
    uint16_t K_P;
    uint16_t K_I;
    uint16_t K_D;
    uint16_t T;
}PID_t;


void update_integral(uint16_t pos, uint16_t current_deviation);

uint16_t get_integral();

// referance needs to go between 0 and encoder max value
int16_t PI(PID_t *pid, uint16_t referance);
