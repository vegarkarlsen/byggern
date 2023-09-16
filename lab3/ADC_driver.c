#include "ADC_driver.h"
#include <avr/io.h>

void ADC_init(){
    DDRD = (1 << PD4);
    

}