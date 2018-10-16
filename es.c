/*
 * es.c
 *
 * Created: 26.03.2014 20:13:28
 *  Author: Jenya
 */
#include "es.h"

#include <stdint.h>

volatile uint8_t es_head;
volatile uint8_t es_tail;
volatile uint8_t es_count;
volatile uint8_t es_events[ES_EVENTS_MAX];
volatile es_handler_t es_handlers[ES_HANDLERS_MAX];

void initialize_es(void)
{
	es_head  = 0;
	es_tail  = 0;
	es_count = 0;
}

void es_attach(uint8_t code, void (*handler_ptr) (void))
{
	if (code < ES_HANDLERS_MAX) {
		es_handlers[code] = handler_ptr;
	}
}

void es_detach(uint8_t code)
{
	if (code < ES_HANDLERS_MAX) {
		es_handlers[code] = 0;
	}
}

void es_trigger(uint8_t code)
{
	if (code < ES_HANDLERS_MAX && es_count < ES_EVENTS_MAX) {
		es_events[es_tail] = code;
		es_tail++;
		
		if (es_tail == ES_EVENTS_MAX) {
			es_tail = 0;
		}
		
		es_count++;
	}
}

void es_dispatch(void)
{
	if (es_count > 0) {
		uint8_t code = es_events[es_head];
		
		es_count--;
		es_head++;
		
		if (es_head == ES_EVENTS_MAX) {
			es_head = 0;
		}
		
		if (es_handlers[code]) {
			es_handlers[code]();
		}
	}	
}