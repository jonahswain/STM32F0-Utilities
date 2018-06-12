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



}
/*
USARTperiph - USART peripheral module to configure
BAUD - BAUD rate (based on a 48MHz system clock)
oversampling - 0: oversampling by 16 1: oversampling by 8
frameSize - Number of data bits - 00: 8 data bits 01: 9 data bits 10: 7 data bits (STM32F07x only)
frameFormat - 0: LSB first 1: MSB first
TXEN - Enable transmission
RXEN - Enable reception
stopBits - Number of stop bits - 00: 1 stop bit 10: 2 stop bits 11: 1.5 stop bits (smartcard mode)
parityConfiguration - Bit 0: Parity enable (PCE), Bit 1: Parity select (PS) (0: Even parity 1: Odd parity)
inversionConfiguration - Bit 0: Invert data, Bit 1: Invert TX pin, Bit 2: Invert RX pin
swapTXRX - Swap TX and RX pins
halfDuplexMode - Enable single wire half duplex mode
clockConfiguration - Synchronous mode clock configuration - Bit 0: Enable clock, Bit 1: Clock polarity (0: Idle low 1: Idle high), Bit 2: Clock phase (0: Data capture on first clock transition 1: Data capture on second clock transition), Bit 3: Last bit clock pulse (0: No clock on last data bit 1: Clock on last data bit)
driverConfiguration - External transceiver control - Bit 0: Enable driver enable signal, Bit 1: Driver enable signal polarity (0: Active high 1: Active low)
driverEAT - Driver Enable Assertion Time (sample time units)
driverEDT - Driver Enable De-Assertion Time (sample time units)
*/

void init_USART_serial(USART_TypeDef* USARTperiph, uint32_t BAUD, uint8_t oversampling, uint8_t frameSize, uint8_t frameFormat, uint8_t stopBits, uint8_t parityConfiguration, uint8_t inversionConfiguration, uint8_t swapTXRX) {
	// Initialises a USART peripheral module for serial communication
}
/*
USARTperiph - USART peripheral module to configure
BAUD - BAUD rate (based on a 48MHz system clock)
oversampling - 0: oversampling by 16 1: oversampling by 8
frameSize - Number of data bits - 00: 8 data bits 01: 9 data bits 10: 7 data bits (STM32F07x only)
frameFormat - 0: LSB first 1: MSB first
stopBits - Number of stop bits - 00: 1 stop bit 10: 2 stop bits 11: 1.5 stop bits (smartcard mode)
parityConfiguration - Bit 0: Parity enable (PCE), Bit 1: Parity select (PS) (0: Even parity 1: Odd parity)
inversionConfiguration - Bit 0: Invert data, Bit 1: Invert TX pin, Bit 2: Invert RX pin
swapTXRX - Swap TX and RX pins
*/

void usartSetBAUD(USART_TypeDef* USARTperiph, uint32_t BAUD) {
	// Sets the BAUD rate of a USART peripheral module (based on a 48MHz system clock)
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
