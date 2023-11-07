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
    float K_P;
    float K_I;
    float K_D;
    float T;

}PID_t;


void update_integral(int16_t current_deviation);

int16_t get_integral();

// referance needs to go between 0 and encoder max value
int16_t PI(PID_t pid, uint16_t referance, uint16_t pos);


int16_t P(PID_t pid, uint16_t referance, uint16_t pos);
