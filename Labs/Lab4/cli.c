/*
	ENSE 452 Lab4
	Dillan Zurowski
	200431334
	This lab contains an example of USART and TIM interrupts

*/

#include "cli.h"

char inputString[10] = ""; // Declare and initialize a character array to store the input string

// Transmit data via USART
void CLI_Transmit(uint8_t *pData, uint16_t size) {
	for (int  i=0; i<size; i++)
	{
			sendbyte(pData[i]);//if fail state(1)
	}
}




//Receive data via USART
int CLI_Receive(uint8_t *input, uint16_t size) {
		
		int inputLength = strlen(inputString);
		if (input[0] == 0x7F)										//if user entered a backspace, pop the last letter
		{
				if (inputLength > 0) 								//if the string is not empty
					{
								inputString[--inputLength] = '\0'; 		// Remove the last character
					}		
		}		
		else if (input[0] == 0x0D)	//if the enter button was clicked
		{            
			if (strcmp(inputString, (const char *)"on") == 0)	//check if total string is a command
			{
				GPIOA->BSRR |= GPIO_BSRR_BS5;										//this function turns on an LED
				const char* str = "\r\nThe LED is now on!"; 
				uint16_t length = strlen(str);  
				uint8_t* array = (uint8_t*)str;
				CLI_Transmit(array, length);
			}
			else if (strcmp(inputString, (const char *)"off") == 0)
			{
				GPIOA->BSRR |= GPIO_BSRR_BR5;										//this function turns off the LED		
				const char* str = "\r\nThe LED is now off!"; 
				uint16_t length = strlen(str); 
				uint8_t* array = (uint8_t*)str;
				CLI_Transmit(array, length);
			}
			else if (strcmp(inputString, (const char *)"help") == 0)
			{
				const char* str = "\r\n'on' to turn the LED on\r\n'off' to turn the LED off\r\n'Quit' to close connection"; 
				uint16_t length = strlen(str);  
				uint8_t* array = (uint8_t*)str;
				CLI_Transmit(array, length);
			}
			else if (strcmp(inputString, (const char *)"quit") == 0)
			{
				const char* str = "\r\nClosing serial connection";
				uint16_t length = strlen(str);  //close serial connection and exit
				uint8_t* array = (uint8_t*)str;
				CLI_Transmit(array, length);
				serial_close();
				return 1;
			}
			
			memset(inputString, 0, sizeof(inputString));
			const char* str = "\r\nEnter command(help for more info):"; 
			uint16_t length = (uint16_t)strlen(str);
			uint8_t* array = (uint8_t*)str;	
			CLI_Transmit(array, length);

			}	//end of enter case
		
		else 
			if (inputLength < sizeof(inputString) - 1) {
                inputString[inputLength++] = input[0]; // Add the character to the inputString
                inputString[inputLength] = '\0'; // Null-terminate the string
								return 2;
            }
			return 2;
} //end of receive function

