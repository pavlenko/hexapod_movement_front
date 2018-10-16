﻿/*
 * main.c
 *
 * Created: 25.03.2014 22:54:37
 *  Author: Jenya
 */ 
/*
	PE	, // PE 0 ** 0 ** USART0_RX	
	PE	, // PE 1 ** 1 ** USART0_TX	
	PE	, // PE 4 ** 2 ** PWM2	
	PE	, // PE 5 ** 3 ** PWM3	
	PG	, // PG 5 ** 4 ** PWM4	
	PE	, // PE 3 ** 5 ** PWM5	
	PH	, // PH 3 ** 6 ** PWM6	
	PH	, // PH 4 ** 7 ** PWM7	
	PH	, // PH 5 ** 8 ** PWM8	
	PH	, // PH 6 ** 9 ** PWM9	
	PB	, // PB 4 ** 10 ** PWM10	
	PB	, // PB 5 ** 11 ** PWM11	
	PB	, // PB 6 ** 12 ** PWM12	
	PB	, // PB 7 ** 13 ** PWM13	
	PJ	, // PJ 1 ** 14 ** USART3_TX	
	PJ	, // PJ 0 ** 15 ** USART3_RX	
	PH	, // PH 1 ** 16 ** USART2_TX	
	PH	, // PH 0 ** 17 ** USART2_RX	
	PD	, // PD 3 ** 18 ** USART1_TX	
	PD	, // PD 2 ** 19 ** USART1_RX	
	PD	, // PD 1 ** 20 ** I2C_SDA	
	PD	, // PD 0 ** 21 ** I2C_SCL	
	PA	, // PA 0 ** 22 ** D22	
	PA	, // PA 1 ** 23 ** D23	
	PA	, // PA 2 ** 24 ** D24	
	PA	, // PA 3 ** 25 ** D25	
	PA	, // PA 4 ** 26 ** D26	
	PA	, // PA 5 ** 27 ** D27	
	PA	, // PA 6 ** 28 ** D28	
	PA	, // PA 7 ** 29 ** D29	
	PC	, // PC 7 ** 30 ** D30	
	PC	, // PC 6 ** 31 ** D31	
	PC	, // PC 5 ** 32 ** D32	
	PC	, // PC 4 ** 33 ** D33	
	PC	, // PC 3 ** 34 ** D34	
	PC	, // PC 2 ** 35 ** D35	
	PC	, // PC 1 ** 36 ** D36	
	PC	, // PC 0 ** 37 ** D37	
	PD	, // PD 7 ** 38 ** D38	
	PG	, // PG 2 ** 39 ** D39	
	PG	, // PG 1 ** 40 ** D40	
	PG	, // PG 0 ** 41 ** D41	
	PL	, // PL 7 ** 42 ** D42	
	PL	, // PL 6 ** 43 ** D43	
	PL	, // PL 5 ** 44 ** D44	
	PL	, // PL 4 ** 45 ** D45	
	PL	, // PL 3 ** 46 ** D46	
	PL	, // PL 2 ** 47 ** D47	
	PL	, // PL 1 ** 48 ** D48	
	PL	, // PL 0 ** 49 ** D49	
	PB	, // PB 3 ** 50 ** SPI_MISO	
	PB	, // PB 2 ** 51 ** SPI_MOSI	
	PB	, // PB 1 ** 52 ** SPI_SCK	
	PB	, // PB 0 ** 53 ** SPI_SS	
	PF	, // PF 0 ** 54 ** A0	
	PF	, // PF 1 ** 55 ** A1	
	PF	, // PF 2 ** 56 ** A2	
	PF	, // PF 3 ** 57 ** A3	
	PF	, // PF 4 ** 58 ** A4	
	PF	, // PF 5 ** 59 ** A5	
	PF	, // PF 6 ** 60 ** A6	
	PF	, // PF 7 ** 61 ** A7	
	PK	, // PK 0 ** 62 ** A8	
	PK	, // PK 1 ** 63 ** A9	
	PK	, // PK 2 ** 64 ** A10	
	PK	, // PK 3 ** 65 ** A11	
	PK	, // PK 4 ** 66 ** A12	
	PK	, // PK 5 ** 67 ** A13	
	PK	, // PK 6 ** 68 ** A14	
	PK	, // PK 7 ** 69 ** A15	
 */
#include "main.h"

#include "es.h"
#include "hp.h"
#include "led.h"
#include "leg.h"
#include "servomotor.h"
#include "usart0.h"
#include "timer0.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <stdio.h>
//#include <util/delay.h>

volatile uint8_t calibration_sm = 0;
volatile int16_t calibration_val[SM_MAX];

int main(void)
{
	//char str[16];
	
	initialize_usart0();
	initialize_led();
	initialize_es();
	initialize_leg();
	initialize_hp();
	initialize_timer0();
	
	sei();
	
    while(1)
    {
		es_dispatch();
        LED_OFF();
		//TODO:: Please write your application code
		/*int8_t ch = usart0_getc();
		if (ch == 'w') {
			calibration_val[calibration_sm]++;
			
			servomotor_set_middle(calibration_sm, calibration_val[calibration_sm]);
			servomotor_set_angle(calibration_sm, 90);
			
			sprintf(str,"+ i=%d m=%d\r\n", calibration_sm, calibration_val[calibration_sm]);
			usart0_puts(str);
		} else if (ch == 's') {
			calibration_val[calibration_sm]--;
			
			servomotor_set_middle(calibration_sm, calibration_val[calibration_sm]);
			servomotor_set_angle(calibration_sm, 90);
			
			sprintf(str,"- i=%d m=%d\r\n", calibration_sm, calibration_val[calibration_sm]);
			usart0_puts(str);
		} else if (ch == 'a') {
			if (calibration_sm > 0) {
				calibration_sm--;
			}			
			
			servomotor_set_middle(calibration_sm, calibration_val[calibration_sm]);
			servomotor_set_angle(calibration_sm, 90);
			
			sprintf(str,"< i=%d m=%d\r\n", calibration_sm, calibration_val[calibration_sm]);
			usart0_puts(str);
		} else if (ch == 'd') {
			if (calibration_sm < 17) {
				calibration_sm++;
			}
			
			servomotor_set_middle(calibration_sm, calibration_val[calibration_sm]);
			servomotor_set_angle(calibration_sm, 90);
			
			sprintf(str,"> i=%d m=%d\r\n", calibration_sm, calibration_val[calibration_sm]);
			usart0_puts(str);
		}*/			
    }
}