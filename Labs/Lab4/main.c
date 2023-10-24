/*
	ENSE 452 Lab4
	Dillan Zurowski
	200431334
	This lab explores the ansi commands we can use to manipulate the terminal window

*/

#include "cli.h"
#include <stdint.h>
#include "main.h"

extern int characterFlag;
extern uint8_t characterReceived;
extern char inputString[10]; // Declare and initialize a character array to store the input string


int main(void)
{
	serial_open();
	
		CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set scroll window
    CLI_Transmit(ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));		//clear the screen in case previous messages were printed
    CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));		//move the cursor to the top to set default message
	

		const char statement[] = "The LED is Turned OFF";
		CLI_Transmit(statement, sizeof(statement));						//display the status of the board, defaults to off
	
	
    CLI_Transmit(ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));		//move the cursor to the scroll window in the middle
		
		const char welcomeMessage[] = "\r\nEnter command(help for more info):";		//start welcome message
		CLI_Transmit(welcomeMessage, sizeof(welcomeMessage));
		
			
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
