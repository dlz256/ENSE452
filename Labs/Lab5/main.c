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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cli.h"
#include <stdint.h>
#include "main.h"

extern int characterFlag;
extern char inputString[10]; // Declare and initialize a character array to store the input string
extern QueueHandle_t freqQueue;
extern QueueHandle_t CLIQueue;


int main(void)
{	
	serial_open();

	freqQueue = xQueueCreate(3, sizeof(char));
	CLIQueue = xQueueCreate(3, sizeof(char));
	
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL); 
	xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL); 

	vTaskStartScheduler();
while(1){}
		
}

static void vBlinkTask(void * parameters)
{
		int frequency;
		int defaultFrequency = 1000;
		BaseType_t xStatus = xQueueReceive(freqQueue, &frequency, 0);
		if( xStatus == pdPASS )	//if received new value from the queue
		{
			frequency = (int)pdMS_TO_TICKS(frequency);	//frequency is the new value
		}
		else frequency = defaultFrequency;		//otherwise the frequency is the default
		for (;;)
		{
			GPIOA->BSRR |= GPIO_BSRR_BS5;		
			vTaskDelay((TickType_t)frequency);
			GPIOA->BSRR |= GPIO_BSRR_BR5;		
			vTaskDelay((TickType_t)frequency);
		}
}
	
static void vCLITask(void * parameters)
{
		CLI_Transmit((uint8_t *)ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set scroll window
    CLI_Transmit((uint8_t *)ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));		//clear the screen in case previous messages were printed
    CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));		//move the cursor to the top to set default message
	

		const char statement[] = "The LED is Turned OFF";
		CLI_Transmit(statement, sizeof(statement));						//display the status of the board, defaults to off
	
	
    CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));		//move the cursor to the scroll window in the middle
		
		const char welcomeMessage[] = "\r\nEnter command(help for more info): ";		//start welcome message
		CLI_Transmit(welcomeMessage, sizeof(welcomeMessage));
		
		TickType_t lastWakeTime = xTaskGetTickCount();
		BaseType_t xStatus;
		uint8_t characterReceived = NULL;
		while(1)
		{
				if(characterFlag == 1)	//check if global flag is received
				{		
						xStatus = xQueueReceive(CLIQueue, &characterReceived, portMAX_DELAY);
						if( xStatus == pdPASS )
						{
							sendbyte(characterReceived);				//send received character
							CLI_Receive(&characterReceived, 1);	//process character
							characterFlag= 0;										//SET the flag back to 0
						}
						//vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS( 250 ));
				}
			vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS( 100 ));

		} 
}
