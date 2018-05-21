/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 04/05/2018
Date modified: 21/05/2018

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
	uint16_t dummyData; // A dummy data variable to store pending receive data in
	while (SPIperiph->SR & SPI_SR_FRLVL) {
		dummyData = SPIperiph->DR; // Read in any data from pending receptions
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

void spiTransmitFrame(SPI_TypeDef* SPIperiph, uint16_t data); // Transmits a frame of data over SPI
uint16_t spiReceiveFrame(SPI_TypeDef* SPIperiph); // Gets a frame of data received over SPI