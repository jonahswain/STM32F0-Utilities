/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 24/04/2018
Date modified: 24/04/2018

Module: I2C


*/

/* INCLUDES */

#ifndef STM32F0_I2C_H
#include "STM32F0_I2C.h"
#define STM32F0_I2C_H
#endif

/* FUNCTIONS */

void init_I2C(I2C_TypeDef* i2cPeriph, uint8_t PSC, uint8_t SCLL, uint8_t SCLH, uint8_t SCLDEL, uint8_t SDADEL) {
	// Initialises an I2C peripheral module and configures its timings
	if (i2cPeriph == I2C1) {
		// Enable clock for I2C1
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	}
	else if (i2cPeriph == I2C2) {
		// Enable clock for I2C2
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	}
	else {
		return; // End function and do no further configuration if peripheral invalid
	}

	i2cPeriph->CR1 &= ~1; // Disable the i2c peripheral module

	i2cPeriph->TIMINGR &= 0x0F000000; // Reset the timing register
	i2cPeriph->TIMINGR |= ((PSC & 0x0F) << 28); // Set the timing prescaler
	i2cPeriph->TIMINGR |= (SCLL << 0); // Set the SCL low period
	i2cPeriph->TIMINGR |= (SCLH << 8); // Set the SCL high period
	i2cPeriph->TIMINGR |= ((SCLDEL & 0x0F) << 20); // Set the SCL delay
	i2cPeriph->TIMINGR |= ((SDADEL & 0x0F) << 16); // Set the SDA delay

	i2cPeriph->CR1 |= 1; // Enable the i2c peripheral module

	uint8_t rubbish; // A variable to store any rubbish in the received data buffer
	rubbish = i2cPeriph->RXDR; // Clear the received data buffer by reading it

}


void i2cSlaveAddress(I2C_TypeDef* i2cPeriph, int addressMode, int address) {
	// Set the address of the slave to be sent (master mode)
	if (addressMode) {
		// 10 bit addressing mode
		i2cPeriph->CR2 |= (1 << 11); // Set 10 bit addressing mode
		i2cPeriph->CR2 &= ~(0x000003FF); // Reset slave address
		i2cPeriph->CR2 |= (address & 0x000003FF); // Set slave address
	}
	else {
		// 7 bit addressing mode
		i2cPeriph->CR2 &= ~(1 << 11); // Set 7 bit addressing mode
		i2cPeriph->CR2 &= ~(0x000003FF); // Reset slave address
		i2cPeriph->CR2 |= ((address & 0x0000007F) << 1); // Set slave address
	}
}


void i2cOwnAddress(I2C_TypeDef* i2cPeriph, int addressNum, int addressMode, int address, int ackEN) {
	// Set the address of this peripheral module (slave mode)
	if (addressNum == 1) {
		// Set address 1
		if (addressMode) {
			// 10 bit addressing mode
			i2cPeriph->OAR1 |= (1 << 10); // Set 10 bit addressing mode
			i2cPeriph->OAR1 &= (0x000003FF); // Reset address bits
			i2cPeriph->OAR1 |= (address & 0x000003FF); // Set address bits
			if (ackEN) {
				i2cPeriph->OAR1 |= (1 << 15); // Enable OA1 ACK
			}
			else {
				i2cPeriph->OAR1 |= ~(1 << 15); // Enable OA1 NACK
			}
		}
		else {
			// 7 bit addressing mode
			i2cPeriph->OAR1 &= ~(1 << 10); // Set 7 bit addressing mode
			i2cPeriph->OAR1 &= (0x000003FF); // Reset address bits
			i2cPeriph->OAR1 |= ((address & 0x000007F) << 1); // Set address bits
			if (ackEN) {
				i2cPeriph->OAR1 |= (1 << 15); // Enable OA1 ACK
			}
			else {
				i2cPeriph->OAR1 |= ~(1 << 15); // Enable OA1 NACK
			}
		}
	}
	else if (addressNum == 2) {
		// Set address 2
		if (addressMode) {
			// 10 bit addressing mode
			i2cPeriph->OAR2 |= (1 << 10); // Set 10 bit addressing mode
			i2cPeriph->OAR2 &= (0x000003FF); // Reset address bits
			i2cPeriph->OAR2 |= (address & 0x000003FF); // Set address bits
			if (ackEN) {
				i2cPeriph->OAR2 |= (1 << 15); // Enable OA2 ACK
			}
			else {
				i2cPeriph->OAR2 |= ~(1 << 15); // Enable OA2 NACK
			}
		}
		else {
			// 7 bit addressing mode
			i2cPeriph->OAR2 &= ~(1 << 10); // Set 7 bit addressing mode
			i2cPeriph->OAR2 &= (0x000003FF); // Reset address bits
			i2cPeriph->OAR2 |= ((address & 0x000007F) << 1); // Set address bits
			if (ackEN) {
				i2cPeriph->OAR2 |= (1 << 15); // Enable OA2 ACK
			}
			else {
				i2cPeriph->OAR2 |= ~(1 << 15); // Enable OA2 NACK
			}
		}
	}
}


int i2cTransmitByte(I2C_TypeDef* i2cPeriph, char txByte) {
	// Transmit a byte over I2C
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to transmit)
	i2cPeriph->CR2 |= (1 << 16); // Set number of bytes to transmit to 1
	i2cPeriph->CR2 |= I2C_CR2_AUTOEND; // Enable automatic stop generation
	i2cPeriph->CR2 |= I2C_CR2_START; // Send a start condition when the bus is ready
	volatile unsigned int __timeout = I2C_TIMEOUT_LONG; // Set a timeout
	while (!(i2cPeriph->ISR & I2C_ISR_TXIS)) { // Wait until I2C start sequence transmission complete and data transmission ready
		if ((__timeout--) == 0) return 1; // End if timeout is reached
	}
	i2cPeriph->TXDR = txByte; // Place byte to send in TX data buffer
	__timeout = I2C_TIMEOUT_LONG; // Set a timeout
	while (!(i2cPeriph->ISR & I2C_ISR_STOPF)) { // Wait until stop condition is generated
		if ((__timeout--) == 0) return 1; // End if timeout is reached
	}
	i2cPeriph->ICR = I2C_ICR_STOPCF; // Clear the stop flag
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to transmit) as transmission is complete
	return ((i2cPeriph->ISR & 0x0700) >> 8); // Return Overrun error (bit 2), Arbitration lost error (bit 1), and Bus error (bit 0) flags
											 // A 0 being returned means transmission was successful
}

int i2cTransmit(I2C_TypeDef* i2cPeriph, char* txByte) {
	// Transmit a sequence of bytes over I2C
	uint8_t txLength = 0; // The number of bytes to transmit
	while (txByte[txLength]) { // Wait for null terminator
		if (txLength == 255) {
			return 1; // Return error on overflow (transmission size greater than 255)
		}
		txLength++; // Iterate through data until null terminator is reached, counting number of bytes
	}
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to transmit)
	i2cPeriph->CR2 |= (txLength << 16); // Set the number of bytes to transmit
	i2cPeriph->CR2 |= I2C_CR2_AUTOEND; // Enable automatic stop generation
	i2cPeriph->CR2 |= I2C_CR2_START; // Send a start condition when the bus is ready
	volatile unsigned int __timeout = I2C_TIMEOUT_LONG; // Set a timeout
	while (!(i2cPeriph->ISR & I2C_ISR_TXIS)) { // Wait until I2C start sequence transmission complete and data transmission ready
		if ((__timeout--) == 0) return 1; // End if timeout is reached
	}
	for (int i = 0; i <= txLength; i++) {
		// Transmit sequence of bytes
		__timeout = I2C_TIMEOUT_LONG; // Set the timeout
		while (!(i2cPeriph->ISR & 1)) { // Wait for TX buffer empty flag before transmitting next byte
			if ((__timeout--) == 0) return 1; // End if timeout reached
		}
		i2cPeriph->TXDR = txByte[i]; // Load the current byte into the TX data buffer
									 // TODO: Add NACK/error detection
	}
	i2cPeriph->ICR = I2C_ICR_STOPCF; // Clear the stop flag
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to transmit) as transmission is complete
	return ((i2cPeriph->ISR & 0x0700) >> 8); // Return Overrun error (bit 2), Arbitration lost error (bit 1), and Bus error (bit 0) flags
											 // A 0 being returned means transmission was (probably) successful
}

uint8_t i2cReadByte(I2C_TypeDef* i2cPeriph) {
	// Reads a single byte from an I2C interface
	uint8_t rxByte;

	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to transmit)
	i2cPeriph->CR2 |= (1 << 16); // Set number of bytes to transmit to 1
	i2cPeriph->CR2 |= I2C_CR2_AUTOEND; // Enable automatic stop generation
	i2cPeriph->CR2 |= I2C_CR2_START; // Send a start condition when the bus is ready
	volatile unsigned int __timeout = I2C_TIMEOUT_LONG; // Set a timeout
	while (!(i2cPeriph->ISR & I2C_ISR_RXNE)) { // Wait until data is ready in RX data register
		if ((__timeout--) == 0) return 0; // End if timeout is reached
	}
	rxByte = (uint8_t)i2cPeriph->RXDR; // Read the received data
	__timeout = I2C_TIMEOUT_LONG; // Set a timeout
	while (!(i2cPeriph->ISR & I2C_ISR_STOPF)) { // Wait until stop condition is generated
		if ((__timeout--) == 0) return 0; // End if timeout is reached
	}
	i2cPeriph->ICR = I2C_ICR_STOPCF; // Clear the stop flag
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to receive)
	return rxByte; // Return the received data

}

void i2cRead(I2C_TypeDef* i2cPeriph, char* data, uint8_t dataSize) {
	// Reads in data from an I2C interface to a location in memory (data)
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to receive)
	i2cPeriph->CR2 |= (dataSize << 16); // Set the number of bytes to receive
	i2cPeriph->CR2 |= I2C_CR2_AUTOEND; // Enable automatic stop generation
	i2cPeriph->CR2 |= I2C_CR2_START; // Send a start condition when the bus is ready
	volatile unsigned int __timeout = I2C_TIMEOUT_LONG; // Set a timeout
	while (!(i2cPeriph->ISR & I2C_ISR_RXNE)) { // Wait until data is ready in RX data register
		if ((__timeout--) == 0) return; // End if timeout is reached
	}
	for (int i = 0; i < dataSize; i++) {
		// Transmit sequence of bytes
		__timeout = I2C_TIMEOUT_LONG; // Set the timeout
		while (!(i2cPeriph->ISR & I2C_ISR_RXNE)) { // Wait for RX buffer data ready flag
			if ((__timeout--) == 0) return; // End if timeout reached
		}
		data[i] = i2cPeriph->RXDR; // Read the current byte in from the buffer
	}
	while (!(i2cPeriph->ISR & I2C_ISR_STOPF)) { // Wait until stop condition is generated
		if ((__timeout--) == 0) return; // End if timeout is reached
	}
	i2cPeriph->ICR = I2C_ICR_STOPCF; // Clear the stop flag
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to receive)
}

void i2cReadFromSlave(I2C_TypeDef* i2cPeriph, char* data, uint8_t dataSize, int addressMode, int slaveReadAddress, int slaveWriteAddress, uint8_t addressToRead) {
	// Reads data from a slave
	i2cSlaveAddress(i2cPeriph, addressMode, slaveWriteAddress); //Set the slave address to the writing to slave address
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to transmit)
	i2cPeriph->CR2 |= (1 << 16); // Set to transmit 1 byte
	i2cPeriph->CR2 &= ~I2C_CR2_AUTOEND; // Disable automatic end generation
	i2cPeriph->CR2 |= I2C_CR2_START; // Send a start condition when the bus is ready
	volatile unsigned int __timeout = I2C_TIMEOUT_LONG; // Set the timeout
	while (!(i2cPeriph->ISR & I2C_ISR_TXIS)) { // Wait until I2C start sequence transmission complete and data transmission ready
		if ((__timeout--) == 0) return; // End if timeout is reached
	}
	i2cPeriph->TXDR = addressToRead; // Place address into transmit buffer
	__timeout = I2C_TIMEOUT_LONG;
	while (!(i2cPeriph->ISR & I2C_ISR_TC)) { // Wait for transmission to complete
		if ((__timeout--) == 0) return; // End if timeout is reached
	}
	i2cSlaveAddress(i2cPeriph, addressMode, slaveReadAddress); // Set the slave address to the reading from slave address
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to receive)
	i2cPeriph->CR2 |= (dataSize << 16); // Set to receive dataSize bytes
	i2cPeriph->CR2 |= I2C_CR2_AUTOEND; // Enable automatic end generation
	i2cPeriph->CR2 |= I2C_CR2_RD_WRN; // Tell the I2C peripheral module that we have requested a read
	i2cPeriph->CR2 |= I2C_CR2_START; // Send a start condition when the bus is ready
	for (int i = 0; i < dataSize; i++) {
		__timeout = I2C_TIMEOUT_LONG; // Set the timeout
		while (!(i2cPeriph->ISR & I2C_ISR_RXNE)) { // Wait for RX buffer data ready flag
			if ((__timeout--) == 0) return; // End if timeout reached
		}
		data[i] = i2cPeriph->RXDR; // Read the current byte in from the buffer
	}
	while (!(i2cPeriph->ISR & I2C_ISR_STOPF)) { // Wait until stop condition is generated
		if ((__timeout--) == 0) return; // End if timeout is reached
	}
	i2cPeriph->ICR = I2C_ICR_STOPCF; // Clear the stop flag
	i2cPeriph->CR2 &= 0xFF00FFFF; // Clear NBYTES (number of bytes to receive)
	i2cPeriph->CR2 &= ~I2C_CR2_RD_WRN; // Reset RD_WRN
}
