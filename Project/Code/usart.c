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
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

		//Enable the USART 2 Clock
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable USART2 clock
		
		// Configure USART2 TX (PA2) as output and RX (PA3)  as input
		GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
		GPIOA->CRL |= (GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3_1);
		
		// Configure pin PA5 as a general-purpose output push-pull
		GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
		GPIOA->CRL |= (GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
	
		// Configure pin PC13(user button) as a general-purpose output push-pull
		//GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
		//GPIOC->CRH |= (GPIO_CRH_MODE13_1);
		
		AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC; // Map PC13 to EXTI13
		// Enable EXTI line 13 for PA13
    EXTI->IMR |= EXTI_IMR_MR13; // Enable interrupt on EXTI line 13 (PC13)
    EXTI->FTSR |= EXTI_FTSR_TR13; // Set falling edge trigger on EXTI line 13 (PC13)

    // Enable and set EXTI line 0 IRQ in NVIC
		NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable EXTI 15 to 10 IRQ in NVIC
		NVIC_SetPriority(EXTI15_10_IRQn, 6); // Set priority level

	

		//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
		USART2->BRR = (19 <<4) | (9 & 0xF);
		// Configure data format: 8 bits, no parity, 1 stop bit
		USART2->CR1 &= ~USART_CR1_M; // 8 data bits
		USART2->CR1 &= ~USART_CR1_PCE; // No parity
		USART2->CR2 &= ~USART_CR2_STOP; // 1 stop bit
	
		NVIC_EnableIRQ(USART2_IRQn);
		
		// Enable the interrupt in NVIC
		NVIC_EnableIRQ(TIM2_IRQn);
	
		USART2->CR1 |= USART_CR1_RXNEIE;  // Enable RXNE interrupt
	  // Enable the USART Tx and Rx in the USART Control register.

		USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; //enable

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

void serial_close(void){
	USART2->CR1 &= ~(USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
	// Configure USART2 settings
	USART2->CR1 &= ~USART_CR1_UE; // Disable USART2

}
// USART2 interrupt service routine
void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {
			
        // Read the received data from USART2_DR register
        uint8_t characterReceived = USART2->DR;
				xQueueSendToFrontFromISR(CLIQueue, &characterReceived, NULL);

        // Clear the RXNE flag (optional, but recommended)
        USART2->SR &= ~USART_SR_RXNE;
    }
}

