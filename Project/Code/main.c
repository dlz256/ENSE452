/*
	ENSE 452 Project
	Dillan Zurowski
	200431334
	Elevator Control System
	-Usart2 task to receive commands to control the direction, doors, and target floor
	-Elevator Control task to iterate the floor number and open/close the door
	-Emergencny stop/maintenance mode interrupt - triggered with the blue user button
	Assumptions:
	- Only one user at a time.
	- The user starts on the current floor of the door
*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"

extern QueueHandle_t directionQueue;
extern QueueHandle_t CLIQueue;
extern QueueHandle_t floorQueue;
extern QueueHandle_t arrivedQueue;
extern QueueHandle_t EmergencyStopQueue;

int main(void)
{	
	serial_open();

	directionQueue = xQueueCreate(1, sizeof(char));			//'u' or 'd'
	floorQueue = xQueueCreate(1, sizeof(int));					//1-8
	CLIQueue = xQueueCreate(1, sizeof(char));						//characters typed, from ISR
	arrivedQueue = xQueueCreate(1, sizeof(int));				//confirmation of arrival
	EmergencyStopQueue = xQueueCreate(1, sizeof(int));	//confirmation of arrival


	
	xTaskCreate(vElevatorControlTask, "Elevator Controller", configMINIMAL_STACK_SIZE, NULL, mainELEVATOR_TASK_PRIORITY, NULL); 
	xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL); 

	vTaskStartScheduler();
	while(1){}
		
}
//This task controls the state of the elevator. 
//Keeps track of the selected direction, current floor, and target floor. 
/*
	1. check for emergency stop button
	2. receive direction 'u' or 'd' as well as desired floor number
	3. iterate from current floor to the desired floor
*/
static void vElevatorControlTask(void * parameters)
{
	const int ELEVATOR_SPEED = 1500;	//changes floor every 1.5 seconds
	int current_floor_number =1;
	int selected_floor_number = 1;
	bool at_floor = 0;
	char direction[1] = "x";
	bool maintenance_mode = 0;

	for (;;)
	{
		if( uxQueueMessagesWaiting( EmergencyStopQueue ) != 0  )	//check if direction has already been selected
		{
			xQueueReceive(EmergencyStopQueue, &maintenance_mode, portMAX_DELAY);		//emergency stop hit, now in maintenance mode
			if (maintenance_mode==true)
			{
				Maintenance_Mode();	//dislay maintenance mode on screen
				xQueueReceive(EmergencyStopQueue, &maintenance_mode, portMAX_DELAY);		//block until the button is hit again.
				xQueueSendToFront(floorQueue, &current_floor_number, portMAX_DELAY); 		//tell CLI the floor it is on

				maintenance_mode = false;
				CLI_Change_Floor_Number(current_floor_number);	//reset display. 
				char welcomeMessage[] = "\r\nRestarting...\r\n'up'/'down', floor#'s 1-8\r\nEnter direction: ";		//start welcome message
				CLI_Transmit((uint8_t *)welcomeMessage, sizeof(welcomeMessage));
			}
			vTaskDelay(10);
		}
		else if( uxQueueMessagesWaiting( directionQueue ) != 0  )	//check if direction has already been selected
		{
			xQueueReceive(directionQueue, &direction, portMAX_DELAY);		//idle state, waiting for "up" or "down" command
			xQueueReceive(floorQueue, &selected_floor_number, portMAX_DELAY);		//in elevator, waiting for floor number

		}
		else if (strcmp(direction, (const char *)"u") == 0 && at_floor == false && maintenance_mode == false)
			{
					CLI_Change_Floor_Number(current_floor_number);
					vTaskDelay((pdMS_TO_TICKS(ELEVATOR_SPEED)));
					if (current_floor_number >= selected_floor_number)
					{
						at_floor = true;
						xQueueSendToFront(floorQueue, &current_floor_number, portMAX_DELAY); //send message to CLI that it has arrived
						xQueueReceive(arrivedQueue, &at_floor, portMAX_DELAY);		//sets at_floor back to false

						direction[0] = 'x';
					}
					else current_floor_number++;
			}
		else if (strcmp(direction, (const char *)"d") == 0 && at_floor == false && maintenance_mode == false)
			{
					CLI_Change_Floor_Number(current_floor_number);
					vTaskDelay((pdMS_TO_TICKS(ELEVATOR_SPEED)));
					if (current_floor_number <= selected_floor_number)
					{
						at_floor = true;
						xQueueSendToFront(floorQueue, &current_floor_number, portMAX_DELAY); //send message to CLI that it has arrived
						xQueueReceive(arrivedQueue, &at_floor, portMAX_DELAY);		//sets at_floor back to false
						direction[0] = 'x';
					}					
					else current_floor_number--;
			}
			
		else vTaskDelay((pdMS_TO_TICKS(100)));
		
	}
}
	
//This task will control the input of the user and output feedback to the CLI
static void vCLITask(void * parameters)
{
	
	CLI_Transmit((uint8_t *)ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set scroll window
	CLI_Transmit((uint8_t *)ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));		//clear the screen in case previous messages were printed
	CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));		//move the cursor to the top to set default message

	char floorStartMessage[] = "Floor: 1";		//start welcome message
	CLI_Transmit((uint8_t *)floorStartMessage, sizeof(floorStartMessage));
	CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));		//move the cursor to the scroll window in the middle

	char welcomeMessage[] = "\r\n'up'/'down', floor#'s 1-8\r\nEnter direction: ";		//start welcome message
	CLI_Transmit((uint8_t *)welcomeMessage, sizeof(welcomeMessage));

	uint8_t characterReceived;
	for (;;)
	{
		BaseType_t xStatus = xQueueReceive(CLIQueue, &characterReceived, portMAX_DELAY);
		if( xStatus == pdPASS )
		{
			sendbyte(characterReceived);				//send received character
			CLI_Receive(&characterReceived, 1);	//process character
		}		
	}

}
