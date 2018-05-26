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
}

void analogWrite(uint8_t channel, uint16_t value) {
	// Outputs an analog value using the DAC
}

void dacWaveOut(uint8_t channel, uint16_t* values, uint16_t length) {
	// Outputs a series of analog values in sequence using the DAC
}

void dacWaveGen(uint8_t channel, uint16_t* values, uint16_t length, uint32_t repetitions) {
	// Outputs a series of analog values multiple times using the DAC
}

void dacDMAWaveGen(uint8_t channel, uint16_t* values, uint16_t length, uint16_t period) {
	// Enables waveform generation using DMA (repeats forever...)
}

void dacDMAWaveGenDisable(uint8_t channel) {
	// Disables waveform generation using DMA
}