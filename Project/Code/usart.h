/*
	ENSE 452 Lab5
	Dillan Zurowski
	200431334
	-Main task (blinks the LED at a certain rate)
	-CLI task that updates the terminal and receives characters from the 
	USART2 ISR from a Queue and sends to the Main task (via Queue) to 
	change the frequency of the Blinky light.
	-in the USART2 ISR send the data via Queues from FreeRTOS.
*/
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
