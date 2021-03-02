/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 26/05/2018
Date modified: 26/05/2018

Module: DMA
Functions for configuring/controlling the direct memory access module

*/

/* INCLUDES */
#ifndef STM32F0_DMA_H
#include "STM32F0_DMA.h"
#define STM32F0_DMA_H
#endif

/* FUNCTIONS */
DMA_Channel_TypeDef* __dmaChannelAddress(uint8_t channel) {
	// Returns the address of a DMA channel (configuration registers)
	switch (channel) {
	case 1: return DMA1_Channel1;
	case 2: return DMA1_Channel2;
	case 3: return DMA1_Channel3;
	case 4: return DMA1_Channel4;
	case 5: return DMA1_Channel5;
	case 6: return DMA1_Channel6;
	case 7: return DMA1_Channel7;
	default: return 0;
	}
}

void init_DMA(uint8_t channel, uint32_t peripheralAddress, uint32_t memoryAddress, uint16_t dataSize, uint8_t priority, uint8_t transferDirection, uint8_t transferMode, uint8_t incrementMode, uint8_t peripheralTransferSize, uint8_t memoryTransferSize) {
	// Initialises and configures a DMA channel
	RCC->AHBENR |= RCC_AHBENR_DMAEN; // Enable clock for DMA controller
	DMA_Channel_TypeDef* DMACH = __dmaChannelAddress(channel); // Get the channel configuration address

	DMACH->CCR &= ~DMA_CCR_EN; // Disable the DMA channel for configuration
	DMACH->CCR &= ~DMA_CCR_MEM2MEM; // Disable memory-to-memory copy mode

	DMACH->CPAR = peripheralAddress; // Set the peripheral address
	DMACH->CMAR = memoryAddress; // Set the memory address
	DMACH->CNDTR = (uint32_t)dataSize; // Set the data size
	DMACH->CCR &= ~DMA_CCR_PL; // Reset priority bits
	DMACH->CCR |= ((priority & 0x03) << 12); // Set priority bits

	// Transfer direction
	if (transferDirection) {
		DMACH->CCR |= DMA_CCR_DIR; // Memory to peripheral
	}
	else {
		DMACH->CCR &= ~DMA_CCR_DIR; // Peripheral to memory
	}

	// Transfer mode
	if (transferMode) {
		DMACH->CCR |= DMA_CCR_CIRC; // Circular mode
	}
	else {
		DMACH->CCR &= ~DMA_CCR_CIRC; // Single transfer mode
	}

	// Increment mode
	if (incrementMode & DMA_INCREMENT_MEMORY) { // Increment memory
		DMACH->CCR |= DMA_CCR_MINC;
	}
	else {
		DMACH->CCR &= ~DMA_CCR_MINC;
	}
	if (incrementMode & DMA_INCREMENT_PERIPHERAL) { // Increment peripheral
		DMACH->CCR |= DMA_CCR_PINC;
	}
	else {
		DMACH->CCR &= ~DMA_CCR_PINC;
	}

	// Memory transaction size
	DMACH->CCR &= ~DMA_CCR_MSIZE; // Reset memory transaction size bits
	DMACH->CCR |= (memoryTransferSize << 10); // Set memory transaction size bits

	// Peripheral transaction size
	DMACH->CCR &= ~DMA_CCR_PSIZE; // Reset peripheral transaction size bits
	DMACH->CCR |= (peripheralTransferSize << 8); // Set peripheral transaction size bits

	DMACH->CCR |= DMA_CCR_EN; // Enable the DMA channel
}

void dmaChannelDisable(uint8_t channel) {
	// Disables a DMA channel
	DMA_Channel_TypeDef* DMACH = __dmaChannelAddress(channel); // Get the channel configuration address
	DMACH->CCR &= ~DMA_CCR_EN; // Disable the DMA channel for configuration
}

void dmaMemCopy(uint8_t channel, uint32_t fromAddress, uint32_t toAddress, uint16_t dataSize, uint8_t transferSize, uint8_t priority) {
	// Copies a block of memory from one location to another
	DMA_Channel_TypeDef* DMACH = __dmaChannelAddress(channel); // Get the channel configuration address
	DMACH->CCR &= ~DMA_CCR_EN; // Disable the DMA channel for configuration

	DMACH->CPAR = fromAddress; // Set the peripheral address
	DMACH->CMAR = toAddress; // Set the memory address
	DMACH->CNDTR = (uint32_t)dataSize; // Set the data size
	DMACH->CCR &= ~DMA_CCR_PL; // Reset priority bits
	DMACH->CCR |= ((priority & 0x03) << 12); // Set priority bits

	DMACH->CCR &= ~DMA_CCR_DIR; // Set direction from peripheral to memory
	DMACH->CCR &= ~DMA_CCR_CIRC; // Single transfer mode
	DMACH->CCR |= DMA_CCR_MINC; // Increment memory address
	DMACH->CCR |= DMA_CCR_PINC; // Increment peripheral address


	// Memory transaction size
	DMACH->CCR &= ~DMA_CCR_MSIZE; // Reset memory transaction size bits
	DMACH->CCR |= (transferSize << 10); // Set memory transaction size bits

	// Peripheral transaction size
	DMACH->CCR &= ~DMA_CCR_PSIZE; // Reset peripheral transaction size bits
	DMACH->CCR |= (transferSize << 8); // Set peripheral transaction size bits

	DMACH->CCR |= DMA_CCR_MEM2MEM; // Enable memory-to-memory copy mode

	DMACH->CCR |= DMA_CCR_EN; // Enable the DMA channel
}

void dmaInterruptConfig(uint8_t channel, uint8_t interruptSource, uint8_t priority) {
	// Configures DMA interrupts for a channel
	// Set the interrupt source/s in the channel configuration register
	DMA_Channel_TypeDef* DMACH = __dmaChannelAddress(channel); // Get the channel configuration address
	DMACH->CCR &= ~0x0000000E; // Disable interrupts
	DMACH->CCR |= (0x0000000E & (interruptSource << 1)); // Enable the relevant interrupt sources

	if (channel == 1) {
		// DMA Channel 1 interrupt
		nvicSetPriority(9, priority);
		nvicEnableInterrupt(9);
	}
	else if ((channel == 2) || (channel == 3)) {
		// DMA Channel 2/3 interrupt
		nvicSetPriority(10, priority);
		nvicEnableInterrupt(10);
	}
	else if ((channel == 4) || (channel == 5)) {
		// DMA Channel 4/5 interrupt
		nvicSetPriority(11, priority);
		nvicEnableInterrupt(11);
	}
}
