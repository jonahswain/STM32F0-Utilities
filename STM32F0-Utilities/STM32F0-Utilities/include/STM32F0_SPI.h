#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 04/05/2018
Date modified: 04/05/2018

Module: SPI
Functions for using the Serial Peripheral Interface peripheral module

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
// Common BAUD rates


#define SPI_MASTER_MODE 1
#define SPI_SLAVE_MODE 0
#define SPI_MSB_FIRST 0
#define SPI_LSB_FIRST 1
#define SPI_FRAMESIZE_8BIT 0x7
#define SPI_FRAMESIZE_16BIT 0xF

// Common data transfer modes
// TODO

#define SPI_MULTIMASTER_ENABLE 0
#define SPI_MULTIMASTER_DISABLE 1

// Common hardware CRC configurations
// TODO

#define SPI_RXNE_8BIT 1
#define SPI_RXNE_16BIT 0

/* FUNCTIONS */
void init_SPI(SPI_TypeDef* SPIperiph, int BAUD, int masterMode, int frameFormat, int dataSize, int dataTransferMode, int multiMasterMode, int clockPolarity, int clockPhase, int crcMode, int rxThreshold); // Initialises and configures an SPI peripheral module
/*
SPIperiph - the SPI interface to initialise and configure
BAUD - the fpclk scaler to use for the BAUD rate (pg 754 of reference manual)
masterMode - 0: Slave mode 1: Master mode
frameFormat - 0: MSB first 1: LSB first (order in which data is transmitted/recieved)
dataSize - The number of bits in a data frame (pg 757 of reference manual)
dataTransferMode - 2/1 line operation, uni/bidirectional data transfer, receive only mode (RXONLY, BIDIMODE, BIDIOE)
multiMasterMode - 0: Allow multi-master mode 1: Disable multi-master mode
clockPolarity - 0: CK LOW when idle 1: CK HIGH when idle
clockPhase - 0: 1st clock transition is 1st data capture edge 1: 2nd clock transition is 1st data capture edge
crcMode - Hardware CRC calculation enable/configuration
rxThreshold - 0: RXNE on 16 bits received 1: RXNE on 8 bits received
*/

void spiTransmitFrame(uint16_t data); // Transmits a frame of data over SPI
void spiTransmit(int* data); // Transmits a series of frames over SPI (until the null terminator -1 is reached)
uint16_t spiReceiveFrame(); // Gets a received from SPI
// TODO add a suitable receive data method