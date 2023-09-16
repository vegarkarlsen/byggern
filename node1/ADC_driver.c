#include "ADC_driver.h"
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

void ADC_timer(){
    DDRD |= (1 << PD5); 
	TCCR1A=(1 << COM1A0);			 	 // Toggle-mode
	TCCR1B= (1 << WGM12)|(1 << CS10);	 // CTSmode, No scaler
	OCR0 = 1;		
}

uint8_t read_channel_1(){
    volatile char *adc = (char * ) 0x1400;
    adc[0] = 0x00;
    _delay_ms(50); // right time
    
    return adc[0];
    
}
