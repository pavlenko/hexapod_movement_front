/*
 * servomotor.c
 *
 * Created: 20.03.2014 9:24:31
 *  Author: user
 */
#include "led.h"

#include "es.h"
#include "servomotor.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/atomic.h>
#include <stdint.h>

static sm_t sm[SM_MAX];
static volatile int8_t sm_channel[sm_timers_count];

uint8_t sm_count = 0;

static inline void _servomotor_isr(sm_timer_t timer, volatile uint16_t *TCNTn, volatile uint16_t* OCRnA)
{
	LED_ON();
	uint8_t index;
	
	if (sm_channel[timer] < 0) {		
		*TCNTn = 0;
	} else {		
		index = SM_INDEX(timer, sm_channel[timer]);
		
		if (index < sm_count && sm[index].is_active) {
			*(sm[index].port) &= ~_BV(sm[index].pin);
		}		
	}
	
	sm_channel[timer]++;
	index = SM_INDEX(timer, sm_channel[timer]);
	
	if (index < sm_count && sm_channel[timer] < SM_PER_TIMER) {
		*OCRnA = *TCNTn + sm[index].ticks;
		
		if (sm[index].is_active) {
			*(sm[index].port) |= _BV(sm[index].pin);
		}
	} else {
		if (*TCNTn + 4 < SM_US_TO_TICKS(SM_REFRESH_US)) {
			*OCRnA = (uint16_t) SM_US_TO_TICKS(SM_REFRESH_US);
		} else {
			*OCRnA = *TCNTn + 4;
		}
		
		//es_trigger(SM_STEPS_COMPLETE);
		sm_channel[timer] = -1;
	}
}

#ifdef SM_USE_TIMER1

	ISR(TIMER1_COMPA_vect, ISR_BLOCK)
	{
		_servomotor_isr(sm_timer1, &TCNT1, &OCR1A);
	}
	
#endif

#ifdef SM_USE_TIMER3

	ISR(TIMER3_COMPA_vect, ISR_BLOCK)
	{
		_servomotor_isr(sm_timer3, &TCNT3, &OCR3A);
	}

#endif

#ifdef SM_USE_TIMER4

	ISR(TIMER4_COMPA_vect, ISR_BLOCK)
	{
		_servomotor_isr(sm_timer4, &TCNT4, &OCR4A);
	}

#endif

#ifdef SM_USE_TIMER5

	ISR(TIMER5_COMPA_vect, ISR_BLOCK)
	{
		_servomotor_isr(sm_timer5, &TCNT5, &OCR5A);
	}

#endif

void _servomotor_start_isr(sm_timer_t timer)
{
	//TODO check timers config, it trigger invalid interrupts
	#ifdef SM_USE_TIMER1
		if (timer == sm_timer1) {
			TCCR1A = 0;
			TCCR1B = _BV(CS11);
			TCNT1  = 0;
			#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
				TIFR  |= _BV(OCF1A);
				TIMSK |= _BV(OCIE1A);
			#else
				TIFR1  |= _BV(OCF1A);
				TIMSK1 |= _BV(OCIE1A);
			#endif
		}
	#endif

	#ifdef SM_USE_TIMER3
		if (timer == sm_timer3) {
			TCCR3A = 0;
			TCCR3B = _BV(CS31);
			TCNT3  = 0;
			#if defined(__AVR_ATmega128__)
				TIFR   |= _BV(OCF3A);
				ETIMSK |= _BV(OCIE3A);
			#else
				TIFR3  = _BV(OCF3A);
				TIMSK3 = _BV(OCIE3A);
			#endif
		}
	#endif

	#ifdef SM_USE_TIMER4
		if (timer == sm_timer4) {
			TCCR4A = 0;
			TCCR4B = _BV(CS41);
			TCNT4  = 0;
			TIFR4  = _BV(OCF4A);
			TIMSK4 = _BV(OCIE4A);
		}
	#endif

	#ifdef SM_USE_TIMER5
		if (timer == sm_timer5) {
			TCCR5A = 0;
			TCCR5B = _BV(CS51);
			TCNT5  = 0;
			TIFR5  = _BV(OCF5A);
			TIMSK5 = _BV(OCIE5A);
		}
	#endif
}

void _servomotor_stop_isr(sm_timer_t timer)
{
	#ifdef SM_USE_TIMER1
		if (timer == sm_timer1) {
			#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
				TIMSK &= ~_BV(OCIE1A);
			#else
				TIMSK1 &= ~_BV(OCIE1A);
			#endif
		}
	#endif

	#ifdef SM_USE_TIMER3
		if (timer == sm_timer3) {
			#if defined(__AVR_ATmega128__)
				ETIMSK &= ~_BV(OCIE3A);
			#else
				TIMSK3 &= ~_BV(OCIE3A);
			#endif
		}
	#endif

	#ifdef SM_USE_TIMER4
		if (timer == sm_timer4) {
			TIMSK4 &= ~_BV(OCIE4A);
		}
	#endif

	#ifdef SM_USE_TIMER5
		if (timer == sm_timer5) {
			TIMSK5 &= ~_BV(OCIE5A);
		}
	#endif
}

uint8_t _servomotor_timer_is_active(sm_timer_t timer)
{
	uint8_t i;

	for (i = 0; i < SM_PER_TIMER; i++) {
		if (sm[SM_INDEX(timer, sm_channel[timer])].is_active) {
			return 1;
		}
	}

	return 0;
}

uint8_t servomotor_attach(volatile uint8_t *port, uint8_t pin, int16_t middle, uint8_t is_reverse)
{
	LED_ON();
	uint8_t index;
	
	if (sm_count < SM_MAX) {
		index = sm_count++;
		sm[index].ticks = SM_US_TO_TICKS(SM_DEFAULT_PULSE);
		
		*(port - 1) |= _BV(pin);
		
		sm[index].pin  = pin;
		sm[index].port = port;
		
		sm[index].middle     = middle;
		sm[index].is_reverse = is_reverse;
		
		sm_timer_t timer = SM_GET_TIMER(index);
		if (!_servomotor_timer_is_active(timer)) {
			_servomotor_start_isr(timer);
		}
		
		sm[index].is_active = 1;
	} else {
		index = SM_INVALID;
	}
	
	return index;
}

void servomotor_detach(uint8_t index)
{
	LED_ON();
	if (index < sm_count) {
		sm[index].is_active = 0;
		
		sm_timer_t timer = SM_GET_TIMER(index);
		if (_servomotor_timer_is_active(timer)) {
			_servomotor_stop_isr(timer);
		}
	}
}

void servomotor_set_angle(uint8_t index, int16_t value)
{
	LED_ON();
	if (value < SM_MIN_PULSE) {
		if (value < 0) {
			value = 0;
		} else if (value > 180) {
			value = 180;
		}
		
		value = SM_ANGLE_TO_US(value);
	}
	
	servomotor_set_us(index, value);
}

void servomotor_set_us(uint8_t index, int16_t value)
{
	LED_ON();
	
	sm_t *sm_ptr = &sm[index];
	
	if (value < SM_MIN_PULSE) {
		value = SM_MIN_PULSE;
	} else if (value > SM_MAX_PULSE) {
		value = SM_MAX_PULSE;
	}
	
	value = value + (*sm_ptr).middle;
	
	if ((*sm_ptr).is_reverse) {
		value = (SM_MAX_PULSE - value) + SM_MIN_PULSE;
	}
	
	value = value - SM_TRIM_DURATION;
	value = SM_US_TO_TICKS(value);
	
	uint8_t old_sreg = SREG;
	cli();
	(*sm_ptr).ticks = value;
	SREG = old_sreg;
}

uint8_t servomotor_is_attached(uint8_t index)
{
	LED_ON();
	return sm[index].is_active;
}

void servomotor_set_middle(uint8_t index, int16_t value)
{
	LED_ON();
	sm[index].middle = value;
}