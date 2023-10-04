/*
	ENSE 452 Lab2
	Dillan Zurowski
	200431334
	This lab contains a basic example of messaging through USART
	as well as turning on/off the LED with commands
	This file controls the reading and writing of bytes to the USART data registers
*/

#include "usart.h"

/** Configure and enable the device. */
void serial_open(void){
	// Enable the USART Tx and Rx in the USART Control register.
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
	// Configure USART2 settings
	//USART2->CR1 &= ~USART_CR1_UE; // Disable USART2

	// Set baud rate to 115200 bps
	USART2->BRR = (19 <<4) | (9 & 0xF);

	// Configure data format: 8 bits, no parity, 1 stop bit
	USART2->CR1 &= ~USART_CR1_M; // 8 data bits
	USART2->CR1 &= ~USART_CR1_PCE; // No parity
	USART2->CR2 &= ~USART_CR2_STOP; // 1 stop bit

	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; //enable
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
int sendbyte(uint8_t b)
{
	// Wait until TXE (Transmit Data Register Empty) flag is set
    while (!(USART2->SR & USART_SR_TXE)) {}

    // Load the character to be transmitted into the data register

			USART2->DR = b & 0xFF;
		while (!(USART2->SR & USART_SR_TC)) {
        // Implement error handling or timeout mechanism if needed
    }
			return 1;
}	
/**
Gets an 8-bit character from the serial port, and returns it.
@pre must have already called serial_open()
*/
uint8_t getbyte(void)
{
	// Wait until RXNE (Read Data Register Not Empty) flag is set
    while (!(USART2->SR & USART_SR_RXNE)) {}

    // Read the received character from the data register
    return (char)(USART2->DR & 0xFF);
}
