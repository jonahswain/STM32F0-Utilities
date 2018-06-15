/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 12/06/2018
Date modified: 12/06/2018

Module: USART
Functions for communicating over USART

*/

/* INCLUDES */

#ifndef STM32F0_USART_H
#include "STM32F0_USART.h"
#define STM32F0_USART_H
#endif

/* FUNCTIONS */
void init_USART(USART_TypeDef* USARTperiph, uint32_t BAUD, uint8_t oversampling, uint8_t frameSize, uint8_t frameFormat, uint8_t TXEN, uint8_t RXEN, uint8_t stopBits, uint8_t parityConfiguration, uint8_t inversionConfiguration, uint8_t swapTXRX, uint8_t halfDuplexMode, uint8_t clockConfiguration, uint8_t driverConfiguration, uint8_t driverEAT, uint8_t driverEDT) {
	// Initialises a USART peripheral module

	// Enable clock for USART peripheral module
	if (USARTperiph == USART1) {
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	}
	else if (USARTperiph == USART2) {
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	}
	else if (USARTperiph == USART3) {
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	}
	else if (USARTperiph == USART4) {
		RCC->APB1ENR |= RCC_APB1ENR_USART4EN;
	}
	else {
		return; // Invalid USART peripheral, do not continue with configuration
	}

	USARTperiph->CR1 &= ~USART_CR1_UE; // Disable USART for configuration

	// Configure BAUD rate and oversampling
	uint16_t usartDiv = 48000000 / BAUD; // USARTDIV (for BAUD rate)
	if (oversampling) {
		// Oversampling 8
		USARTperiph->CR1 |= USART_CR1_OVER8;
		USARTperiph->BRR = ((usartDiv & 0xFFF0) | ((usartDiv & 0x000F) >> 1));
	}
	else {
		// Oversampling 16
		USARTperiph->CR1 &= ~USART_CR1_OVER8;
		USARTperiph->BRR = usartDiv;
	}

	// Configure frame size
	if (frameSize & 1) {
		USARTperiph->CR1 |= USART_CR1_M0;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_M0;
	}
	if (frameSize & 2) {
		USARTperiph->CR1 |= USART_CR1_M1;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_M1;
	}

	// Configure frame format
	if (frameFormat) {
		USARTperiph->CR2 |= USART_CR2_MSBFIRST;
	}
	else {
		USARTperiph->CR2 &= USART_CR2_MSBFIRST;
	}

	// Configure stop bits
	USARTperiph->CR2 &= ~USART_CR2_STOP;
	USARTperiph->CR2 |= ((stopBits & 0x03) << 12);

	// Configure parity
	if (parityConfiguration & USART_PARITY_ENABLE) {
		USARTperiph->CR1 |= USART_CR1_PCE;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_PCE;
	}
	if (parityConfiguration & USART_PARITY_ODD) {
		USARTperiph->CR1 |= USART_CR1_PS;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_PS;
	}

	// Configure inversion
	if (inversionConfiguration & USART_INVERSION_DATA) {
		USARTperiph->CR2 |= USART_CR2_DATAINV;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_DATAINV;
	}
	if (inversionConfiguration & USART_INVERSTION_TRANSMIT) {
		USARTperiph->CR2 |= USART_CR2_TXINV;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_TXINV;
	}
	if (inversionConfiguration & USART_INVERSION_RECEIVE) {
		USARTperiph->CR2 |= USART_CR2_RXINV;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_RXINV;
	}

	// Configure swap TX/RX pins
	if (swapTXRX) {
		USARTperiph->CR2 |= USART_CR2_SWAP;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_SWAP;
	}

	// Configure half duplex
	if (halfDuplexMode) {
		USARTperiph->CR3 |= USART_CR3_HDSEL;
	}
	else {
		USARTperiph->CR3 &= ~USART_CR3_HDSEL;
	}

	// Configure clock (synchronous mode)
	if (clockConfiguration & USART_CLOCK_ENABLE) { // Clock enable
		USARTperiph->CR2 |= USART_CR2_CLKEN;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_CLKEN;
	}
	if (clockConfiguration & USART_CLOCK_POLARITY) { // Clock polarity
		USARTperiph->CR2 |= USART_CR2_CPOL;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_CPOL;
	}
	if (clockConfiguration & USART_CLOCK_PHASE) { // Clock phase
		USARTperiph->CR2 |= USART_CR2_CPHA;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_CPHA;
	}
	if (clockConfiguration & USART_CLOCK_LASTBITCLOCK) { // Last bit clock
		USARTperiph->CR2 |= USART_CR2_LBCL;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_LBCL;
	}

	// Configure external driver enable signal
	if (driverConfiguration & USART_DRIVER_ENABLE) { // Driver enable
		USARTperiph->CR3 |= USART_CR3_DEM;
	}
	else {
		USARTperiph->CR3 &= ~USART_CR3_DEM;
	}
	if (driverConfiguration & USART_DRIVER_POLARITY) { // Driver polarity
		USARTperiph->CR3 |= USART_CR3_DEP;
	}
	else {
		USARTperiph->CR3 &= ~USART_CR3_DEP;
	}

	// Driver enable assertion/deassertion time
	USARTperiph->CR1 &= 0xFC00FFFF; // Clear assertion/deassertion bits
	USARTperiph->CR1 |= ((driverEAT & 0x1F) << 21); // Set assertion bits
	USARTperiph->CR1 |= ((driverEDT & 0x1F) << 16); // Set assertion bits

	USARTperiph->CR1 |= USART_CR1_UE; // Enable USART

	// Configure TXEN
	if (TXEN) {
		USARTperiph->CR1 |= USART_CR1_TE;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_TE;
	}

	// Configure RXEN
	if (RXEN) {
		USARTperiph->CR1 |= USART_CR1_RE;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_RE;
	}
}

void init_USART_serial(USART_TypeDef* USARTperiph, uint32_t BAUD, uint8_t oversampling, uint8_t frameSize, uint8_t frameFormat, uint8_t stopBits, uint8_t parityConfiguration, uint8_t inversionConfiguration, uint8_t swapTXRX) {
	// Initialises a USART peripheral module for serial communication

	// Enable clock for USART peripheral module
	if (USARTperiph == USART1) {
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	}
	else if (USARTperiph == USART2) {
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	}
	else if (USARTperiph == USART3) {
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	}
	else if (USARTperiph == USART4) {
		RCC->APB1ENR |= RCC_APB1ENR_USART4EN;
	}
	else {
		return; // Invalid USART peripheral, do not continue with configuration
	}

	USARTperiph->CR1 &= ~USART_CR1_UE; // Disable USART for configuration

	// Configure BAUD rate and oversampling
	uint16_t usartDiv = 48000000 / BAUD; // USARTDIV (for BAUD rate)
	if (oversampling) {
		// Oversampling 8
		USARTperiph->CR1 |= USART_CR1_OVER8;
		USARTperiph->BRR = ((usartDiv & 0xFFF0) | ((usartDiv & 0x000F) >> 1));
	}
	else {
		// Oversampling 16
		USARTperiph->CR1 &= ~USART_CR1_OVER8;
		USARTperiph->BRR = usartDiv;
	}

	// Configure frame size
	if (frameSize & 1) {
		USARTperiph->CR1 |= USART_CR1_M0;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_M0;
	}
	if (frameSize & 2) {
		USARTperiph->CR1 |= USART_CR1_M1;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_M1;
	}

	// Configure frame format
	if (frameFormat) {
		USARTperiph->CR2 |= USART_CR2_MSBFIRST;
	}
	else {
		USARTperiph->CR2 &= USART_CR2_MSBFIRST;
	}

	// Configure stop bits
	USARTperiph->CR2 &= ~USART_CR2_STOP;
	USARTperiph->CR2 |= ((stopBits & 0x03) << 12);

	// Configure parity
	if (parityConfiguration & USART_PARITY_ENABLE) {
		USARTperiph->CR1 |= USART_CR1_PCE;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_PCE;
	}
	if (parityConfiguration & USART_PARITY_ODD) {
		USARTperiph->CR1 |= USART_CR1_PS;
	}
	else {
		USARTperiph->CR1 &= ~USART_CR1_PS;
	}

	// Configure inversion
	if (inversionConfiguration & USART_INVERSION_DATA) {
		USARTperiph->CR2 |= USART_CR2_DATAINV;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_DATAINV;
	}
	if (inversionConfiguration & USART_INVERSTION_TRANSMIT) {
		USARTperiph->CR2 |= USART_CR2_TXINV;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_TXINV;
	}
	if (inversionConfiguration & USART_INVERSION_RECEIVE) {
		USARTperiph->CR2 |= USART_CR2_RXINV;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_RXINV;
	}

	// Configure swap TX/RX pins
	if (swapTXRX) {
		USARTperiph->CR2 |= USART_CR2_SWAP;
	}
	else {
		USARTperiph->CR2 &= ~USART_CR2_SWAP;
	}

	// Disable half duplex
	USARTperiph->CR3 &= ~USART_CR3_HDSEL;

	// Disable clock
	USARTperiph->CR2 &= ~USART_CR2_CLKEN;

	// Reset external driver enable signal
	USARTperiph->CR3 &= ~USART_CR3_DEM;
	USARTperiph->CR3 &= ~USART_CR3_DEP;
	USARTperiph->CR1 &= 0xFC00FFFF;

	USARTperiph->CR1 |= USART_CR1_UE; // Enable USART

	// Enable TX/RX
	USARTperiph->CR1 |= USART_CR1_TE;
	USARTperiph->CR1 |= USART_CR1_RE;
}

void usartSetBAUD(USART_TypeDef* USARTperiph, uint32_t BAUD) {
	// Sets the BAUD rate of a USART peripheral module (based on a 48MHz system clock)

	uint16_t usartDiv = 48000000 / BAUD; // USARTDIV (for BAUD rate)
	if (USARTperiph->CR1 & USART_CR1_OVER8) {
		// Oversampling 8
		USARTperiph->BRR = ((usartDiv & 0xFFF0) | ((usartDiv & 0x000F) >> 1));
	}
	else {
		// Oversampling 16
		USARTperiph->BRR = usartDiv;
	}
}

void usartAutoBAUDDetection(USART_TypeDef* USARTperiph) {
	// Automatically detects the BAUD rate on a USART peripheral module (Note char to send)


}

void usartConfigureAddressRecognition(USART_TypeDef* USARTperiph, uint8_t address, uint8_t addressRecognitionConfig) {
	// Configures address/character recognition on a USART peripheral module
}

void usartSetAddress(USART_TypeDef* USARTperiph, uint8_t address) {
	// Sets the address of a USART peripheral module
}

uint8_t usartRXNE(USART_TypeDef* USARTperiph) {
	// Returns whether the USART peripheral module has data in its RX buffer
}

uint8_t usartGetData(USART_TypeDef* USARTperiph) {
	// Returns any data received over USART
}

uint8_t usartReceiveData(USART_TypeDef* USARTperiph) {
	// Waits for data from USART before returning it
}

void usartTransmitData(USART_TypeDef* USARTperiph, uint8_t data) {
	// Sends a data frame over USART
}

void usartDMATransmitData(USART_TypeDef* USARTperiph, uint8_t* data, uint16_t dataSize) {
	// Sends a block of data over USART using DMA
}
