/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 04/05/2018
Date modified: 23/05/2018

Module: SPI
Functions for using the Serial Peripheral Interface peripheral module

*/

/* INCLUDES */

#ifndef STM32F0_SPI_H
#include "STM32F0_SPI.h"
#define STM32F0_SPI_H
#endif

/* FUNCTIONS */
void init_SPI(SPI_TypeDef* SPIperiph, uint8_t BAUD, int masterMode, int frameFormat, uint8_t dataSize, int dataTransferMode, int multiMasterMode, int clockPolarity, int clockPhase, int crcMode, int rxThreshold) {
	// Initialises and configures an SPI peripheral module
	// Clock
	if (SPIperiph == SPI1) {
		// Enable clock for SPI1
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	}
	else if (SPIperiph == SPI2) {
		// Enable clock for SPI2
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	}

	// Ensure SPI module is disabled before configuration
	while (SPIperiph->SR & SPI_SR_FTLVL); // Wait for any ongoing transmissions to complete
	while (SPIperiph->SR & SPI_SR_BSY); // Wait until the last data frame is processed
	SPIperiph->CR1 &= SPI_CR1_SPE; // Disable the peripheral
	// Clears any junk out of the SPI RX FIFO buffers
	uint16_t dummyData; // A variable to temporarily store data
	while (SPIperiph->SR & SPI_SR_FRLVL) { // Run until buffer is empty
		if (SPIperiph->CR2 & SPI_CR2_FRXTH) {
			// 8-bit access mode
			dummyData = *((uint8_t*)(&SPIperiph->DR));
		}
		else {
			// 16-bit access mode
			dummyData = SPIperiph->DR;
		}
	}

	// BAUD rate
	SPIperiph->CR1 &= ~SPI_CR1_BR; // Reset BAUD bits
	SPIperiph->CR1 |= ((BAUD & 0x7) << 3); // Set BAUD bits

	// Master mode
	if (masterMode) {
		SPIperiph->CR1 |= SPI_CR1_MSTR; // Set to master mode
	}
	else {
		SPIperiph->CR1 &= ~SPI_CR1_MSTR; // Set to slave mode
	}

	// Frame format
	if (frameFormat) {
		SPIperiph->CR1 &= ~SPI_CR1_LSBFIRST; // Set to MSB first
	}
	else {
		SPIperiph->CR1 |= SPI_CR1_LSBFIRST; // Set to LSB first
	}

	// Clock polarity
	if (clockPolarity) {
		SPIperiph->CR1 |= SPI_CR1_CPOL; // Clock HIGH when idle
	}
	else {
		SPIperiph->CR1 &= ~SPI_CR1_CPOL; // Clock LOW when idle
	}

	// Clock phase
	if (clockPhase) {
		SPIperiph->CR1 |= SPI_CR1_CPHA; // Data capture on second clock transmission
	}
	else {
		SPIperiph->CR1 &= ~SPI_CR1_CPHA; // Data capture on first clock transition
	}

	// Data transfer mode
	if (dataTransferMode & SPI_BIDIMODE) { // Bidirectional mode
		SPIperiph->CR1 |= SPI_CR1_BIDIMODE; // 1-line bidirectional mode
	}
	else {
		SPIperiph->CR1 &= ~SPI_CR1_BIDIMODE; // 2-line dual unidirectional mode
	}

	if (dataTransferMode & SPI_BIDIOE) { // Bidirectional output enable
		SPIperiph->CR1 |= SPI_CR1_BIDIOE; // Transmit mode
	}
	else {
		SPIperiph->CR1 &= ~SPI_CR1_BIDIOE; // Receive mode
	}

	if (dataTransferMode & SPI_RXONLY) { // Bidirectional mode
		SPIperiph->CR1 |= SPI_CR1_BIDIMODE; // Receive only
	}
	else {
		SPIperiph->CR1 &= ~SPI_CR1_BIDIMODE; // Full duplex
	}

	// CRC
	if (crcMode & SPI_CRCEN) { // Hardware CRC Enable
		SPIperiph->CR1 |= SPI_CR1_CRCEN; // Enable hardware CRC calculation
	}
	else {
		SPIperiph->CR1 &= ~SPI_CR1_CRCEN; // Disable hardware CRC calculation
	}

	if (crcMode & SPI_CRCL) { // Hardware CRC length
		SPIperiph->CR1 |= SPI_CR1_CRCL; // 8 bit CRC length
	}
	else {
		SPIperiph->CR1 &= ~SPI_CR1_CRCL; // 16 bit CRC length
	}

	// Frame size
	SPIperiph->CR2 &= ~SPI_CR2_DS; // Clear data size bits
	SPIperiph->CR2 |= (dataSize << 8); // Set data size bits

	// SSOE
	if (multiMasterMode) {
		SPIperiph->CR2 |= SPI_CR2_SSOE; // Disable multi-master mode
	}
	else {
		SPIperiph->CR2 &= ~SPI_CR2_SSOE; // Enable multi-master mode
	}

	// FRXTH
	if (rxThreshold) {
		SPIperiph->CR2 |= SPI_CR2_FRXTH; // Set RX threshold to 8 bit
	}
	else {
		SPIperiph->CR2 &= ~SPI_CR2_FRXTH; // Set RX threshold to 16 bit
	}

	// Enable SPI peripheral module
	SPIperiph->CR1 |= SPI_CR1_SPE;

}

void __spiFlushRXBuffer(SPI_TypeDef* SPIperiph) {
	// Clears any junk out of the SPI RX FIFO buffers
	uint16_t dummyData; // A variable to temporarily store data
	while (SPIperiph->SR & SPI_SR_FRLVL) { // Run until buffer is empty
		if (SPIperiph->CR2 & SPI_CR2_FRXTH) {
			// 8-bit access mode
			dummyData = *((uint8_t*)(&SPIperiph->DR));
		}
		else {
			// 16-bit access mode
			dummyData = SPIperiph->DR;
		}
	}
}

void spiTransmitFrame(SPI_TypeDef* SPIperiph, uint16_t data) {
	// Transmits a frame of data over SPI
	uint8_t dataSize = ((SPIperiph->CR1 >> 8) & 0x0F); // Retrieve the data size bits

	while ((SPIperiph->SR & SPI_SR_FTLVL) == 0x1800); // Wait for TX buffer to not be full

	// SPI peripheral module data register is an interface to the TX and RX FIFO buffers, so access must correspond to the data size
	if (dataSize > 7) {
		// 2-byte frame size
		SPIperiph->DR = data; // Place the data in the data register
	}
	else {
		// 1-byte frame size
		*((uint8_t*)(&SPIperiph->DR)) = (uint8_t)data; // Place the data in the data register
	}
	
}


// TODO NB: Re-write for DR access requirements
uint16_t spiReceiveFrame(SPI_TypeDef* SPIperiph) {
	// Gets a frame of data received over SPI
	uint8_t dataSize = ((SPIperiph->CR1 >> 8) & 0x0F); // Retrieve the data size bits

	// Clears any junk out of the SPI RX FIFO buffers
	uint16_t dummyData; // A variable to temporarily store data
	while (SPIperiph->SR & SPI_SR_FRLVL) { // Run until buffer is empty
		if (SPIperiph->CR2 & SPI_CR2_FRXTH) {
			// 8-bit access mode
			dummyData = *((uint8_t*)(&SPIperiph->DR));
		}
		else {
			// 16-bit access mode
			dummyData = SPIperiph->DR;
		}
	}

	// Send a frame of arbitrary data to allow the slave to clock out data
	if (dataSize > 7) {
		// 2-byte frame size
		SPIperiph->DR = 0x6969; // Place the data in the data register
	}
	else {
		// 1-byte frame size
		*((uint8_t*)(&SPIperiph->DR)) = (uint8_t)0x69; // Place the data in the data register
	}
	
	unsigned int timeout = SPI_TIMEOUT_LONG;
	while (!(SPIperiph->SR & SPI_SR_RXNE)) {
		// Wait for data to arrive in the RX buffer
		if (timeout == 0) {
			return 0;
		}
		timeout--;
	}

	// Return the received data
	if (SPIperiph->CR2 & SPI_CR2_FRXTH) {
		// 8-bit access mode
		return ((uint16_t) *((uint8_t*)&SPIperiph->DR));
	}
	else {
		// 16-bit access mode
		return ((uint16_t) SPIperiph->DR);
	}

}

uint16_t spiGetData(SPI_TypeDef* SPIperiph) {
	// Gets the last received data frame
	if (SPIperiph->CR2 & SPI_CR2_FRXTH) {
		// 8-bit access mode
		return ((uint16_t) *((uint8_t*)&SPIperiph->DR));
	}
	else {
		// 16-bit access mode
		return ((uint16_t)SPIperiph->DR);
	}
}