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
void init_DMA(uint8_t channel, uint32_t peripheralAddress, uint32_t memoryAddress, uint16_t dataSize, uint8_t priority, uint8_t transferDirection, uint8_t transferMode, uint8_t incrementMode, uint8_t peripheralTransferSize, uint8_t memoryTransferSize) {
	// Initialises and configures a DMA channel
}

void dmaChannelDisable(uint8_t channel) {
	// Disables a DMA channel
}

void dmaMemCopy(uint8_t channel, uint32_t fromAddress, uint32_t toAddress, uint16_t dataSize, uint8_t transferSize, uint8_t priority) {
	// Copies a block of memory from one location to another
}

void dmaInterruptEnable(uint8_t channel, uint8_t interruptSource, uint8_t priority) {
	// Enables DMA interrupts for a channel
}
