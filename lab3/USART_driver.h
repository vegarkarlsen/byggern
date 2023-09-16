
#pragma once
#include <avr/io.h>
#include <stdio.h>




int USART0_transmit(unsigned char data);

int USART0_receive(FILE *file);

void USART0_init(unsigned int ubrr);