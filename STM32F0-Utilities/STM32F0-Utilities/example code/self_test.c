/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 24/05/2018
Date modified: 24/05/2018

Self Test
Example firmware to load on to the board to do a full peripheral self-test and demonstrate the features
Rename this file to 'main.c'

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

#ifndef STM32F0_UCTDEV_H
#include "STM32F0_UCTDEV.h"
#define STM32F0_UCTDEV_H
#endif

/* CONSTANT DEFINITIONS */
#define EEPROM_TESTDATA_OFFSET 0 // Address in EEPROM to store test data
#define EEPROM_TESTCOMPL_OFFSET 8189 // Address in EEPROM to store all tests complete flag
#define EEPROM_SIG_OFFSET 8190 // Address in EEPROM to place signature after running tests

/* GLOBAL VARIABLES */
uint8_t eepromTestData[] = { 0xEE, 0xE2, 0x04, 0x6F }; // EEPROM test data
uint8_t eepromSignature[] = { 'J', 'S' }; // A personal touch
uint8_t menuPosition = 0;

/* FUNCTIONS */

void fullSelfTest() {
	// The full complement of self tests
	// Initial setup
	uint8_t count = 0;
	startRepeatingTimer(TIM6, 20); // Set TIM6 to overflow every ~20ms
	timerInterruptEnable(TIM6, 255); // Enable TIM6 interrupt

	if (GPIOA->IDR & 0x000F) {
		// Push-button pressed in
		lcdWrite("Release all", "push buttons"); // Display message on LCD
		while (GPIOA->IDR & 0x000F); // Wait for buttons to be released
	}

	lcdWrite("Press SW0", ""); // Display message on LCD
	while (digitalRead(SW0)); // Wait for SW0 pressed

	nvicDisableInterrupt(17); // Disable TIM6 interrupt
	// HERE

}

void menu() {
	// Menu of self-tests to display on LCD
}

void pinInterruptTriggered(IOPin_TypeDef* iopin) {
	// Function called when a pin interrupt is triggered
}

void TIM6_DAC_IRQHandler() {
	// TIM6 ISR
	static int rLED = 0;
	static int gLED = 255;
	static int rIncrement = 5;
	static int gIncrement = 5;
	static uint8_t ledPattern;
	static int ledDirection = 0;
	static uint8_t counter = 0;

	clearStatusFlag(TIM6); // Clear the interrupt flag

	rLED += rIncrement; // Increment red PWM value
	if ((rLED > 255) || (rLED < 0)) { // Check for overflow
		rIncrement *= -1; // Reverse direction
		rLED += rIncrement; // Increment again
	}

	gLED += gIncrement; // Increment green PWM value
	if ((gLED > 255) || (gLED < 0)) { // Check for overflow
		gIncrement *= -1; // Reverse direction
		gLED += gIncrement; // Increment again
	}

	pwmWrite(TIM2, 3, rLED); // Set LED PWM
	pwmWrite(TIM2, 4, gLED); // Set LED PWM

	counter++;
	if (counter > 50) {
		// Move LEDs every second
		counter = 0;
		if (ledDirection == 0) {
			ledPattern = 1;
			ledDirection = -1;
		}
		else if (ledDirection == -1) {
			ledPattern << 1;
		}
		else if (ledDirection == 1) {
			ledPattern >> 1;
		}
		if (ledPattern == 0) {
			if (ledDirection == -1) {
				ledPattern = 0x40;
			}
			else if (ledDirection == 1) {
				ledPattern = 0x02;
			}
			ledDirection *= -1;
		}
		ledWrite(ledPattern);
	}

}

int testEEPROMWrite() {
	// Write test data to EEPROM
	uint16_t address; // EEPROM address
	for (uint16_t i = 0; i < 4; i++) {
		address = EEPROM_TESTDATA_OFFSET + i;
		eepromWrite(address, eepromTestData[i]); // Write data to EEPROM
		if (eepromRead(address) != eepromTestData[i]) { // Verify data
			return 0; // EEPROM data does not match
		}
	}
	return 1; // Writing complete
}

int testEEPROMRead() {
	// Read, verify, and erase test data from EEPROM
	uint16_t address; // EEPROM address
	for (uint16_t i = 0; i < 4; i++) {
		address = EEPROM_TESTDATA_OFFSET + i;
		if (eepromRead(address) != eepromTestData[i]) { // Check to see if data matches
			return 0;
		}
		eepromWrite(address, 0); // Erase data
	}
	return 1;
}

/* MAIN FUNCTION */
void main() {
	// Setup code, to run once
	init_peripherals(); // Initialise all the onboard peripherals
	lcdWrite("Welcome!", "STM32F0 ready"); // Display welcome message
	startTimer(TIM6, 999); // Start a timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
	if (eepromRead(EEPROM_TESTCOMPL_OFFSET) == 0xFF) {
		if (testEEPROMRead()) {
			// EEPROM test passed
			lcdWrite("EEPROM Pass!", "Press SW0"); // Display message on LCD
			while (!digitalRead(SW0)); // Wait for button press
		}
		pinInterruptEnable(SW0, 0, 1, 255); // Enable falling edge interrupt on SW0
		pinInterruptEnable(SW1, 0, 1, 255); // Enable falling edge interrupt on SW1
		pinInterruptEnable(SW2, 0, 1, 255); // Enable falling edge interrupt on SW2
		pinInterruptEnable(SW3, 0, 1, 255); // Enable falling edge interrupt on SW3
		menu(); // Call the menu
	}
	else {
		if (testEEPROMRead()) {
			// EEPROM test passed
			lcdWrite("EEPROM Pass!", "Press SW0"); // Display message on LCD
			while (!digitalRead(SW0)); // Wait for button press
			eepromWrite(EEPROM_TESTCOMPL_OFFSET, 0xFF); // Write test complete data to EEPROM
			for (uint16_t i = 0; i < 2; i++) {
				eepromWrite(i + EEPROM_SIG_OFFSET, eepromSignature[i]); // A personal touch
			}
			lcdWrite("Testing Complete", "Reboot!"); // Display message on LCD
		}
		else {
			fullSelfTest(); // Run the full complement of self-tests
		}
	}

	// Main loop
	while(1) {

	}
}