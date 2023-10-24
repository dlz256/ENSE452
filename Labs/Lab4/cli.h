/*
	ENSE 452 Lab4
	Dillan Zurowski
	200431334
	This lab explores the ansi commands we can use to manipulate the terminal window
*/

#include "stdint.h"
#include "usart.h"
#include "string.h"

#define ANSI_SCROLL_REGION "\x1b[5;15r"

#define ANSI_POSITION_CURSOR "\x1b[5;0H"
#define ANSI_MOVE_CURSOR_TOP "\x1b[1;1H"  
#define ANSI_MOVE_CURSOR_MIDDLE "\x1b[6;1H" 

#define ANSI_CLEAR_SCREEN "\x1b[2J"
#define ANSI_CLEAR_LINE "\x1b[K"

#define ANSI_SAVE_CURSOR "\x1b[s"
#define ANSI_RESTORE_CURSOR "\x1b[u"

void CLI_Transmit(uint8_t* pData, uint16_t Size);
int CLI_Receive(uint8_t *pData, uint16_t Size);



