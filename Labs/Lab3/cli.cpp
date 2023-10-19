/*
	ENSE 452 Lab2
	Dillan Zurowski
	200431334
	This lab contains a basic example of messaging through USART
	as well as turning on/off the LED with commands
	This file controls the CLI transmit and receive funnctions
*/

#include "cli.h"

// Transmit data via USART
void CLI_Transmit(uint8_t *pData, uint16_t size) {
	for (int  i=0; i<size; i++)
	{
			sendbyte(pData[i]);//if fail state(1)
	}
}



// Receive data via USART
//void CLI_Receive(uint8_t *pData, uint16_t size) {
//		std::string inputString = "";
//		CLI_Transmit(pData, size);	//send the prompt

//    while(1){

//		uint8_t input = getbyte();	//read input
//		CLI_Transmit(&input, 1);
//		if (input == 0x7F)	//if user entered a backspace, pop the last letter
//		{
//			inputString.pop_back();	//delete desired char
//		}
//		else 
//			inputString +=input;	//if not the backspace, add the char to the input string
//		
//		if (input == '\r')	//wait for enter before submitting command
//		{
//			inputString.pop_back(); //get rid of enter symbol

//			if (inputString == "on")
//			{
//				GPIOA->BSRR |= GPIO_BSRR_BS5;
//				const char* str = "\r\nThe LED is now on!"; 
//				uint16_t length = strlen(str);  // Get the length of the string
//				uint8_t* array = (uint8_t*)str;
//				CLI_Transmit(array, length);
//			}
//			else if (inputString == "off")
//			{
//				GPIOA->BSRR |= GPIO_BSRR_BR5;
//				const char* str = "\r\nThe LED is now off!"; 
//				uint16_t length = strlen(str);  // Get the length of the string
//				uint8_t* array = (uint8_t*)str;
//				CLI_Transmit(array, length);
//			}
//			else if (inputString == "help")
//			{
//				const char* str = "\r\n'on' to turn the LED on\r\n'off' to turn the LED off\r\n'Quit' to close connection"; 
//				uint16_t length = strlen(str);  // Get the length of the string
//				uint8_t* array = (uint8_t*)str;
//				CLI_Transmit(array, length);
//			}
//			else if (inputString == "quit")
//			{
//				const char* str = "\r\nClosing serial connection";
//				uint16_t length = strlen(str);  // Get the length of the string
//				uint8_t* array = (uint8_t*)str;
//				CLI_Transmit(array, length);
//				break;
//			}
//			
//			inputString = ""; 					//clear the string
//			CLI_Transmit(pData, size);	//send the prompt

//			}	//end of enter case
//		
//    } //end of while loop
//} //end of receive function

int checkCommand(const char* inputString, uint16_t size)
{
	if (inputString == "on")
			{
				GPIOA->BSRR |= GPIO_BSRR_BS5;
				const char* str = "\r\nThe LED is now on!"; 
				uint16_t length = strlen(str);  // Get the length of the string
				uint8_t* array = (uint8_t*)str;
				CLI_Transmit(array, length);
				return 0;
			}
			else if (inputString == "off")
			{
				GPIOA->BSRR |= GPIO_BSRR_BR5;
				const char* str = "\r\nThe LED is now off!"; 
				uint16_t length = strlen(str);  // Get the length of the string
				uint8_t* array = (uint8_t*)str;
				CLI_Transmit(array, length);
				return 0;
			}
			else if (inputString == "help")
			{
				const char* str = "\r\n'on' to turn the LED on\r\n'off' to turn the LED off\r\n'Quit' to close connection"; 
				uint16_t length = strlen(str);  // Get the length of the string
				uint8_t* array = (uint8_t*)str;
				CLI_Transmit(array, length);
				return 0;
			}
			else if (inputString == "quit")
			{
				const char* str = "\r\nClosing serial connection";
				uint16_t length = strlen(str);  // Get the length of the string
				uint8_t* array = (uint8_t*)str;
				CLI_Transmit(array, length);
				return 1;
			}
			else
				return 2;
}
