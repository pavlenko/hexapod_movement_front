/*
 * leg.h
 *
 * Created: 25.03.2014 23:12:11
 *  Author: Jenya
 */ 


#ifndef LEG_H_
#define LEG_H_

#include <stdint.h>

typedef struct {
	uint8_t c;
	uint8_t f;
	uint8_t t;
} leg_t;

typedef struct {
	float x;
	float y;
	float z;
} leg_point_t;

typedef struct {
	int16_t c;
	int16_t f;
	int16_t t;
} leg_angles_t;

#define LEG_FRONT  0
#define LEG_MIDDLE 1
#define LEG_BACK   2

/* Servo thickness, in millimeters */
#define S_THICKNESS 12.0

/* Femur length, in millimeters */
#define F_LENGTH 60.0

/* Tiba length, in millimeters */
#define T_LENGTH 55.0

/* Radians to degree conversion */
//#define RAD_TO_DEG(x) ( (180.0 / M_PI) * (x) )
#define RAD_TO_DEG(x) ( 57.29577951308233 * (x) )
//#define RAD_TO_DEG(x) ( 57.29577951308233 * (x) * 1.031 )

#define SQR(x) ((x) * (x))

void initialize_leg(void);

void leg_point_to_angles(leg_point_t *point, leg_angles_t *angles);

#endif /* LEG_H_ */