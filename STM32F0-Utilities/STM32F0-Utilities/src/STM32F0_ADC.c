/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 17/04/2018
Date modified: 01/05/2018

Module: ADC

*/

/* INCLUDES */

#ifndef STM32F0_ADC_H
#include "STM32F0_ADC.h"
#define STM32F0_ADC_H
#endif


/* FUNCTIONS */

void init_ADC(int resolution) {
	// Initialise and calibrate the ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN; // Enable clock for the ADC
	ADC1->CR |= ADC_CR_ADCAL; // Start ADC calibration
	while ((ADC1->CR & ADC_CR_ADCAL); // Wait for calibration to complete
	ADC1->CR |= ADC_CR_ADEN; // Enable the ADC
	while (!(ADC1->ISR & ADC_ISR_ADRDY)); // Wait for ADC to be ready
	if ((resolution >= 0) && (resolution <= 3)) {
		// Check if resolution is valid
		ADC1->CFGR1 |= (resolution << 3); // Set the ADC resolution
	}
}

uint16_t analogRead(IOPin_TypeDef* iopin) {
	// Read an analog value from a pin
	ADC1->CHSELR &= 0xFFF80000; // Reset the channel selection
	if ((iopin->port == GPIOA) && (iopin->pin == 0)) { // PA0
		ADC1->CHSELR |= (1 << 0); // Select channel 0
	}
	else if ((iopin->port == GPIOA) && (iopin->pin == 1)) { // PA1
		ADC1->CHSELR |= (1 << 1); // Select channel 1
	}
	else if ((iopin->port == GPIOA) && (iopin->pin == 2)) { // PA2
		ADC1->CHSELR |= (1 << 2); // Select channel 2
	}
	else if ((iopin->port == GPIOA) && (iopin->pin == 3)) { // PA3
		ADC1->CHSELR |= (1 << 3); // Select channel 3
	}
	else if ((iopin->port == GPIOA) && (iopin->pin == 4)) { // PA4
		ADC1->CHSELR |= (1 << 4); // Select channel 4
	}
	else if ((iopin->port == GPIOA) && (iopin->pin == 5)) { // PA5
		ADC1->CHSELR |= (1 << 5); // Select channel 5
	}
	else if ((iopin->port == GPIOA) && (iopin->pin == 6)) { // PA6
		ADC1->CHSELR |= (1 << 6); // Select channel 6
	}
	else if ((iopin->port == GPIOA) && (iopin->pin == 7)) { // PA7
		ADC1->CHSELR |= (1 << 7); // Select channel 7
	}
	else if ((iopin->port == GPIOB) && (iopin->pin == 0)) { // PB0
		ADC1->CHSELR |= (1 << 8); // Select channel 8
	}
	else if ((iopin->port == GPIOB) && (iopin->pin == 1)) { // PB1
		ADC1->CHSELR |= (1 << 9); // Select channel 9
	}
	else if ((iopin->port == GPIOC) && (iopin->pin == 0)) { // PC0
		ADC1->CHSELR |= (1 << 10); // Select channel 10
	}
	else if ((iopin->port == GPIOC) && (iopin->pin == 1)) { // PC1
		ADC1->CHSELR |= (1 << 11); // Select channel 11
	}
	else if ((iopin->port == GPIOC) && (iopin->pin == 2)) { // PC2
		ADC1->CHSELR |= (1 << 12); // Select channel 12
	}
	else if ((iopin->port == GPIOC) && (iopin->pin == 3)) { // PC3
		ADC1->CHSELR |= (1 << 13); // Select channel 13
	}
	else if ((iopin->port == GPIOC) && (iopin->pin == 4)) { // PC4
		ADC1->CHSELR |= (1 << 14); // Select channel 14
	}
	else if ((iopin->port == GPIOC) && (iopin->pin == 5)) { // PC5
		ADC1->CHSELR |= (1 << 15); // Select channel 15
	}
	else {
		// Pin not connected to ADC, do nothing
		return 0;
	}

	ADC1->CR |= (1 << 2); // Set the ADSTART bit to start a conversion
	while ((ADC1->ISR & (1 << 2)) == 0); // Wait for the conversion to complete
	return (uint16_t)ADC1->DR; // Return the ADC data

}

uint16_t analogReadChannel(int channel) {
	// Read an analog value from a specific ADC channel
	ADC1->CHSELR &= 0xFFF80000; // Reset the channel selection
	if ((channel >= 0) && (channel <= 18)) { // Check that the channel requested is valid
		ADC1->CHSELR |= (1 << channel); // Select the channel
		ADC1->CR |= (1 << 2); // Set the ADSTART bit to start a conversion
		while ((ADC1->ISR & (1 << 2)) == 0); // Wait for the conversion to complete
		return (uint16_t)ADC1->DR; // Return the ADC data
	}
	return 0; // Invalid channel selection, do not read ADC
}
