/*
 * servomotor.h
 *
 * Created: 20.03.2014 9:24:16
 *  Author: user
 */ 


#ifndef SERVOMOTOR_H_
#define SERVOMOTOR_H_

#include "main.h"
#include <stdint.h>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	
	#define SM_USE_TIMER5
	#define SM_USE_TIMER1
	#define SM_USE_TIMER3
	#define SM_USE_TIMER4
	
	typedef enum {
		sm_timer5,
		sm_timer1,
		sm_timer3,
		sm_timer4,
		sm_timers_count
	} sm_timer_t;

#elif defined(__AVR_ATmega32U4__)
	
	#define SM_USE_TIMER1
	
	typedef enum {
		sm_timer1,
		sm_timers_count
	} sm_timer_t;

#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	
	#define SM_USE_TIMER3
	#define SM_USE_TIMER1
	
	typedef enum {
		sm_timer3,
		sm_timer1,
		sm_timers_count
	} sm_timer_t;

#elif defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
	
	#define SM_USE_TIMER3
	#define SM_USE_TIMER1
	
	typedef enum {
		sm_timer3,
		sm_timer1,
		sm_timers_count
	} sm_timer_t;

#else

	#define SM_USE_TIMER1
	
	typedef enum {
		sm_timer1,
		sm_timers_count
	} sm_timer_t;

#endif

typedef struct {
	uint8_t pin :6;//:3
	uint8_t is_active :1;//:1
	uint8_t is_reverse :1;//:1
	uint16_t ticks;
	int16_t middle;
	//uint16_t target;
	//uint16_t speed;
	volatile uint8_t *port;
} sm_t;

// constant definitions
#define SM_MIN_PULSE 544
#define SM_MAX_PULSE 2400
#define SM_DEFAULT_PULSE 1500
#define SM_PER_TIMER 10
#define SM_INVALID 0xFF
#define SM_TRIM_DURATION 2
#define SM_US_PER_ANGLE 10.311111111111111111111111111111
#define SM_REFRESH_US 20000U
#define SM_STEPS_COMPLETE 1

// macro
#if defined(F_CPU) && F_CPU == 16000000UL

	#define SM_US_TO_TICKS(_val_) (_val_ * 2)
	#define SM_TICKS_TO_US(_val_) (_val_ / 2)

#elif defined(F_CPU) && F_CPU == 8000000UL

	#define SM_US_TO_TICKS(_val_) (_val_)
	#define SM_TICKS_TO_US(_val_) (_val_)

#elif defined(F_CPU)

	#error "driver can work only with F_CPU 8MHz or 16 MHz"

#else

	#error "F_CPU not defined"

#endif /* !F_CPU */

#define SM_US_TO_ANGLE(_val_) ( (int16_t) ((float) (_val_ - SM_MIN_PULSE) / SM_US_PER_ANGLE) )
#define SM_ANGLE_TO_US(_val_) ( (int16_t) ((float) _val_ * SM_US_PER_ANGLE) + SM_MIN_PULSE )

#define SM_GET_TIMER(_index_) ( (sm_timer_t) (_index_ / SM_PER_TIMER) )
#define SM_MAX (sm_timers_count * SM_PER_TIMER)

#define SM_ABS(_val_) ( (_val_) > 0 ? (_val_) : -(_val_) )

#define SM_INDEX(_timer_, _channel_) ( (_timer_ * SM_PER_TIMER) + _channel_ )
#define SM_MS_TO_STEPS(_val_) ( _val_ / 20U )
#define SM_STEPS_TO_MS(_val_) ( _val_ * 20U )

// prototypes
uint8_t servomotor_attach(volatile uint8_t *port, uint8_t pin, int16_t middle, uint8_t is_reverse);

void servomotor_detach(uint8_t index);

void servomotor_set_angle(uint8_t index, int16_t value);

void servomotor_set_us(uint8_t index, int16_t value);

uint8_t servomotor_is_attached(uint8_t index);

void servomotor_set_middle(uint8_t index, int16_t value);

#endif /* SERVOMOTOR_H_ */