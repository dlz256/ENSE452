/*
	ENSE 452 Lab3
	Dillan Zurowski
	200431334
	This lab contains a basic example of messaging through USART
	as well as turning on/off the LED with commands
	This file controls the reading and writing of bytes to the USART data registers
*/

#include "usart.h"
extern uint8_t receivedCharFlag;  // Definition of receivedCharFlag and initialization
extern uint8_t receivedChar;      // Definition of receivedChar and initialization


/** Configure and enable the device. */
void serial_open(void){
		// Enable the USART Tx and Rx in the USART Control register.

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
//		NVIC_EnableIRQ(TIM2_IRQn);
	
		USART2->CR1 |= USART_CR1_RXNEIE;  // Enable RXNE interrupt



		USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; //enable

		//IRQ
		// Enable TIM2 clock
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

		// Set the prescaler value for 1ms timing assuming a 36MHz clock
		TIM2->PSC = 35999;

		// Set the auto-reload value to achieve a 1s delay
		TIM2->ARR = 1;

		// Enable the update interrupt
		TIM2->DIER |= TIM_DIER_UIE;

		// Enable the timer
		TIM2->CR1 |= TIM_CR1_CEN;
		//ISER
		//NVIC->ISER[0] = 1<<7; //enable IRQ7 (bit 7 of ISER[0]
		

			
}


/**
Undo whatever serial_open() did, setting the peripheral back to
its reset configuration.
*/
void serial_close(void)
{
	USART2->CR1 &= ~(USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
	// Configure USART2 settings
	USART2->CR1 &= ~USART_CR1_UE; // Disable USART2

}

/**
Send an 8-bit byte to the serial port, using the configured
bit-rate, # of bits, etc. Returns 0 on success and non-zero on
failure.
@param b the 8-bit quantity to be sent.
@pre must have already called serial_open()
*/
int sendbyte(uint8_t b) {
    // Set the timeout in Timer2
//    TIM2->ARR = Timeout; // Assuming the timer is configured to count microseconds

//    // Start the Timer
//    TIM2->CR1 |= TIM_CR1_CEN;

    while (!(USART2->SR & USART_SR_TXE)) {
//        // Check for timeout
//        if (TIM2->SR & TIM_SR_UIF) {
//            TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
//            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
//            return 1;  // Timeout error
//        }
    }

    USART2->DR = b & 0xFF; // Load the character to be transmitted into the data register

    // Wait for the transmission to complete
//    while (!(USART2->SR & USART_SR_TC)) {
//        // Check for timeout
//        if (TIM2->SR & TIM_SR_UIF) {
//            TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
//            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
//            return 1;  // Timeout error
//        }
//    }
//    // Stop the Timer
//    TIM2->CR1 &= ~TIM_CR1_CEN;

    return 0; // Successful transmission
}
/**
Gets an 8-bit character from the serial port, and returns it.
@pre must have already called serial_open()
*/
//uint8_t getbyte(void)
//{
//	// Wait until RXNE (Read Data Register Not Empty) flag is set
//    while (!(USART2->SR & USART_SR_RXNE)) {}

//    // Read the received character from the data register
//    return (char)(USART2->DR & 0xFF);
//}



void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {  // Character received, clear the flag and handle the received character
        receivedCharFlag = 1;          // Handle the received character
        receivedChar = (uint8_t)(USART2->DR);
    }
		else sendbyte('h');
}
