/*
	ENSE 452 Lab5
	Dillan Zurowski
	200431334
	This lab utilizes the USART interup handler
*/

#include "stm32f10x.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"


/** private declares */
void USART2_IRQHandler(void);


/**Global Variables */
int characterFlag;
QueueHandle_t CLIQueue;

void serial_open(void){
		
	  //Enable Port A clock
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |RCC_APB2ENR_IOPCEN;
		//Enable the USART 2 Clock
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable USART2 clock
		
		// Configure USART2 TX (PA2) as output and RX (PA3)  as input
		// Bit pattern for TX is MODE = 11 and CNF is 10 Alternate Function output
		// Bit pattern for RX is MODE = 00 and CNF is 10
		// Configure the USART2 pins (PA2 = TX, PA3 = RX)
		GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
		GPIOA->CRL |= (GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3_1);
		
		// Configure pin PA5 as a general-purpose output push-pull
		GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
		GPIOA->CRL |= (GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
	

		//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
		// Set baud rate to 115200 bps
		USART2->BRR = (19 <<4) | (9 & 0xF);

		// Configure data format: 8 bits, no parity, 1 stop bit
		USART2->CR1 &= ~USART_CR1_M; // 8 data bits
		USART2->CR1 &= ~USART_CR1_PCE; // No parity
		USART2->CR2 &= ~USART_CR2_STOP; // 1 stop bit
	
		NVIC_EnableIRQ(USART2_IRQn);
		
		// Enable the interrupt in NVIC
		//NVIC_SetPriority(USART2_IRQn, 0);
		NVIC_EnableIRQ(TIM2_IRQn);
	
		USART2->CR1 |= USART_CR1_RXNEIE;  // Enable RXNE interrupt
	  // Enable the USART Tx and Rx in the USART Control register.

		USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; //enable
		//USART2->CR2 |= USART_CR2_CLKEN;

		//IRQ
		// Enable TIM2 clock
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

		// Set the prescaler value for 1ms timing assuming a 36MHz clock
		TIM2->PSC = 35999;

		// Set the auto-reload value to achieve a 1s delay
		TIM2->ARR = 50000;

		// Enable the update interrupt
		TIM2->DIER |= TIM_DIER_UIE;

		// Enable the timer
		TIM2->CR1 |= TIM_CR1_CEN;			
}

int sendbyte(uint8_t b) {
	
    TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
	
    // Start the Timer
    TIM2->CR1 |= TIM_CR1_CEN;
		TIM2->CNT = 0; // Reset the timer counter

    while (!(USART2->SR & USART_SR_TXE)) {
        // Check for timeout
        if (TIM2->SR & TIM_SR_UIF) {
            TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
            return 1;  // Timeout error
        }
    }

    USART2->DR = b & 0xFF; // Load the character to be transmitted into the data register

    // Wait for the transmission to complete
    while (!(USART2->SR & USART_SR_TC)) {
        // Check for timeout
        if (TIM2->SR & TIM_SR_UIF) {
            TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
            return 1;  // Timeout error
        }
    }
    // Stop the Timer
    TIM2->CR1 &= ~TIM_CR1_CEN;
		TIM2->CNT = 0; // Reset the timer counter

    return 0; // Successful transmission
}

void serial_close(void)
{
	USART2->CR1 &= ~(USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
	// Configure USART2 settings
	USART2->CR1 &= ~USART_CR1_UE; // Disable USART2

}
// USART2 interrupt service routine
void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {
        // USART2 received data interrupt

        // Read the received data from USART2_DR register
        uint8_t characterReceived = USART2->DR;
			  characterFlag = 1;
				xQueueSendToFrontFromISR(CLIQueue, &characterReceived, NULL);
        // Process the received data as needed

        // Clear the RXNE flag (optional, but recommended)
        USART2->SR &= ~USART_SR_RXNE;
    }

    // Add more conditions for other USART2 interrupt sources if needed
}
