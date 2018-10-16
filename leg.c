/*
 * leg.c
 *
 * Created: 25.03.2014 23:11:56
 *  Author: Jenya
 */ 
#include "leg.h"

#include "servomotor.h"

#include <avr/io.h>
#include <math.h>

volatile float hp_body_height = 75.0;

volatile leg_t right_legs[3];
volatile leg_t left_legs[3];

void initialize_leg(void)
{
	right_legs[LEG_FRONT].c = servomotor_attach(&PORTL, PL3, 130, 1);//130
	right_legs[LEG_FRONT].f = servomotor_attach(&PORTL, PL2, -60, 1);//-60
	right_legs[LEG_FRONT].t = servomotor_attach(&PORTL, PL1, -80, 0);//-80
	
	right_legs[LEG_MIDDLE].c = servomotor_attach(&PORTK, PK4, 80, 1);//80
	right_legs[LEG_MIDDLE].f = servomotor_attach(&PORTK, PK3, -270, 1);//-540????
	right_legs[LEG_MIDDLE].t = servomotor_attach(&PORTK, PK2, -330, 0);//-330
	
	right_legs[LEG_BACK].c = servomotor_attach(&PORTF, PF2, 220, 1);//220
	right_legs[LEG_BACK].f = servomotor_attach(&PORTF, PF1, -190, 1);//-190
	right_legs[LEG_BACK].t = servomotor_attach(&PORTF, PF0, -170, 0);//-170
	
	left_legs[LEG_FRONT].c = servomotor_attach(&PORTA, PA7, -10, 0);//-10
	left_legs[LEG_FRONT].f = servomotor_attach(&PORTA, PA6, -70, 0);//-70
	left_legs[LEG_FRONT].t = servomotor_attach(&PORTA, PA5, 120, 1);//120
	
	left_legs[LEG_MIDDLE].c = servomotor_attach(&PORTA, PA4, -130, 0);//-130
	left_legs[LEG_MIDDLE].f = servomotor_attach(&PORTA, PA3, -140, 0);//-140
	left_legs[LEG_MIDDLE].t = servomotor_attach(&PORTA, PA2, -50, 1);//-50
	
	left_legs[LEG_BACK].c = servomotor_attach(&PORTB, PB4, -110, 0);//-110
	left_legs[LEG_BACK].f = servomotor_attach(&PORTB, PB5, -210, 0);//-210
	left_legs[LEG_BACK].t = servomotor_attach(&PORTB, PB6, -40, 1);//-40
}

void leg_point_to_angles(leg_point_t *point, leg_angles_t *angles)
{
	/* Calculate horizontal distance in millimeters */
	float h_distance = hypot((*point).x, (*point).y);

	/* Calculate coxa angle in radians */
	float c_angle = atan((*point).y / (*point).x) + M_PI_2;

	/* Calculate vertical distance in millimeters */
	float v_distance = sqrt(SQR(h_distance - S_THICKNESS) + SQR(hp_body_height - (*point).z));

	/* Calculate femur add angle from z to v_distance axis in radians */
	float f_add_angle = atan((h_distance - S_THICKNESS) / (hp_body_height - (*point).z));

	/* Calculate femur angle in radians */
	float f_angle = acos((SQR(F_LENGTH) + SQR(v_distance) - SQR(T_LENGTH)) / (2 * F_LENGTH * v_distance)) + f_add_angle;
	
	/* Calculate tiba angle in radians */
	float t_angle = acos((SQR(T_LENGTH) + SQR(F_LENGTH) - SQR(v_distance)) / (2 * T_LENGTH * F_LENGTH));

	(*angles).c = (uint16_t) RAD_TO_DEG(c_angle);
	(*angles).f = (uint16_t) RAD_TO_DEG(f_angle);
	(*angles).t = (uint16_t) RAD_TO_DEG(t_angle);
}