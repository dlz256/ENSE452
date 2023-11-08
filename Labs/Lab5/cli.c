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

#include "cli.h"
#include "FreeRTOS.h"
#include "queue.h"

QueueHandle_t freqQueue;
char inputString[10] = ""; // Declare and initialize a character array to store the input string

// Transmit data via USART
void CLI_Transmit(uint8_t *pData, uint16_t size) {
	for (int  i=0; i<size; i++)
	{
			sendbyte(pData[i]);		//loop through the array and send one byte at a time
	}
}


//Receive data via USART
void CLI_Receive(uint8_t *input, uint16_t size) {
		
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
//			if (strcmp(inputString, (const char *)"on") == 0)	//check if total string is a command
//			{
//				GPIOA->BSRR |= GPIO_BSRR_BS5;		//this function turns on an LED
//				
//				const char LED_on_stmt[] = "\r\nThe LED is now on!"; 
//				CLI_Transmit(LED_on_stmt, sizeof(LED_on_stmt));
//				
//				CLI_Transmit((uint8_t *)ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR));						//save cursor position
//				CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));		//move cursor to top
//				CLI_Transmit((uint8_t *)ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE));							//clear the top line
//				const char statement[] = "The LED is Turned ON";			
//				CLI_Transmit(statement, sizeof(statement));													//send message to top
//				CLI_Transmit((uint8_t *)ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));
//				CLI_Transmit((uint8_t *)ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR));	//restore cursor to middle
//			}
//			else if (strcmp(inputString, (const char *)"off") == 0)
//			{
//				GPIOA->BSRR |= GPIO_BSRR_BR5;										//this function turns off the LED		
//				const char LED_off_stmt[] = "\r\nThe LED is now off!"; 
//				CLI_Transmit(LED_off_stmt, sizeof(LED_off_stmt));
//				
//				CLI_Transmit((uint8_t *)ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR));						//save cursor position
//				CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));		//move cursor to top
//				CLI_Transmit((uint8_t *)ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE));							//clear the top line
//				const char statement[] = "The LED is Turned OFF";			
//				CLI_Transmit(statement, sizeof(statement));													//send message to top
//				CLI_Transmit((uint8_t *)ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));				//reset scroll region
//				CLI_Transmit((uint8_t *)ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR));	//restore cursor to middle

//			}
			if (strcmp(inputString, (const char *)"status") == 0)
			{
				if (GPIOA ->ODR & (1 << 5))
				{
					const char status_stmt[] = "\r\nStatus: The LED is on"; 
					CLI_Transmit(status_stmt, sizeof(status_stmt));
				}
				else
				{
					const char status_stmt[] = "\r\nStatus: The LED is off"; 
					CLI_Transmit(status_stmt, sizeof(status_stmt));
				}
			}
			else if (strcmp(inputString, (const char *)"help") == 0)
			{
				const char help_stmt[] = "\r\n'on' to turn the LED on\r\n'off' to turn the LED off\r\n'status' to view status of LED\r\n'Quit' to close connection"; 
				CLI_Transmit(help_stmt, sizeof(help_stmt));
			}
			else if (strcmp(inputString, (const char *)"quit") == 0)
			{
				const char close_stmt[] = "\r\nClosing serial connection"; 
				CLI_Transmit(close_stmt, sizeof(close_stmt));
				serial_close();
			}
			if (strcmp(inputString, (const char *)"f1") == 0)
			{
				
				int f = 200;
				xQueueSendToFront(freqQueue, &f, portMAX_DELAY);
			}
			else if (strcmp(inputString, (const char *)"f2") == 0)
			{
				int f = 1000;
				xQueueSendToFront(freqQueue, &f, portMAX_DELAY);
			}
			else if (strcmp(inputString, (const char *)"f3") == 0)
			{
				int f = 3000;
				xQueueSendToFront(freqQueue, &f, portMAX_DELAY);				
			}
			
			
			//restarting prompt and erase inutString
			memset(inputString, 0, sizeof(inputString));
			const char str[] = "\r\nEnter command(help for more info):"; 
			CLI_Transmit(str, sizeof(str));
		}	//end of enter case
		else if (inputLength < sizeof(inputString) - 1) 
		{//if not enter or backspace, then add the letter to the saved inputString buffer. 
			inputString[inputLength++] = input[0]; // Add the character to the inputString
			inputString[inputLength] = '\0'; // Null-terminate the string
    }
} //end of receive function

