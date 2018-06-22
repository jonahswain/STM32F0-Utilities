#pragma once
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

#ifndef STM32F0_DMA_H
#include "STM32F0_DMA.h"
#define STM32F0_DMA_H
#endif

#ifndef STM32F0_OTHER_H
#include "STM32F0_OTHER.h"
#define STM32F0_OTHER_H
#endif

/* CONSTANT DEFINITIONS */
#define USART_CR1_M0 0x10000000
#define USART_CR1_M1 0x00001000

#define USART_OVERSAMPLING_16 0
#define USART_OVERSAMPLING_8 1

#define USART_FRAMESIZE_8BIT 0x0
#define USART_FRAMESIZE_9BIT 0x1
#define USART_FRAMESIZE_7BIT 0x2 // STM32F07x only

#define USART_FRAMEFORMAT_LSBFIRST 0
#define USART_FRAMEFORMAT_MSBFIRST 1

#define USART_TXEN 1
#define USART_RXEN 1

#define USART_STOPBITS_1 0x0
#define USART_STOPBITS_2 0x2
#define USART_STOPBITS_1_5 0x3 // 1.5 stop bits

#define USART_PARITY_ENABLE 0x1
#define USART_PARITY_DISABLE 0x0
#define USART_PARITY_EVEN 0x0
#define USART_PARITY_ODD 0x2

#define USART_INVERSION_DATA 0x1
#define USART_INVERSTION_TRANSMIT 0x2
#define USART_INVERSION_RECEIVE 0x4

#define USART_SWAPTXRX 1

#define USART_FULLDUPLEX 0
#define USART_HALFDUPLEX 1

#define USART_CLOCK_ENABLE 0x1
#define USART_CLOCK_POLARITY 0x2
#define USART_CLOCK_POLARITY_IDLELOW 0x0
#define USART_CLOCK_POLARITY_IDLEHIGH 0x2
#define USART_CLOCK_PHASE 0x4
#define USART_CLOCK_PHASE_1STCLOCK 0x0
#define USART_CLOCK_PHASE_2NDCLOCK 0x4
#define USART_CLOCK_LASTBITCLOCK 0x8
#define USART_CLOCK_LASTBITCLOCK_DISABLE 0x0
#define USART_CLOCK_LASTBITCLOCK_ENABLE 0x8

#define USART_DRIVER_ENABLE 0x1
#define USART_DRIVER_POLARITY 0x2

// Address/character recognition
#define USART_ADDREC_INTERRUPT_ENABLE 0x1
#define USART_ADDRES_INTERRUPT_DISABLE 0x0
#define USART_ADDREC_4BIT 0x0
#define USART_ADDREC_7BIT 0x2

/* FUNCTIONS */
void init_USART(USART_TypeDef* USARTperiph, uint32_t BAUD, uint8_t oversampling, uint8_t frameSize, uint8_t frameFormat, uint8_t TXEN, uint8_t RXEN, uint8_t stopBits, uint8_t parityConfiguration, uint8_t inversionConfiguration, uint8_t swapTXRX, uint8_t halfDuplexMode, uint8_t clockConfiguration, uint8_t driverConfiguration, uint8_t driverEAT, uint8_t driverEDT);
/*
Initialises a USART peripheral module

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

void init_USART_serial(USART_TypeDef* USARTperiph, uint32_t BAUD, uint8_t oversampling, uint8_t frameSize, uint8_t frameFormat, uint8_t stopBits, uint8_t parityConfiguration, uint8_t inversionConfiguration, uint8_t swapTXRX);
/*
Initialises a USART peripheral module for serial communication

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

void usartSetBAUD(USART_TypeDef* USARTperiph, uint32_t BAUD); // Sets the BAUD rate of a USART peripheral module (based on a 48MHz system clock)
void usartAutoBAUDDetection(USART_TypeDef* USARTperiph); // Automatically detects the BAUD rate on a USART peripheral module (See reference manual for modes, if in doubt, use mode 0)
void usartConfigureAddressRecognition(USART_TypeDef* USARTperiph, uint8_t address, uint8_t addressRecognitionConfig); // Configures address/character recognition on a USART peripheral module
void usartSetAddress(USART_TypeDef* USARTperiph, uint8_t address); // Sets the address of a USART peripheral module
uint8_t usartRXNE(USART_TypeDef* USARTperiph); // Returns whether the USART peripheral module has data in its RX buffer
uint8_t usartGetData(USART_TypeDef* USARTperiph); // Returns any data received over USART
uint8_t usartReceiveData(USART_TypeDef* USARTperiph); // Waits for data from USART before returning it
void usartTransmitData(USART_TypeDef* USARTperiph, uint8_t data); // Sends a data frame over USART
void usartDMATransmitData(USART_TypeDef* USARTperiph, uint8_t* data, uint16_t dataSize); // Sends a block of data over USART using DMA