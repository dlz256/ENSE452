/*
	ENSE 452 Lab2
	Dillan Zurowski
	200431334
	This lab contains a basic example of messaging through USART
	as well as turning on/off the LED with commands
	This header file stores the GPIO init and the connections to the other files
*/

#include "stm32f10x.h"
#include "cli.h"
#include "String"

//uint8_t receivedCharFlag;
//uint8_t receivedChar;

void GPIOInit();
