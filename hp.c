/*
 * hp.c
 *
 * Created: 26.03.2014 21:32:36
 *  Author: Jenya
 */
#include "hp.h"

#include "es.h"
#include "led.h"
#include "leg.h"
#include "servomotor.h"
#include "usart0.h"
#include "main.h"
#include "timer0.h"

#include <util/atomic.h>
//#include <util/delay.h>
#include <stdint.h>

#define HP_X 65.0
#define HP_H 40.0

volatile uint8_t position = 0;
volatile uint8_t completed = 0;

extern volatile leg_t right_legs[3];
extern volatile leg_t left_legs[3];

void _hp_steps_complete(void)
{
	completed++;
	
	if (completed >= 12) {//TODO fix invalid call period
		es_trigger(HP_STEPS_COMPLETE);
		completed = 0;
	}
}

void _hp(void)
{
	LED_ON();
	leg_point_t point_a;
	leg_angles_t angles_a;
	leg_point_t point_b;
	leg_angles_t angles_b;
	
	if (position == 0) {
		point_a.x = HP_X;
		point_a.y = 8.0;
		point_a.z = 0.0;
		
		point_b.x = HP_X;
		point_b.y = -8.0;
		point_b.z = HP_H;
		
		position++;
	} else if (position == 1) {
		point_a.x = HP_X;
		point_a.y = 16.0;
		point_a.z = 0.0;
		
		point_b.x = HP_X;
		point_b.y = -16.0;
		point_b.z = 0.0;
		
		position++;
	} else if (position == 2) {
		point_a.x = HP_X;
		point_a.y = 8.0;
		point_a.z = HP_H;
		
		point_b.x = HP_X;
		point_b.y = -8.0;
		point_b.z = 0.0;
	
		position++;
	} else if (position == 3) {
		point_a.x = HP_X;
		point_a.y = 0.0;
		point_a.z = HP_H;
		
		point_b.x = HP_X;
		point_b.y = 0.0;
		point_b.z = 0.0;

		position++;
	} else if (position == 4) {
		point_a.x = HP_X;
		point_a.y = -8.0;
		point_a.z = HP_H;
		
		point_b.x = HP_X;
		point_b.y = 8.0;
		point_b.z = 0.0;

		position++;
	} else if (position == 5) {
		point_a.x = HP_X;
		point_a.y = -16.0;
		point_a.z = 0.0;
		
		point_b.x = HP_X;
		point_b.y = 16.0;
		point_b.z = 0.0;

		position++;
	} else if (position == 6) {
		point_a.x = HP_X;
		point_a.y = -8.0;
		point_a.z = 0.0;
		
		point_b.x = HP_X;
		point_b.y = 8.0;
		point_b.z = HP_H;

		position++;
	} else if (position == 7) {
		point_a.x = HP_X;
		point_a.y = 0.0;
		point_a.z = 0.0;
		
		point_b.x = HP_X;
		point_b.y = 0.0;
		point_b.z = HP_H;

		position = 0;
	}

	leg_point_to_angles(&point_a, &angles_a);
	leg_point_to_angles(&point_b, &angles_b);
		
	servomotor_set_angle(right_legs[LEG_FRONT].c, angles_a.c);
	servomotor_set_angle(right_legs[LEG_FRONT].f, angles_a.f);
	servomotor_set_angle(right_legs[LEG_FRONT].t, angles_a.t);
	
	servomotor_set_angle(left_legs[LEG_FRONT].c, angles_b.c);
	servomotor_set_angle(left_legs[LEG_FRONT].f, angles_b.f);
	servomotor_set_angle(left_legs[LEG_FRONT].t, angles_b.t);
	
	servomotor_set_angle(right_legs[LEG_MIDDLE].c, angles_b.c);
	servomotor_set_angle(right_legs[LEG_MIDDLE].f, angles_b.f);
	servomotor_set_angle(right_legs[LEG_MIDDLE].t, angles_b.t);
	
	servomotor_set_angle(left_legs[LEG_MIDDLE].c, angles_a.c);
	servomotor_set_angle(left_legs[LEG_MIDDLE].f, angles_a.f);
	servomotor_set_angle(left_legs[LEG_MIDDLE].t, angles_a.t);
	
	servomotor_set_angle(right_legs[LEG_BACK].c, angles_a.c);
	servomotor_set_angle(right_legs[LEG_BACK].f, angles_a.f);
	servomotor_set_angle(right_legs[LEG_BACK].t, angles_a.t);
	
	servomotor_set_angle(left_legs[LEG_BACK].c, angles_b.c);
	servomotor_set_angle(left_legs[LEG_BACK].f, angles_b.f);
	servomotor_set_angle(left_legs[LEG_BACK].t, angles_b.t);
}

void initialize_hp(void)
{
	//es_attach(SM_STEPS_COMPLETE, _hp_steps_complete);
	es_attach(TIMER0_OVF_EVENT, _hp);
	
	//_delay_ms(1);
	es_trigger(TIMER0_OVF_EVENT);//Run
}
