
#include "SPI_driver.h"
#include <avr/io.h>
#include <stdint.h>

void SPI_set_ss(){
    PORTB |= (1 << DD_SS);
}

void SPI_reset_ss(){
    PORTB &= ~(1 << DD_SS);
}

void SPI_master_init() { 
    // Set MOSI, SCK and SS
    DDR_SPI = (1 << DD_MISO) | (1 << DD_SCK) | (1 << DD_SS);
    // Enalbe SPI, Master, set clock rate = f_obc/16, enable interupt 
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPIE);

    SPI_set_ss();
}

void SPI_transmit(uint8_t byte){
    // start transmittion
    SPDR = byte;
    // wait for transmittion
    while (!(SPSR & (1 << SPIF))){

    }
}

uint8_t SPI_revice(){
    // set dummy value? 
    SPDR = 0xFF;
    // wait for reading
    while (!(SPSR & (1<<SPIF))){

    }
    return SPDR;
}

uint8_t SPI_transmit_and_recice(uint8_t byte){
    SPDR = byte;
    while (!(SPSR & (1 << SPIF))){

    }
    return SPDR;
}

