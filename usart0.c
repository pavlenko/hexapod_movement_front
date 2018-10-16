/**
 * USART stdio compatible driver (unbuffered)
 */
#include "usart0.h"

#include "main.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Initialize interface
 */
void initialize_usart0(void)
{
	/* Set baud rate */
	UBRR0H = 0;
	UBRR0L = 103;
	
	/* Set frame format: 8-bit data, 1-bit stop */
	UCSR0C = _BV(UCSZ01)|_BV(UCSZ00);
	
	/* Enable receiver and transmitter, receive interrupt */
	UCSR0B = _BV(RXEN0)|_BV(TXEN0);
}

/**
 * Put character to UART
 */
void usart0_putc(int8_t byte)
{
	/* Wait for data register empty */
	while ( !(UCSR0A & _BV(UDRE0)) );
	
	/* Put data */
	UDR0 = byte;
}

/**
 * Get character from UART
 */
int8_t usart0_getc(void)
{
	/* Wait while receive complete */
	while ( !(UCSR0A & _BV(RXC0)) );
	
	/* Read data */
	return UDR0;
}

void usart0_puts(char *str)
{
	while (*str) {
		usart0_putc(*str++);
	}
}
