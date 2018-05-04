/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 11/04/2018
Date modified: 30/04/2018

Module: GPIO
*/

/* INCLUDES */

#ifndef STM32F0_GPIO_H
#include "STM32F0_GPIO.h"
#define STM32F0_GPIO_H
#endif

/* FUNCTIONS */

void init_GPIOA() {
	// Enables the clock for GPIO port A
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}
void init_GPIOB() {
	// Enables the clock for GPIO port B
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
}
void init_GPIOC() {
	// Enables the clock for GPIO port C
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}
void init_GPIOD() {
	// Enables the clock for GPIO port D
	RCC->AHBENR |= RCC_AHBENR_GPIODEN;
}
void init_GPIOE() {
	// Enables the clock for GPIO port E
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
}
void init_GPIOF() {
	// Enables the clock for GPIO port F
	RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
}
void init_STD_GPIO() {
	// Enables the clock for GPIO ports A, B, C, and F (those available on the STM32F051)
	RCC->AHBENR |= 0x004E0000;
}
void init_ALL_GPIO() {
	// Enables the clock for all GPIO ports
	RCC->AHBENR |= 0x007E0000;
}

int pinMatches(IOPin_TypeDef* iopin1, IOPin_TypeDef* iopin2) {
	// Returns whether 2 GPIO pins are the same (boolean)
	return ((iopin1->port == iopin2->port) && (iopin1->pin == iopin2->pin)); // Compare their GPIO ports and pin numbers
}

void pinMode(IOPin_TypeDef* iopin, int mode) {
	// Sets the mode of a GPIO pin (input/output/alternate function/analog)
	if ((mode >= 0) && (mode <= 3)) { // Check for valid mode, otherwise do nothing
		iopin->port->MODER &= ~(3 << (2 * iopin->pin)); // Reset the pin to 0 (input)
		iopin->port->MODER |= (mode << (2 * iopin->pin)); // Set the pin to the desired mode
	}
}

void pinState(IOPin_TypeDef* iopin, int state) {
	// Sets the pin input state (floating/pullup/pulldown)
	if ((state >= 0) && (state <= 3)) { // Check for valid state, otherwise do nothing
		iopin->port->PUPDR &= ~(3 << (2 * iopin->pin)); // Reset the pin to 0 (floating)
		iopin->port->PUPDR |= (state << (2 * iopin->pin)); // Set the pin to the desired state
	}
}

void pinOutputType(IOPin_TypeDef* iopin, int type) {
	// Sets the pin type (push-pull/open drain)
	if (type) {
		// Set type to open drain
		iopin->port->OTYPER |= (1 << iopin->pin);
	}
	else {
		// Set type to push-pull
		iopin->port->OTYPER &= ~(1 << iopin->pin);
	}
}

void afSelect(IOPin_TypeDef* iopin, int altfn) {
	// Maps a GPIO pin to one of its alternate functions (note: not all alternate functions are supported on all GPIO ports/pins)
	// See STM32F051 datasheet page 37 for alternate function mappings
	if ((altfn >= 0) && (altfn <= 7)) { // Check for valid alternate function
		if (iopin->pin <= 7) { // Check if writing to high/low AFR
			// Write to low AFR
			iopin->port->AFR[0] &= ~(7 << (4 * iopin->pin)); // Reset the pin alternate function
			iopin->port->AFR[0] |= (altfn << (4 * iopin->pin)); // Set the pin alternate function
		}
		else {
			// Write to high AFR
			iopin->port->AFR[1] &= ~(7 << (4 * (iopin->pin - 8))); // Reset the alternate function
			iopin->port->AFR[1] |= (altfn << (4 * (iopin->pin - 8))); // Write the alternate function
		}
	}
}

int digitalRead(IOPin_TypeDef* iopin) {
	// Returns the current input state of a GPIO pin (high/low)
	return !!(iopin->port->IDR & (1 << iopin->pin));
}

void digitalWrite(IOPin_TypeDef* iopin, int state) {
	// Sets the output state of a GPIO pin (high/low)
	// Uses the bit set/reset register (less computationally intensive and writes are atomic)
	if (state) {
		// Write high
		iopin->port->BSRR = (1 << iopin->pin); // Set the pin
	}
	else {
		// Write low
		iopin->port->BSRR = (1 << (16 + iopin->pin)); // Reset the pin
	}
}