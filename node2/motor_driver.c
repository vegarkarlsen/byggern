
#include "motor_driver.h"
#include "component/component_dacc.h"
#include "component/component_pio.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
#include "timer.h"
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

void motor_init(){
    /* MJ1: (all output)*/
    // enable MJ1 pins
    PIOD->PIO_PER |= PIO_PD0 | PIO_PD1 | PIO_PD2 | PIO_PD9 | PIO_PD10;
    // set pins as output
    PIOD->PIO_OER |= PIO_PD0 | PIO_PD1 | PIO_PD2 | PIO_PD9 | PIO_PD10;
    // Enable internal pullup for inverted pins:
    PIOD->PIO_PUER |= PIO_PD0 | PIO_PD1;
    // Disable internal pullup for normal pins
    PIOD->PIO_PUDR |= PIO_PD2 | PIO_PD9 | PIO_PD10;
    // Ouput write enable on all pins
    PIOD->PIO_OWER |= PIO_PD0 | PIO_PD1 | PIO_PD2 | PIO_PD9 | PIO_PD10;

    /* MJ2: (All input)*/
    // enable MJ2 pins
    PIOC->PIO_PER |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5;
    // set pins as input (Disable output)
    PIOC->PIO_ODR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5;
    // Disable internal pullup
    PIOC->PIO_PUDR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5;    

    /* DAC: */
    // Dissable write protection
    DACC->DACC_WPMR &= ~DACC_WPMR_WPEN;
    // configure mode (free running mode, and using full word)
    DACC->DACC_MR |= DACC_MR_TRGEN_DIS | DACC_MR_WORD_HALF;
    // Eanble DACC channel 1 = DAC1?
    DACC->DACC_CHER |= DACC_CHER_CH1;
}



uint16_t read_encoder(){
// 1: Set !OE low, to sample and hold the encoder value
    PIOD->PIO_ODSR &= ~(PIO_PD0); 
// 2: Set SEL low to output high byte
    PIOD->PIO_ODSR &= ~(PIO_PD2);
// 3: Wait approx. 20 microseconds for output to settle
    uint8_t wait_ms =  getTimeMs();
    uint8_t start_time = wait_ms;
    while(wait_ms<start_time+2){
        wait_ms = getTimeMs();
    }
// 4: Read MJ2 to get high byte
//since PD1-PD8 is in use and not PD0, push PD0 out
//the size of PDSR function is limited to 8 bit by uint8_t
    uint8_t high_byte = (uint8_t)(PIOC->PIO_PDSR >> 1);
// 5: Set SEL high to output low byte
    PIOD->PIO_ODSR |= PIO_PD2;
// 6: Wait approx. 20 microseconds
    uint8_t wait_ms_1 =  getTimeMs();
    uint8_t start_time_1 = wait_ms_1;
    while(wait_ms_1<start_time_1+2){
        wait_ms_1 = getTimeMs();
    }
// 7: Read MJ2 to get low byte
//since PD1-PD8 is in use and not PD0, push PD0 out
//the size of PDSR function is limited to 8 bit by uint8_t
    uint8_t low_byte = (uint8_t)(PIOC->PIO_PDSR >> 1);
// 8: Set !OE to high
    PIOD->PIO_ODSR |= PIO_PD0;
//combines low and high byte by switching size of data and pushing 8 zeroes behind to 
//make space for low values
    uint16_t full_byte = (uint16_t)((high_byte << 8)|(low_byte));
    return full_byte;
}

void dac_write(uint16_t bin_voltage){
    // write value to DAC 12bit
    DACC->DACC_CDR = bin_voltage; 
}


// TODO: finish this
void move_motor(uint16_t value, uint8_t direction){
    // setting EN low 
    PIOD->PIO_ODSR |= PIO_PD9;
    // choose direction 
    if (direction){
        PIOD->PIO_ODSR |= PIO_PD10;
    }
    else {
        PIOD->PIO_ODSR &= ~PIO_PD10;
    }

}
