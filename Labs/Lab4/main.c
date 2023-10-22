/*
	ENSE 452 Lab4
	Dillan Zurowski
	200431334
	This lab contains an example of USART and TIM interrupts

*/

#include "cli.h"
#include <stdint.h>

extern int characterFlag;
extern uint8_t characterReceived;
extern char inputString[10]; // Declare and initialize a character array to store the input string


int main(void)
{
	serial_open();
	
	const char* str = "\r\nEnter command(help for more info):"; 
	uint16_t length = (uint16_t)strlen(str);
	uint8_t* array = (uint8_t*)str;	
	CLI_Transmit(array, length);
	
	while(1)
	{
			if(characterFlag == 1)	//check if global flag is received
			{
				sendbyte(characterReceived);	//send received character
				CLI_Receive(&characterReceived, 1);	//process character
				characterFlag= 0;							//SET the flag back to 0
			}
				
	}
	
	return 0;
}
