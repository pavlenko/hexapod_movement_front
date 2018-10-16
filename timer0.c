/*
 * timer0.c
 *
 * Created: 06.04.2014 11:01:46
 *  Author: MASTER
 */ 
#include "timer0.h"

#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <stdint.h>

#include "es.h"

volatile uint8_t timer0 = 0;

void initialize_timer0(void)
{
	TCCR0A = 0x00;
	TCCR0B = _BV(CS02)|_BV(CS00);
	
	TCNT0 = 0x00;
	
	TIFR0  = _BV(TOV0);
	TIMSK0 = _BV(TOIE0);
}

ISR(TIMER0_OVF_vect)
{
	timer0++;
	
	if (timer0 == 3) {
		es_trigger(TIMER0_OVF_EVENT);
		timer0 = 0;
	}
}