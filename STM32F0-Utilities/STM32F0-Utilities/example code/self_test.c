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
uint8_t ledPattern = 0; // Pattern to display on LEDs
uint8_t ledDirection = 0; // Direction to move LED pattern (initial stage)
int mode = 0; // Testing mode
uint8_t menuPosition = 0;

/* FUNCTIONS */

void fullSelfTest() {
	// The full complement of self tests
}

void menu() {
	// Menu of self-tests to display on LCD
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


	// Main loop
	while(1) {

	}
}