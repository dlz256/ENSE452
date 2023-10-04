/*
	ENSE 452 Lab2
	Dillan Zurowski
	200431334
	This lab contains a basic example of messaging through USART
	as well as turning on/off the LED with commands
	
*/


#include "main.h"

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
	const char* str = "\r\nEnter command(help for more info):"; 
	uint16_t length = strlen(str);  // Get the length of the string
	uint8_t* array = (uint8_t*)str;
	
	
//		uint8_t data[13];
//		uint16_t arrLength = sizeof(data) / sizeof(data[0]);
		CLI_Receive(array, length);
		serial_close();
		
	return 0;
}
