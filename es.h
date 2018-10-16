/*
 * es.h
 *
 * Created: 26.03.2014 20:13:11
 *  Author: Jenya
 */ 


#ifndef ES_H_
#define ES_H_

#include <stdint.h>

typedef void (*es_handler_t) (void);

#define ES_HANDLERS_MAX 30
#define ES_EVENTS_MAX 30
#define ES_EVENT_NOP 0

void initialize_es(void);

void es_attach(uint8_t code, void (*handler_ptr) (void));

void es_detach(uint8_t code);

void es_trigger(uint8_t code);

void es_dispatch(void);

#endif /* ES_H_ */