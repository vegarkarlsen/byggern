#pragma once
#include <avr/io.h>
#include <stdlib.h>

// enalbe databus pins and mask pins used for JTAG
dataBus_init();


// ----------------- 
// USART
// -----------------

void USART_init(unsigned int ubrr);

int USART_transmit(unsigned char data);

int USART_receive(FILE *file);



// ----------------- 
// SRAM
// -----------------

void adc_timer_init();

uint8_t adc_read()

// ----------------- 
// ADC
// -----------------

