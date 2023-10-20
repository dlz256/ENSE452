/*
	ENSE 452 Lab3
	Dillan Zurowski
	200431334
	This lab contains an example of USART and TIM interrupts
*/

#include "stdint.h"
#include "usart.h"
#include "string.h"



void CLI_Transmit(uint8_t* pData, uint16_t Size);
int CLI_Receive(uint8_t *pData, uint16_t Size);
//int checkCommand(char* inputString, uint16_t size);



