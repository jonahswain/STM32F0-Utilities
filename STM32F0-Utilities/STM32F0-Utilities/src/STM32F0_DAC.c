/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 26/05/2018
Date modified: 26/05/2018

Module: DAC
Functions for operating the Digital to Analog converter

*/

/* INCLUDES */

#ifndef STM32F0_DAC_H
#include "STM32F0_DAC.h"
#define STM32F0_DAC_H
#endif

/* FUNCTIONS */
void init_DAC(uint8_t channel, uint8_t buffer, uint8_t triggerEnable, uint8_t triggerSource) {
	// Initialises and configures  DAC channel
	RCC->APB1ENR |= RCC_APB1ENR_DACEN; // Enable clock for DAC
	if (channel == 1) {
		// Configure channel 1
		DAC->CR &= ~DAC_CR_EN1; // Disable CH1 for configuration

		if (buffer) { // Configure buffer
			DAC->CR &= ~DAC_CR_BOFF1; // Enable buffer
		}
		else {
			DAC->CR |= DAC_CR_BOFF1; // Disable buffer
		}

		if (triggerEnable) { // Configure trigger
			DAC->CR |= DAC_CR_TEN1; // Enable trigger
		}
		else {
			DAC->CR &= ~DAC_CR_TEN1; // Disable trigger
		}

		DAC->CR &= ~DAC_CR_TSEL1; // Clear trigger selection bits
		DAC->CR |= (DAC_CR_TSEL1 & (triggerSource << 3)); // Set trigger selection bits

		DAC->CR |= DAC_CR_EN1; // Enable CH1

	}
	else if (channel == 2) {
		// Configure channel 2
		DAC->CR &= ~DAC_CR_EN2; // Disable CH2 for configuration

		if (buffer) { // Configure buffer
			DAC->CR &= ~DAC_CR_BOFF2; // Enable buffer
		}
		else {
			DAC->CR |= DAC_CR_BOFF2; // Disable buffer
		}

		if (triggerEnable) { // Configure trigger
			DAC->CR |= DAC_CR_TEN2; // Enable trigger
		}
		else {
			DAC->CR &= ~DAC_CR_TEN2; // Disable trigger
		}

		DAC->CR &= ~DAC_CR_TSEL2; // Clear trigger selection bits
		DAC->CR |= (DAC_CR_TSEL2 & (triggerSource << 19)); // Set trigger selection bits

		DAC->CR |= DAC_CR_EN2; // Enable CH2
	}

	__cpuHoldDelay(10); // Wait 10 uS for DAC to power on
}

void analogWrite(uint8_t channel, uint8_t value) {
	// Outputs an 8 bit analog value using the DAC
	if (channel == 1) {
		// Write to CH1
		DAC->DHR8R1 = value;
	}
	else if (channel == 2) {
		// Write to CH2
		DAC->DHR8R2 = value;
	}
}

void dacValueOut(uint8_t channel, uint16_t value, uint8_t mode) {
	// Outputs an analog value using the DAC
	if (channel == 1) {
		// Write to CH1
		if (!(mode & DAC_MODE_RESOLUTION)) {
			// 8 bit mode
			DAC->DHR8R1 = (0x00FF & value);
		}
		else {
			if (mode & DAC_MODE_DATAALIGNMENT) {
				// 12 bit left aligned mode
				DAC->DHR12L1 = (0xFFF0 & value);
			}
			else {
				// 12 bit right aligned mode
				DAC->DHR12R1 = (0x0FFF & value);
			}
		}
	}
	else if (channel == 2) {
		// Write to CH2
		if (!(mode & DAC_MODE_RESOLUTION)) {
			// 8 bit mode
			DAC->DHR8R2 = (0x00FF & value);
		}
		else {
			if (mode & DAC_MODE_DATAALIGNMENT) {
				// 12 bit left aligned mode
				DAC->DHR12L2 = (0xFFF0 & value);
			}
			else {
				// 12 bit right aligned mode
				DAC->DHR12R2 = (0x0FFF & value);
			}
		}
	}
}

void dacWaveOut(uint8_t channel, uint16_t* values, uint8_t mode, uint16_t length) {
	// Outputs a series of analog values in sequence using the DAC
}

void dacWaveGen(uint8_t channel, uint16_t* values, uint8_t mode, uint16_t length, uint32_t repetitions) {
	// Outputs a series of analog values multiple times using the DAC
}

void dacDMAWaveGen(uint8_t channel, uint16_t* values, uint8_t mode, uint16_t length, uint16_t period) {
	// Enables waveform generation using DMA (repeats forever...)
}

void dacDMAWaveGenDisable(uint8_t channel) {
	// Disables waveform generation using DMA
}
