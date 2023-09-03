
#define FOSC 4915200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define F_CPU 16000000


#include <avr/io.h>
#include "USART_driver.h"
#include <stdio.h>


int main(void)
{
	/* Replace with your application code */

	USART0_init ( MYUBRR );
	
	while (1)
	{
		printf("halla");
		
	}
}