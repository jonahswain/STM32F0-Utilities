#pragma once
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

#ifndef STM32F0XX_H
#include "stm32f0xx.h"
#define STM32F0XX_H
#endif

#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif

#ifndef STM32F0_GPIO_H
#include "STM32F0_GPIO.h"
#define STM32F0_GPIO_H
#endif

#ifndef STM32F0_OTHER_H
#include "STM32F0_OTHER.h"
#define STM32F0_OTHER_H
#endif

/* CONSTANT DEFINITIONS */
// Priority levels
#define DMA_PRIORITY_LOW 0
#define DMA_PRIORITY_MEDIUM 1
#define DMA_PRIORITY_HIGH 2
#define DMA_PRIORITY_VHIGH 3

// Transfer directions
#define DMA_TRANSFERDIRECTION_P2M 0 // Peripheral to memory
#define DMA_TRANSFERDIRECTION_M2P 1 // Memory to peripheral

// Transfer modes
#define DMA_TRANSFERMODE_SINGLE 0
#define DMA_TRANSFERMODE_CIRCULAR 1

// Increment modes
#define DMA_INCREMENT_MEMORY 0x1
#define DMA_INCREMENT_PERIPHERAL 0x2

// Transfer sizes
#define DMA_TRANSFERSIZE_BYTE 0
#define DMA_TRANSFERSIZE_HALFWORD 1
#define DMA_TRANSFERSIZE_WORD 2


/* FUNCTIONS */
void init_DMA(uint8_t channel, uint32_t peripheralAddress, uint32_t memoryAddress, uint16_t dataSize, uint8_t priority, uint8_t transferDirection, uint8_t transferMode, uint8_t incrementMode, uint8_t peripheralTransferSize, uint8_t memoryTransferSize); // Initialises and configures a DMA channel
/*
channel - the DMA channel to configure
peripheralAddress - the address of the peripheral register to transfer to/from
memoryAddress - the address in memory to transfer to/from
dataSize - the number of data to be transferred
priority - the channel priority
transferDirection - 0: Transfer from peripheral to memory 1: Transfer from memory to peripheral
transferMode - 0: Single transfer (channel stop after full transfer complete) 1: Circular mode (channel restart after full transfer complete)
incrementMode - MINC / PINC (Increments pointer/memory address between transfers
peripheralTransferSize - peripheral transaction memory size
memoryTransferSize - memory transaction memory size
*/

void dmaChannelDisable(uint8_t channel); // Disables a DMA channel

void dmaMemCopy(uint8_t channel, uint32_t fromAddress, uint32_t toAddress, uint16_t dataSize, uint8_t transferSize, uint8_t priority); // Copies a block of memory from one location to another

void dmaInterruptEnable(uint8_t channel, uint8_t interruptSource, uint8_t priority); // Enables DMA interrupts for a channel
