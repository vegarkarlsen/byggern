


#include <avr/io.h>
#include "USART_driver.h"
#include <stdio.h>
#include <util/delay.h>
#include "SRAM_test.h"
#include "ADC_driver.h"

#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define F_CPU 16000000

uint8_t joy_x = -1;
uint8_t joy_y = -1;

int main(void)
{
    USART0_init( MYUBRR );
    SRAM_init(); // this is genreal for databus
    SRAM_test();
    ADC_timer_init();
     
	while (1){
        joy_x = read_channel(0);
        joy_y = read_channel(1);

        printf("joysick (x, y): (%2d, %2d) \r\n",joy_x, joy_y);
	}
}
