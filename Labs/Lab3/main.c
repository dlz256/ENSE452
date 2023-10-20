#include "cli.h"
#include <stdint.h>

extern int characterFlag;
extern uint8_t characterReceived;
extern char inputString[10]; // Declare and initialize a character array to store the input string


int main(void)
{
	//usart2_open();
	serial_open();
//	char inputString[30]; // Define MAX_INPUT_LENGTH according to your needs
//  char*  inputString[0] = '\0';
//	int inputLength=0;

	
	const char* str = "\r\nEnter command(help for more info):"; 
	uint16_t length = (uint16_t)strlen(str);
	uint8_t* array = (uint8_t*)str;	
	CLI_Transmit(array, length);
	while(1)
	{

			if(characterFlag == 1)
			{
				sendbyte(characterReceived);
				CLI_Receive(&characterReceived, 1);
				characterFlag= 0;
			}
				
	}
	
	return 0;
}
