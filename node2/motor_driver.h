#pragma once

#include "pio/pio_sam3x8e.h"
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "sam.h"
/*  * MJ1 motorbox to arduino connections.:
 * motorbox - arduino
 * DIR - PD10
 * EN - PD9
 * SEL - PD2
 * !RST - PD1
 * !OE - PD0
*/
#define MOTOR_NOT_OE PIO_PD0
#define MOTOR_NOT_RST PIO_PD1
#define MOTOR_SEL PIO_PD2
#define MOTOR_EN PIO_PD9
#define MOTOR_DIR PIO_PD10



// PC1 - PC8
/* const static uint32_t MJ2_pins = 0b111111110; */
#define MOTOR_PINS 0x1FE

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

// reset encoder on zero point and returns max value. 
uint16_t calibrate_encoder();




