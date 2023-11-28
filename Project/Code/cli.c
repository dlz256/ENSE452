/*
	ENSE 452 Project
	Dillan Zurowski
	200431334
	Elevator Control System
	-Usart2 task to receive commands to control the direction, doors, and target floor
	-Elevator Control task to iterate the floor number and open/close the door
	-Emergencny stop/maintenance mode interrupt - triggered with the blue user button
*/

#include "cli.h"
#include "FreeRTOS.h"
#include "queue.h"

QueueHandle_t directionQueue;
QueueHandle_t floorQueue;
QueueHandle_t arrivedQueue;
QueueHandle_t EmergencyStopQueue;
QueueHandle_t doorQueue;
SemaphoreHandle_t floorSemaphore;


static char inputString[10] = ""; // Declare and initialize a character array to store the input string
static int EmergencyFlag;

// Transmit data via USART
void CLI_Transmit(uint8_t *pData, uint16_t size) {
	for (int  i=0; i<size; i++)
	{
			sendbyte(pData[i]);		//loop through the array and send one byte at a time
	}
}


//Receive data via USART
void CLI_Receive(uint8_t *input, uint16_t size) {
		char direction;
		int current_floor;
	
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
		//if direction selected, then can check for floor queue
		if (strcmp(inputString, (const char *)"up") == 0)
		{
			if( uxQueueMessagesWaiting( directionQueue ) == 0 )	//check if direction has already been selected
			{
				direction = 'u';
				xQueueSendToFront(directionQueue, &direction, portMAX_DELAY);
				const char str[] = "\r\nDoor closed!\r\nEnter a floor number(1-8):"; 
				CLI_Transmit((uint8_t *)str, sizeof(str));	
			}
		}
		//down case, decrement
		else if (strcmp(inputString, (const char *)"down") == 0)
		{
			if( uxQueueMessagesWaiting( directionQueue ) == 0 )	//check if direction has already been selected
			{
				direction = 'd';
				xQueueSendToFront(directionQueue, &direction, portMAX_DELAY);
				const char str[] = "\r\nDoor closed!\r\nEnter a floor number(1-8):"; 
				CLI_Transmit((uint8_t *)str, sizeof(str));				
			}						
		}
		else if (atoi(inputString) >= 1 && atoi(inputString) <= 8)
		{
			if( uxQueueMessagesWaiting( floorQueue ) == 0 )	//check if floor has already been selected
			{				
				int floor = atoi(inputString);
				xQueueSendToFront(floorQueue, &floor, portMAX_DELAY);		
				char statement[27];
				snprintf(statement, sizeof(statement), "\r\nTravelling to floor: %d", floor);				
				CLI_Transmit((uint8_t *)statement, sizeof(statement));			
				
				if (xSemaphoreTake(floorSemaphore, portMAX_DELAY) == pdTRUE) {
				
				if (current_floor == floor)	
				{
					const char arrived_msg[] = "\r\nArrived! Opening door...\r\nEnter 'up' or 'down' to select direction"; 
					CLI_Transmit((uint8_t *)arrived_msg, sizeof(arrived_msg));
					int arrived = 0;
				}
			}
			}						
		}			
			//restarting prompt and erase inutString
			memset(inputString, 0, sizeof(inputString));
			const char str[] = "\r\nEnter command:"; 
			CLI_Transmit((uint8_t *)str, sizeof(str));
		}	//end of enter case
		else if (inputLength < sizeof(inputString) - 1) 
		{//if not enter or backspace, then add the letter to the saved inputString buffer. 
			inputString[inputLength++] = input[0]; // Add the character to the inputString
			inputString[inputLength] = '\0'; // Null-terminate the string
    }
} //end of receive function

void CLI_Change_Floor_Number (int floor_num)
{
		CLI_Transmit((uint8_t *)ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR));						//save cursor position
		CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));		//move cursor to top
		CLI_Transmit((uint8_t *)ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE));							//clear the top line

		char statement[10];
    snprintf(statement, sizeof(statement), "Floor: %d", floor_num);
		CLI_Transmit((uint8_t *)statement, sizeof(statement));						//display the status of the board, defaults to off
	
		CLI_Transmit((uint8_t *)ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));				//reset scroll region
		CLI_Transmit((uint8_t *)ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR));	//restore cursor to middle
		
}
void Maintenance_Mode()
{
		CLI_Transmit((uint8_t *)ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR));						//save cursor position
		CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));		//move cursor to top
		CLI_Transmit((uint8_t *)ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE));							//clear the top line

		char mMode[30] = "Maintenance Mode";
		CLI_Transmit((uint8_t *)mMode, sizeof(mMode));						//display the status of the board, defaults to off
	
		CLI_Transmit((uint8_t *)ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));				//reset scroll region
		CLI_Transmit((uint8_t *)ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR));	//restore cursor to middle
		char statement[85] = "\r\nEmergency Stopped! Entering Maintenance Mode.\r\nClick the button to exit";
		CLI_Transmit((uint8_t *)statement, sizeof(statement));						//display the status of the board, defaults to off
	
}

void EXTI15_10_IRQHandler(void){
	if(EXTI->PR & EXTI_PR_PR13) { // Check if the interrupt is from line 13 (corresponding to PC13)
      EXTI->PR |= EXTI_PR_PR13; // Clear the pending bit for line 13
			EmergencyFlag =1;
			xQueueSendToFrontFromISR(EmergencyStopQueue, &EmergencyFlag, NULL);	
    }
}