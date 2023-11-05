
#include "motor_driver.h"
#include "component/component_dacc.h"
#include "component/component_pio.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
#include "timer.h"
#include <stdint.h>
#include <stdio.h>

#define DEBUG 0
#ifdef DEBUG
#define MOTOR_PRINT(M) do { printf("[motor_driver]: "); printf(M); } while (0)
#else
#define MOTOR_PRINT(M)
#endif /* ifdef DEBUG */

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
    PMC->PMC_PCER0 |= PMC_PCER0_PID14;
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

    // /* MJ2: (All input)*/
    // // enable MJ2 pins
    // PIOC->PIO_PER |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5;
    // // set pins as input (Disable output)
    // PIOC->PIO_ODR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5;
    // // Disable internal pullup
    // PIOC->PIO_PUDR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5;    
    /* MJ2: (All input)*/
    PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    // enable MJ2 pins
    PIOC->PIO_PER |= MOTOR_PINS;
    // set pins as input (Disable output)
    PIOC->PIO_ODR |= MOTOR_PINS;
    // Disable internal pullup
    PIOC->PIO_PUDR |= MOTOR_PINS;

    // // Start Encoder reading, and reset encoder val
    PIOD->PIO_ODSR |= PIO_PD0 | PIO_PD1;

    /* DAC: */
    // Enable pmc clock for DAC 
    PMC->PMC_PCER1 |= PMC_PCER1_PID38;
    // Dissable write protection
    DACC->DACC_WPMR &= ~DACC_WPMR_WPEN;
    // configure mode (free running mode, and using full word, select channel 1)
    DACC->DACC_MR |= DACC_MR_TRGEN_DIS | DACC_MR_WORD_HALF | DACC_MR_USER_SEL_CHANNEL1;
    // Eanble DACC channel 1 = DAC1?
    DACC->DACC_CHER |= DACC_CHER_CH1;
}


uint16_t read_encoder(){
// 1: Set !OE low, to sample and hold the encoder value
    PIOD->PIO_ODSR &= ~(MOTOR_NOT_OE);
    // PIOD->PIO_CODR |= PIO_PD0;
// 2: Set SEL low to output high byte
    PIOD->PIO_ODSR &= ~(MOTOR_SEL);
    // PIOD->PIO_CODR |= PIO_PD2;
// 3: Wait approx. 20 microseconds for output to settle
    // uint8_t start_time = wait_ms;
    _ms_delay(20);
// 4: Read MJ2 to get high byte
//since PD1-PD8 is in use and not PD0, push PD0 out
//the size of PDSR function is limited to 8 bit by uint8_t
    if (DEBUG){
        printf("[motor_driver.encoder_read]: PIOC->PDSR (high_byte) = 0x%x\n\r", PIOC->PIO_PDSR);
    }
    uint8_t high_byte = (PIOC->PIO_PDSR >> 1);
    if (DEBUG) {
        printf("[motor_driver.encoder_read]: high_byte = %d\n\r", high_byte);
    }
// 5: Set SEL high to output low byte
    PIOD->PIO_ODSR |= MOTOR_SEL;
// 6: Wait approx. 20 microseconds
    _ms_delay(20);
// 7: Read MJ2 to get low byte
//since PD1-PD8 is in use and not PD0, push PD0 out
//the size of PDSR function is limited to 8 bit by uint8_t
    uint8_t low_byte = (uint8_t)(PIOC->PIO_PDSR >> 1);
    if (DEBUG) {
        printf("[motor_driver.encoder_read]: PIOC->PDSR (low_byte) = 0x%x\n\r", PIOC->PIO_PDSR);
        printf("[motor_driver.encoder_read]: low_byte = %d\n\r", low_byte);
    }
// 8: Set !OE to high
    PIOD->PIO_ODSR |= MOTOR_NOT_OE;
//combines low and high byte by switching size of data and pushing 8 zeroes behind to 
//make space for low values
    uint16_t full_byte = (uint16_t)((high_byte << 8)|(low_byte));
    if (DEBUG) {
        printf("[motor_driver.encoder_read]: full_byte = %d\n\r", full_byte);
    }
    return full_byte;
}

void reset_encoder(){
    //encoder resets when MJ1 pin !RST is pulled low
    //Set pin !RST low
    PIOD->PIO_ODSR &= ~PIO_PD1;
    //wait 100ms
    _ms_delay(100);
    //Set pin !RST high
    PIOD->PIO_ODSR |= PIO_PD1;

}

void dac_write(uint16_t bin_voltage){
    // write value to DAC 12bit(max: 0xFFF)
    DACC->DACC_CDR = bin_voltage; 
}

void dac_write_percentage(uint8_t percent){
    
    // cant be bigger than 100%
    if (percent > 100){
        percent = 100;
    }

    uint16_t max_bin_votalge = 4095;
    uint32_t bin_voltage = (percent * max_bin_votalge)/100;
    if (DEBUG){
        printf("[motor_driver]: Writing to dac: %d\n\r", bin_voltage);
    }
    dac_write(bin_voltage);

}
void stop_motor(){
    PIOD->PIO_ODSR &= ~PIO_PD9;

}
void enable_motor(){
    // setting EN high
    PIOD->PIO_ODSR |= PIO_PD9;
}

void move_motor(uint16_t value, uint8_t direction){
    // write to motor, needs to enable motor before writing.
    switch (direction) {
        case 0:
            // DIR = 0
            PIOD->PIO_ODSR &= ~PIO_PD10;
            break;
        case 1: 
            // DIR = 1 
            PIOD->PIO_ODSR |= PIO_PD10;
            break;
        default:
            stop_motor();
            return;
    }
    dac_write_percentage(value);
}

uint16_t calibrate_encoder(){
    move_motor(50, 1);
    enable_motor();
    _ms_delay(500);
    // stop_motor();
    reset_encoder();
    move_motor(50, 0);
    _ms_delay(600);
    stop_motor();
    return read_encoder();
}
