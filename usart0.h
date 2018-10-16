/**
 * USART stdio compatible driver (unbuffered)
 */ 
#ifndef USART0_H_
#define USART0_H_

/**
 * Library headers
 */
#include <stdint.h>

/**
 * Initialize interface
 */
void initialize_usart0(void);

/**
 * Put character to UART
 */
void usart0_putc(int8_t byte);

/**
 * Get character from UART
 */
int8_t usart0_getc(void);

void usart0_puts(char *str);

#endif /* USART0_H_ */