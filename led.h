/*
 * led.h
 *
 * Created: 25.03.2014 21:48:25
 *  Author: Jenya
 */ 


#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define LED_ON() (PORTB |= _BV(PB7))
#define LED_OFF() (PORTB &= ~_BV(PB7))

void initialize_led(void);

#endif /* LED_H_ */