
/*
 * usart_lib.c
 *
 * Created: 9/19/2024 6:05:29 PM
 *  Author: Ikramuzzaman Muntasir
 */ 

#include <avr/io.h>
#include "usart_lib.h"


void USART_Init(unsigned int ubrr) {
	/* Set baud rate */
	UBRRH = (unsigned char) (ubrr>>8);
	UBRRL = (unsigned char) ubrr; /* Enable receiver and transmitter */
	UCSRB = (1<<RXEN) | (1<<TXEN); /* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL) | (3<<UCSZ0);
}

void USART_Transmit( unsigned char data ) {
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) ) ;
	/* Put data into buffer, sends the data */
	UDR = data;
}

void USART_Transmit_String(char *str) {
	for(uint16_t i = 0; *(str + i) != '\0'; i++) {
		USART_Transmit((char)*(str + i));
	}
}