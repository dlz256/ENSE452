/*
	ENSE 452 Lab2
	Dillan Zurowski
	200431334
	This lab contains a basic example of messaging through USART
	as well as turning on/off the LED with commands
	
*/


#include "main.h"
uint8_t receivedCharFlag=0;  // Declaration of receivedCharFlag as an extern variable
uint8_t receivedChar;      // Declaration of receivedChar as an extern variable

int main()
{
	serial_open();
	//receivedCharFlag =0;
	const char* inputString;
	
	const char* str = "\r\nEnter command(help for more info):"; 
	uint16_t length = strlen(str);
	uint8_t* array = (uint8_t*)str;	
	CLI_Transmit(array, length);
	
	while(1)
	{
		//uint8_t test = getbyte();
		if (receivedCharFlag)
		{
			sendbyte(receivedChar);
			receivedCharFlag = 0;
			inputString += receivedChar;
			uint16_t length = strlen(str);
			checkCommand(inputString, length);
		}
	}
	serial_close();
		
	return 0;
}
