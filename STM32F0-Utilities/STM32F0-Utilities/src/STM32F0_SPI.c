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

#ifndef STM32F0_SPI_H
#include "STM32F0_SPI.h"
#define STM32F0_SPI_H
#endif

/* FUNCTIONS */
void init_SPI(SPI_TypeDef* SPIperiph, int BAUD, int masterMode, int frameFormat, int dataSize, int dataTransferMode, int multiMasterMode, int clockPolarity, int clockPhase, int crcMode, int rxThreshold); // Initialises and configures an SPI peripheral module

void spiTransmitFrame(uint16_t data); // Transmits a frame of data over SPI
void spiTransmit(int* data); // Transmits a series of frames over SPI (until the null terminator -1 is reached)
uint16_t spiReceiveFrame(); // Gets a received from SPI