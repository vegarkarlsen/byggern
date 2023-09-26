
#include "USART_driver.h"
#include <avr/io.h>
#include <stdio.h>

int USART0_transmit(unsigned char data) {
    // wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    // put data into buffer, sends the data
    UDR0 = data;

    return 0;
}

int USART0_receive(FILE *file) {
    // wait for data to be received
    while (!(UCSR0A & (1 << RXC0))) {
    }
    return UDRE0;
}

void USART0_init(unsigned int ubrr) {
    // set baud rate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // set frame format: 8bit, 2 stop bit
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

    /* fdevopen(USART0_transmit, USART0_receive); */
}
