#include "ADC_driver.h"
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#define ADC_START_ADRESS 0x1400


void ADC_timer_init(){
    DDRD |= (1 << PD5); 
	TCCR1A=(1 << COM1A0);			 	 // Toggle-mode
	TCCR1B= (1 << WGM12)|(1 << CS10);	 // CTSmode, No scaler
	OCR0 = 1;		
}

uint8_t read_channel(uint8_t channel){

    // check if channel is valid
    if (channel > 3 | channel < 0){
        // TODO: mabye provoke an error
        return -1;
    }

    volatile char *adc = (char * ) ADC_START_ADRESS;
    
    adc[0] = 0x00; // initialize reading
    _delay_ms(50); // delay need to be more than t_CONV (see adc datasheet)
    
    // store reading in array
    uint8_t readings[4];
    for (int i = 0; i < 4; i++){
        readings[i] = adc[i];
    }

    return readings[channel]; 
}
