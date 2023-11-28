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

#include "stdint.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"    

void EXTI15_10_IRQHandler(void);


#define ANSI_SCROLL_REGION "\x1b[5;15r"

#define ANSI_POSITION_CURSOR "\x1b[5;0H"
#define ANSI_MOVE_CURSOR_TOP "\x1b[1;1H"  
#define ANSI_MOVE_CURSOR_MIDDLE "\x1b[6;1H" 

#define ANSI_CLEAR_SCREEN "\x1b[2J"
#define ANSI_CLEAR_LINE "\x1b[K"

#define ANSI_SAVE_CURSOR "\x1b[s"
#define ANSI_RESTORE_CURSOR "\x1b[u"

void CLI_Transmit(uint8_t* pData, uint16_t Size);
void CLI_Receive(uint8_t *pData, uint16_t Size);
void CLI_Change_Floor_Number (int);
void Maintenance_Mode();