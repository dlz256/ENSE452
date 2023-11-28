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
#include <stdint.h>


#define mainELEVATOR_TASK_PRIORITY (tskIDLE_PRIORITY +2)
static void vElevatorControlTask (void* parameters);

#define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY +1)
static void vCLITask (void* parameters);

