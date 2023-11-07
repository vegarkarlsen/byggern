#include "solenoid.h"


void solenoid_init(){    
    // enable PMC for PIOD
    PMC->PMC_PCER0 |= PMC_PCER0_PID14;
    // enable SEOLENOID pin (PD3)
    PIOD->PIO_PER |= SOLENOID_PIN;
    // set pins as ouput
    PIOD->PIO_OER |= SOLENOID_PIN;
    // Enable internal pullup
    PIOD->PIO_PUER |= SOLENOID_PIN;
    // Disable internal pullup
    // PIOD->PIO_PUDR |= SOLENOID_PIN;
    // output write enable
    PIOD->PIO_OWER |= SOLENOID_PIN;
    // start with on
    solenoid_on();

}
    // PIOD->PIO_PUDR |= PIO_PD2 | PIO_PD9 | PIO_PD10;

void solenoid_on(){
    PIOD->PIO_ODSR |= SOLENOID_PIN;
}

void solenoid_off(){
    PIOD->PIO_ODSR &= ~SOLENOID_PIN;
}
