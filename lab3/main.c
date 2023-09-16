
#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define F_CPU 16000000


#include <avr/io.h>
#include "USART_driver.h"
#include <stdio.h>
#include <util/delay.h>
#include "SRAM_test.h"


void test_hardcode(){
    volatile uint8_t *sramAdress = (uint8_t *) 0x1802;
    volatile uint8_t *adcAdress = (uint8_t *) 0x1405;

    printf("starting sram \r\n");
    for (int i = 0; i < 255; i++){
        *sramAdress = i;
        printf("sram: %4d \r\n", *sramAdress);
    }
    _delay_ms(1000);

    printf("starting ADC \r\n");
    for (int i = 0; i < 255; i++){
        *adcAdress = i;
        printf("adc: %4d \r\n", *adcAdress);
    }
}





void read_ch_1(){
    volatile char *adc = (char *) 0x1400;
    adc[0] = 0x00;
    _delay_ms(50);
    uint8_t value = adc[0];
    printf("adc: %0X\r\n", value);
     
}

void eneable_adc_timer(){
    DDRD |= (1 << PD4);
    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12) | (1 << CS10);
    OCR0 = 1;
}


int main(void)
{
    USART0_init( MYUBRR );
    SRAM_init();
    
    eneable_adc_timer();

    // SRAM_test();
    
    // test_hardcode();
	while (1){

        read_ch_1();
	}
}
