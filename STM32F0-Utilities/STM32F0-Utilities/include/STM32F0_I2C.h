#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 24/04/2018
Date modified: 24/04/2018

Module: I2C
Functions for using the I2C peripheral modules

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
#define I2C_7BIT_ADDRESSING 0
#define I2C_10BIT_ADDRESSING 1

#define I2C_TIMEOUT_LONG 50000

/* FUNCTIONS */

void init_I2C(I2C_TypeDef* i2cPeriph, uint8_t PSC, uint8_t SCLL, uint8_t SCLH, uint8_t SCLDEL, uint8_t SDADEL); // Initialises an I2C peripheral module and configures its timings
/*
PSC - Timer prescaler
SCLL - SCL low period
SCLH - SCL high period
SCLDEL - Data setup time - delay between SDA edge and SCL rising edge
SDADEL - Data hold time - delay between SCL falling edge and SDA edge
Configure these based on the device being interfaced with
*/

void i2cSlaveAddress(I2C_TypeDef* i2cPeriph, int addressMode, int address); // Set the address of the slave to be sent (master mode)
/*
addressMode - 7 or 10 bit addressing
address - The slave's address
*/

void i2cOwnAddress(I2C_TypeDef* i2cPeriph, int addressNum, int addressMode, int address, int ackEN); // Set the address of this peripheral module (slave mode)
/*
addressNum - The number of the address to configure (the I2C peripheral modules can have 2 addresses)
addressMode - 7 or 10 bit addressing
address - The peripheral module's address
ackEN - whether to ACK the received slave address
*/

int i2cTransmitByte(I2C_TypeDef* i2cPeriph, char txByte); // Transmit a byte over I2C - returns 0 if successful
int i2cTransmit(I2C_TypeDef* i2cPeriph, char* txByte); // Transmit a sequence of bytes over I2C - returns 0 if successful

uint8_t i2cReadByte(I2C_TypeDef* i2cPeriph); // Reads a single byte from an I2C interface
void i2cRead(I2C_TypeDef* i2cPeriph, char* data, uint8_t dataSize); // Reads in data from an I2C interface to a location in memory (data)

void i2cReadFromSlave(I2C_TypeDef* i2cPeriph, char* data, uint8_t dataSize, int addressMode, int slaveReadAddress, int slaveWriteAddress, uint8_t addressToRead); // Reads data from a slave
/*
data - the location in memory to place the received data
dataSize - the expected amount of data to be received
addressMode - 7 or 10 bit addressing
slaveReadAddress - Address of the slave when being read from
slaveWriteAddress - Address of the slave to write/send to
addressToRead - Memory address of the slave to read from
*/