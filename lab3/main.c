
#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define F_CPU 16000000


#include <avr/io.h>
#include "USART_driver.h"
#include <stdio.h>
#include <util/delay.h>
#include "SRAM_test.h"


int main(void)
{
    USART0_init( MYUBRR );
    SRAM_init();

    SRAM_test();

	while (1){

	}
}
