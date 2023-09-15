/*
	ENSE 452 Lab1
	Dillan Zurowski
	200431334
	This lab is to refresh our memory on the STM32 and ensure all devices work right

*/
#include "stm32f10x.h"
void delay()
{
	for (int i =0; i<2000; i++)
	{ 
		for (int i=0; i<2000; i++)
			{}
	}
}
int main()
{
  RCC->APB2ENR |=	RCC_APB2ENR_IOPAEN;
	

	// Configure pin PA5 as a general-purpose output push-pull
	GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
	GPIOA->CRL |= (GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
	
	while(1)
	{
		GPIOA->BSRR |= GPIO_BSRR_BS5;
		delay();
		GPIOA->BSRR |= GPIO_BSRR_BR5;
		delay();
	}
	
	return 0;
}