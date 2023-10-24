#ifndef USART_H
#define USART_H

#include <stdint.h>
#include "string.h"
#include <stm32f10x.h>


/** public functions*/
void usart2_open(void);
void serial_open(void);
void serial_close(void);


int sendbyte(uint8_t b);

uint8_t getbyte(void);
#endif