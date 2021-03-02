#pragma once
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

#define SPI_TIMEOUT_LONG 50000 // Timeout waiting for data

// Common BAUD rates (Assuming eclipse default 48MHz fpclk)
#define SPI_BAUD_6MHZ 0x2 // fpclk/8
#define SPI_BAUD_3MHZ 0x3 // fpclk/16
#define SPI_BAUD_750KHZ 0x5 // fpclk/64
#define SPI_BAUD_187KHZ 0x7 // fpclk/256

#define SPI_MASTER_MODE 1
#define SPI_SLAVE_MODE 0
#define SPI_MSB_FIRST 0
#define SPI_LSB_FIRST 1
#define SPI_FRAMESIZE_8BIT 0x7
#define SPI_FRAMESIZE_16BIT 0xF

// Common data transfer modes
#define SPI_BIDIMODE 0x4
#define SPI_BIDIOE 0x2
#define SPI_RXONLY 0x1

#define SPI_MULTIMASTER_ENABLE 0
#define SPI_MULTIMASTER_DISABLE 1

// Common hardware CRC configurations
#define SPI_CRCEN 0x1
#define SPI_CRCL 0x2

#define SPI_RXNE_8BIT 1
#define SPI_RXNE_16BIT 0

/* FUNCTIONS */
void init_SPI(SPI_TypeDef* SPIperiph, uint8_t BAUD, int masterMode, int frameFormat, uint8_t dataSize, int dataTransferMode, int multiMasterMode, int clockPolarity, int clockPhase, int crcMode, int rxThreshold); // Initialises and configures an SPI peripheral module
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

void __spiFlushRXBuffer(SPI_TypeDef* SPIperiph); // Clears any junk out of the SPI RX FIFO buffers

void spiTransmitFrame(SPI_TypeDef* SPIperiph, uint16_t data); // Transmits a frame of data over SPI
uint16_t spiReceiveFrame(SPI_TypeDef* SPIperiph); // Received a frame of data over SPI (sends a junk frame to get a frame)
uint16_t spiGetData(SPI_TypeDef* SPIperiph); // Gets the last received data frame