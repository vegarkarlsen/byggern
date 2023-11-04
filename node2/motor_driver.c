
#include "motor_driver.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
#include <stdint.h>


/*
 * MJ2 motorbox to arduino connections:
 * shild - motorbox - arduino
 * D00 - !RST - PC1
 * D01 - Encoder reset - PC2
 * D02 - Encoder select - PC3
 * D03 - Motor enable - PC4
 * D04 - Motor direction - PC5
 * -------------------------------
 * MJ1 motorbox to arduino connections.:
 * motorbox - arduino
 * DIR - PD10
 * EN - PD9
 * SEL - PD2
 * !RST - PD1
 * !OE - PD0
*/

void moto_init(){
    // enable MJ2 pins
    PIOC->PIO_PER |= PIO_PC1 | PIO_PA2 | PIO_PA3 | PIO_PA4 | PIO_PA5;
    // enable MJ1 pins
    PIOD->PIO_PER |= PIO_PD0 | PIO_PD1 | PIO_PD2 | PIO_PD9 | PIO_PD10;

    
}



uint16_t read_encoder(){
// 1: Set !OE low, to sample and hold the encoder value

// 2: Set SEL low to output high byte
// 3: Wait approx. 20 microseconds for output to settle
// 4: Read MJ2 to get high byte
// 5: Set SEL high to output low byte
// 6: Wait approx. 20 microseconds
// 7: Read MJ2 to get low byte
// 8: Set !OE to high

}

