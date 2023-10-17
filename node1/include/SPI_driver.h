#pragma once
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

/* init SPI, set ATMega as master */
void SPI_master_init();

// transmitt data
void SPI_transmit(uint8_t byte);

// read data
uint8_t SPI_revice();

// revice and transmitt in the same time (less resources?)
/* uint8_t SPI_transmit_and_recice(uint8_t byte); */

// set chicp select
void SPI_set_ss();

// reset chicp select
void SPI_reset_ss();
