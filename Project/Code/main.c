/*
	ENSE 452 Project
	Dillan Zurowski
	200431334
	Elevator Control System
	-Usart2 task to receive commands to control the direction, doors, and target floor
	-Elevator Control task to iterate the floor number and open/close the door
	-Emergencny stop/maintenance mode interrupt - triggered with the blue user button
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
static 	char direction[1] = "x";
extern SemaphoreHandle_t floorSemaphore;

int main(void)
{	
	serial_open();

	directionQueue = xQueueCreate(1, sizeof(char));			//'u' or 'd'
	floorQueue = xQueueCreate(1, sizeof(int));					//1-8
	CLIQueue = xQueueCreate(1, sizeof(char));						//characters typed, from ISR
	arrivedQueue = xQueueCreate(1, sizeof(int));				//confirmation of arrival
	EmergencyStopQueue = xQueueCreate(1, sizeof(int));	//stop

	
	xTaskCreate(vElevatorControlTask, "Elevator Controller", configMINIMAL_STACK_SIZE, NULL, mainELEVATOR_TASK_PRIORITY, NULL); 
	xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL); 
	SemaphoreHandle_t floorSemaphore = xSemaphoreCreateBinary();

	vTaskStartScheduler();
while(1){}
		
}

static void vElevatorControlTask(void * parameters)
{
	const int ELEVATOR_SPEED = 1500;	//changes floor every 1.5 seconds
	int current_floor_number =1;
	int selected_floor_number = 1;
	bool at_floor = 0;
	CLI_Change_Floor_Number(current_floor_number);
	bool maintenance_mode = 0;

	/*
	1. receive direction 'u' or 'd'
	2. receive desired floor number
	3. receive start message 's'
	4. iterate, check for emergency stop button
	*/
	for (;;)
	{
		if( uxQueueMessagesWaiting( EmergencyStopQueue ) != 0  )	//check if direction has already been selected
		{
			xQueueReceive(EmergencyStopQueue, &maintenance_mode, portMAX_DELAY);		//emergency stop hit, now in maintenance mode
			if (maintenance_mode==true)
			{
				Maintenance_Mode();	//dislay maintenance mode on screen
				xQueueReceive(EmergencyStopQueue, &maintenance_mode, portMAX_DELAY);		//block until the button is hit again.
				maintenance_mode = false;
				current_floor_number = 1;
				selected_floor_number = 1;
				direction[0] = 'x';
				xQueueSendToFront(floorQueue, &current_floor_number, portMAX_DELAY); //send message to CLI that it is starting over
				CLI_Change_Floor_Number(current_floor_number);	//reset display. 
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
					CLI_Change_Floor_Number(current_floor_number);			//change display
					vTaskDelay(pdMS_TO_TICKS(ELEVATOR_SPEED));					//delay 1.5 seonds per floor
					if (current_floor_number >= selected_floor_number)	//if reached selected floor, end
					{
						at_floor = true;
						xSemaphoreGive(floorSemaphore);

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
	
static void vCLITask(void * parameters)
{
	
		CLI_Transmit((uint8_t *)ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set scroll window
    CLI_Transmit((uint8_t *)ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));		//clear the screen in case previous messages were printed
    CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));		//move the cursor to the top to set default message

		char floorStartMessage[] = "Floor 1:";		//start welcome message
		CLI_Transmit((uint8_t *)floorStartMessage, sizeof(floorStartMessage));
		CLI_Transmit((uint8_t *)ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));		//move the cursor to the scroll window in the middle
	
		char welcomeMessage[] = "\r\n'up'/'down', 'open'/'close', floor#'s 1-8\r\nEnter command:";		//start welcome message
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
		vTaskDelay(10);
	}

}
