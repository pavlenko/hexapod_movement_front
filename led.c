/*
 * led.c
 *
 * Created: 25.03.2014 21:48:39
 *  Author: Jenya
 */ 
#include "led.h"

#include <avr/io.h>

void initialize_led(void)
{
	DDRB  |= _BV(PB7);
	PORTB &= ~_BV(PB7);
}