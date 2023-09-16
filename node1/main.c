


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

uint8_t adc_value = 1;
int main(void)
{
    USART0_init( MYUBRR );
    SRAM_init(); // this is genreal for databus
    SRAM_test();
    ADC_timer();
     
	while (1){
        adc_value = read_channel_1();
        printf("ADC value: %2d \r\n", adc_value);
	}
}
