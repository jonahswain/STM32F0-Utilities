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
	if (channel == 1) {
		// Write to CH1
		if (!(mode & DAC_MODE_RESOLUTION)) {
			// 8 bit mode
			for (uint16_t i = 0; i < length; i++) {
				DAC->DHR8R1 = (0x00FF & values[i]);
			}
		}
		else {
			if (mode & DAC_MODE_DATAALIGNMENT) {
				// 12 bit left aligned mode
				for (uint16_t i = 0; i < length; i++) {
					DAC->DHR12L1 = (0xFFF0 & values[i]);
				}
			}
			else {
				// 12 bit right aligned mode
				for (uint16_t i = 0; i < length; i++) {
					DAC->DHR12R1 = (0x0FFF & values[i]);
				}
			}
		}
		DAC->DHR12R1 = 0; // Set back to 0 when complete
	}
	else if (channel == 2) {
		// Write to CH2
		if (!(mode & DAC_MODE_RESOLUTION)) {
			// 8 bit mode
			for (uint16_t i = 0; i < length; i++) {
				DAC->DHR8R2 = (0x00FF & values[i]);
			}
		}
		else {
			if (mode & DAC_MODE_DATAALIGNMENT) {
				// 12 bit left aligned mode
				for (uint16_t i = 0; i < length; i++) {
					DAC->DHR12L2 = (0xFFF0 & values[i]);
				}
			}
			else {
				// 12 bit right aligned mode
				for (uint16_t i = 0; i < length; i++) {
					DAC->DHR12R2 = (0x0FFF & values[i]);
				}
			}
		}
		DAC->DHR12R2 = 0; // Set back to 0 when complete
	}
}

void dacWaveGen(uint8_t channel, uint16_t* values, uint8_t mode, uint16_t length, uint32_t repetitions) {
	// Outputs a series of analog values multiple times using the DAC
	if (channel == 1) {
		// Write to CH1
		if (!(mode & DAC_MODE_RESOLUTION)) {
			// 8 bit mode
			for (; repetitions > 0; repetitions--) {
				for (uint16_t i = 0; i < length; i++) {
					DAC->DHR8R1 = (0x00FF & values[i]);
				}
			}
		}
		else {
			if (mode & DAC_MODE_DATAALIGNMENT) {
				// 12 bit left aligned mode
				for (; repetitions > 0; repetitions--) {
					for (uint16_t i = 0; i < length; i++) {
						DAC->DHR12L1 = (0xFFF0 & values[i]);
					}
				}
			}
			else {
				// 12 bit right aligned mode
				for (; repetitions > 0; repetitions--) {
					for (uint16_t i = 0; i < length; i++) {
						DAC->DHR12R1 = (0x0FFF & values[i]);
					}
				}
			}
		}
		DAC->DHR12R1 = 0; // Set back to 0 when complete
	}
	else if (channel == 2) {
		// Write to CH2
		if (!(mode & DAC_MODE_RESOLUTION)) {
			// 8 bit mode
			for (; repetitions > 0; repetitions--) {
				for (uint16_t i = 0; i < length; i++) {
					DAC->DHR8R2 = (0x00FF & values[i]);
				}
			}
		}
		else {
			if (mode & DAC_MODE_DATAALIGNMENT) {
				// 12 bit left aligned mode
				for (; repetitions > 0; repetitions--) {
					for (uint16_t i = 0; i < length; i++) {
						DAC->DHR12L2 = (0xFFF0 & values[i]);
					}
				}
			}
			else {
				// 12 bit right aligned mode
				for (; repetitions > 0; repetitions--) {
					for (uint16_t i = 0; i < length; i++) {
						DAC->DHR12R2 = (0x0FFF & values[i]);
					}
				}
			}
		}
		DAC->DHR12R2 = 0; // Set back to 0 when complete
	}
}


/*
NOTE: Uses DMA channel 2 and TIM17 for DAC channel 1 and DMA channel 3 and TIM16 for DAC channel 2
channel - the DAC channel to output on
values - a pointer to an array of analog values to output
mode - 8/12 bit, left/right aligned
length - the number of values in the array
period - the approximate delay between values (in microseconds)
*/
void dacDMAWaveGen(uint8_t channel, uint16_t* values, uint8_t mode, uint16_t length, uint16_t period) {
	// Enables waveform generation using DMA (repeats forever...)
	uint32_t peripheralAddress;
	uint32_t memoryAddress = (uint32_t)values; // Address of waveform values in memory

	if (channel == 1) {
		// Configure channel 1
		if (!(mode & DAC_MODE_RESOLUTION)) {
			// 8 bit mode
			peripheralAddress = (uint32_t)(&DAC->DHR8R1);
		}
		else {
			if (mode & DAC_MODE_DATAALIGNMENT) {
				// 12 bit left aligned mode
				peripheralAddress = (uint32_t)(&DAC->DHR12L1);
			}
			else {
				// 12 bit right aligned mode
				peripheralAddress = (uint32_t)(&DAC->DHR12R1);
			}
		}
		init_DMA(2, peripheralAddress, memoryAddress, length, DMA_PRIORITY_LOW, DMA_TRANSFERDIRECTION_M2P, DMA_TRANSFERMODE_CIRCULAR, DMA_INCREMENT_MEMORY, DMA_TRANSFERSIZE_HALFWORD, DMA_TRANSFERSIZE_HALFWORD); // Initialise DMA
		init_timer(TIM17, 47); // Initialise timer 17 to tick every uS
		startRepeatingTimer(TIM17, period - 1); // Set to overflow/update every period - 1 uS
		TIM17->CR2 |= TIM_CR2_CCDS; // Change DMA request to send on update
		TIM17->DIER |= TIM_DIER_UDE; // Enable DMA request generation
		SYSCFG->CFGR1 |= SYSCFG_CFGR1_TIM17_DMA_RMP; // Remap TIM17 to trigger DMA
	}
	else if (channel == 2) {
		// Configure channel 2
		if (!(mode & DAC_MODE_RESOLUTION)) {
			// 8 bit mode
			peripheralAddress = (uint32_t)(&DAC->DHR8R2);
		}
		else {
			if (mode & DAC_MODE_DATAALIGNMENT) {
				// 12 bit left aligned mode
				peripheralAddress = (uint32_t)(&DAC->DHR12L2);
			}
			else {
				// 12 bit right aligned mode
				peripheralAddress = (uint32_t)(&DAC->DHR12R2);
			}
		}
		init_DMA(3, peripheralAddress, memoryAddress, length, DMA_PRIORITY_LOW, DMA_TRANSFERDIRECTION_M2P, DMA_TRANSFERMODE_CIRCULAR, DMA_INCREMENT_MEMORY, DMA_TRANSFERSIZE_HALFWORD, DMA_TRANSFERSIZE_HALFWORD); // Initialise DMA
		init_timer(TIM16, 47); // Initialise timer 17 to tick every uS
		startRepeatingTimer(TIM16, period - 1); // Set to overflow/update every period uS
		TIM16->CR2 |= TIM_CR2_CCDS; // Change DMA request to send on update
		TIM16->DIER |= TIM_DIER_UDE; // Enable DMA request generation
	}
}

void dacDMAWaveGenDisable(uint8_t channel) {
	// Disables waveform generation using DMA
	if (channel == 1) {
		// Stop wave gen on CH1
		stopTimer(TIM17); // Stop the timer
		TIM17->DIER &= ~TIM_DIER_UDE; // Disable DMA request generation
		dmaChannelDisable(2); // Disable the DMA channel
		SYSCFG->CFGR1 &= ~SYSCFG_CFGR1_TIM17_DMA_RMP; // Reset TIM17 mapping
	}
	else if (channel == 2) {
		// Stop wave gen on CH2
		stopTimer(TIM16); // Stop the timer
		TIM16->DIER &= ~TIM_DIER_UDE; // Disable DMA request generation
		dmaChannelDisable(3); // Disable the DMA channel
	}
}
