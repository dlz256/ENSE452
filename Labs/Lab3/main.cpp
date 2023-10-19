/*
	ENSE 452 Lab2
	Dillan Zurowski
	200431334
	This lab contains a basic example of messaging through USART
	as well as turning on/off the LED with commands
	
*/


#include "main.h"
uint8_t receivedCharFlag=0;  // Declaration of receivedCharFlag as an extern variable
uint8_t receivedChar;      // Declaration of receivedChar as an extern variable

void GPIOInit()
{	
	//GPIO
	RCC->APB1ENR |=	RCC_APB1ENR_USART2EN;
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;
	
	// Configure pin PA2 as a alternate function output push-pull (10 10)
	GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
	GPIOA->CRL |= (GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2);
	
	//Configure PA3 for Input with pull-up / pull-down. (10 00)
	GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
	GPIOA->CRL |= (GPIO_CRL_CNF3_1);
	
	// Configure pin PA5 as a general-purpose output push-pull
	GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
	GPIOA->CRL |= (GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
		

}



int main()
{
	GPIOInit();
	serial_open();
	//receivedCharFlag =0;
	const char* inputString;
	
	const char* str = "\r\nEnter command(help for more info):"; 
	uint16_t length = strlen(str);
	uint8_t* array = (uint8_t*)str;	
	CLI_Transmit(array, length);
	
	while(1)
	{
		//uint8_t test = getbyte();
		if (receivedCharFlag)
		{
			sendbyte(receivedChar);
			receivedCharFlag = 0;
			inputString += receivedChar;
			uint16_t length = strlen(str);
			checkCommand(inputString, length);
		}
	}
	serial_close();
		
	return 0;
}
