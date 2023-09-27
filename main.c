
#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define F_CPU 16000000


#include <avr/io.h>
#include "USART_driver.h"
#include <stdio.h>
#include <util/delay.h>
#include "SRAM_test.h"

void test_latch_output(){


    DDRE = 0b01;
    DDRE = 0b11111111;

    PORTE = 0b01;
    printf("latch on, setting PORTA to 0b11111111\n");
    PORTA = 0b11111111;
    _delay_ms(1000);

    PORTE = 0b00;
    printf("latch off, setting PORTA to 0b00000000\n");
    PORTA = 0b00000000;
    _delay_ms(1000);

}

void test_latch_value(){
    uint8_t retrived_value;

    for (uint8_t val = 0; val < 256; val += val){
        if (val == 0) val = 1;
        PORTA = val;
        retrived_value = DDRA;
        _delay_ms(1500);        
        printf("setting PORTA to %02X ", val);
        _delay_ms(1500);
        printf("reading value from DDRA: %02X\r\n", retrived_value);        
    }
}


int main(void)
{
    // setup PORTA and PORTE as output
    // DDRA = 0b11111111;
    // DDRE = 0b00000111;

    // setup UART configs
	USART0_init ( MYUBRR );
    SRAM_init();
	
    // test_latch_value();

    // SRAM_test();
    // volatile char *ext_ram = (char *) 0x1400; // Start address
    // uint8_t var;  
	while (1)
	{
        // USART0_transmit('b');
        // printf("hei");
        // var = ext_ram[1];
        // printf(" %02X ", var); 
	}
}