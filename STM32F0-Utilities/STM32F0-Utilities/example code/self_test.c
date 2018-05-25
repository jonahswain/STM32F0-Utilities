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

	// Ensure no buttons are stuck down/shorted to ground
	if ((GPIOA->IDR & 0x000F) != 0x000F) {
		// Push-button pressed in
		lcdWrite("Release all", "push buttons"); // Display message on LCD
		while ((GPIOA->IDR & 0x000F) != 0x000F); // Wait for buttons to be released
	}

	// Test SW0
	lcdWrite("Press SW0", ""); // Display message on LCD
	while (digitalRead(SW0)); // Wait for SW0 pressed

	nvicDisableInterrupt(17); // Disable TIM6 interrupt
	stopTimer(TIM6); // Stop the timer

	testPotentiometers(); // Test pots

	// Test SW1
	lcdWrite("Press SW1", ""); // Display message on LCD
	while (digitalRead(SW1)); // Wait for SW1 pressed

	testRGLED(); // Test RG LED

	// Test SW2
	lcdWrite("Press SW2", ""); // Display message on LCD
	while (digitalRead(SW2)); // Wait for SW2 pressed

	testTempSensor(); // Test temperature sensor

	// Test SW3
	lcdWrite("Press SW3", ""); // Display message on LCD
	while (digitalRead(SW3)); // Wait for SW3 pressed



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

void TIM14_IRQHandler() {
	static uint8_t ledPattern = 0;
	static int ledDirection = 0;

	// Move the LEDs every second for LED test
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

uint8_t readPot(uint8_t pot) {
	// Reads an 8-bit value from the potentiometers
	if (!pot) {
		// Pot 0
		return (uint8_t)(analogRead(POT0) >> 2);
	}
	else {
		// Pot 1
		return (uint8_t)(analogRead(POT0) >> 2);
	}
}

void testLEDs() {
	// Test red LEDs

	lcdWrite("Testing", "Red LEDs"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	init_timer(TIM14, 47999); // Initialise TIM14
	startRepeatingTimer(TIM14, 999); // Set TIM14 to overflow every second
	timerInterruptEnable(TIM14, 255); // Enable TIM14 interrupt
	while (!timerComplete(TIM6)); // Wait for timer to complete
	lcdWrite("Press any key", "to continue"); // Display message on LCD
	while ((GPIOA->IDR & 0x000F) == 0x000F); // Wait for button press
	nvicDisableInterrupt(19); // Disable TIM14 interrupt
	stopTimer(TIM14); // Stop TIM14
}

void testPushButtons() {
	// Test push buttons

	lcdWrite("Testing", "Push buttons"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete

	// Ensure no buttons are stuck down/shorted to ground
	if ((GPIOA->IDR & 0x000F) != 0x000F) {
		// Push-button pressed in
		lcdWrite("Release all", "push buttons"); // Display message on LCD
		while ((GPIOA->IDR & 0x000F) != 0x000F); // Wait for buttons to be released
	}
	lcdWrite("Press SW0", "");
	while (!digitalRead(SW0));
	lcdWrite("Press SW1", "");
	while (!digitalRead(SW1));
	lcdWrite("Press SW2", "");
	while (!digitalRead(SW2));
	lcdWrite("Press SW3", "");
	while (!digitalRead(SW3));

	lcdWrite("Push buttons", "pass!"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
}

void testPotentiometers() {
	// Test potentiometers (must be within 2% of ideal)

	lcdWrite("Testing", "Potentiometers"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
	lcdWrite("Turn POT 0", "clockwise"); // Display message on LCD
	while (readPot(0) < 250) {
		ledWrite(readPot(0)); // Display pot value on LEDs
	}
	lcdWrite("Turn POT 0", "countr-clockwise"); // Display message on LCD
	while (readPot(0) > 5) {
		ledWrite(readPot(0)); // Display pot value on LEDs
	}
	lcdWrite("Turn POT 1", "clockwise"); // Display message on LCD
	while (readPot(1) < 250) {
		ledWrite(readPot(1)); // Display pot value on LEDs
	}
	lcdWrite("Turn POT 1", "countr-clockwise"); // Display message on LCD
	while (readPot(1) > 5) {
		ledWrite(readPot(1)); // Display pot value on LEDs
	}
	lcdWrite("Potentiometers", "Pass!"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
}

void testRGLED() {
	// Test the RG LED

	lcdWrite("Testing", "Red-Green LED"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
	lcdWrite("Red: POT0", "Green: POT1"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
	lcdWrite("Press any key", "when complete"); // Display message on LCD
	while ((GPIOA->IDR & 0x000F) == 0x000F) {
		rgLedWrite(readPot(0), readPot(1)); // Change PWM based on pot inputs
	}
}

void testTempSensor() {
	// Test the temperature sensor

	int tsPass = 1;

	lcdWrite("Testing", "Temp sensor"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete

	if (tempSensorRead() == 0) {
		lcdWrite("No connection to", "Temp sensor"); // Display message on LCD
		tsPass = 0;
		startTimer(TIM6, 999); // Set timer for 1 second
		while (!timerComplete(TIM6)); // Wait for timer to complete
	}
	else if ((tempSensorRead() > 35) || (tempSensorRead() < 5)) {
		lcdWrite("Temperature", "out of range"); // Display message on LCD
		tsPass = 0;
		startTimer(TIM6, 999); // Set timer for 1 second
		while (!timerComplete(TIM6)); // Wait for timer to complete
	}
	else {
		lcdWrite("Temp sensor", "pass!"); // Display message on LCD
		startTimer(TIM6, 999); // Set timer for 1 second
		while (!timerComplete(TIM6)); // Wait for timer to complete
	}
	if (tsPass) {
		char tempStr[4];
		lcdWrite("Temp: ", "Press any key"); // Display message on LCD
		while ((GPIOA->IDR & 0x000F) == 0x000F) { // Wait for button press and display current temperature on LCD
			sprintf(tempStr, "%d", tempSensorRead());
			lcdCursorPosition(1, 7);
			lcdWriteString("   ");
			lcdCursorPosition(1, 7);
			lcdWriteString(tempStr);
		}
	}
	else {
		lcdWrite("Temp sensor", "fail :("); // Display message on LCD
		startTimer(TIM6, 1999); // Set timer for 2 seconds
		while (!timerComplete(TIM6)); // Wait for timer to complete
	}

}

void testEEPROM() {
	lcdWrite("Testing", "EEPROM"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
	if (testEEPROMWrite()) {
		lcdWrite("EEPROM written", "reboot now"); // Display message on LCD
	}
	else {
		lcdWrite("Error writing", "to EEPROM"); // Display message on LCD
		startTimer(TIM6, 999); // Set timer for 1 second
		while (!timerComplete(TIM6)); // Wait for timer to complete
		lcdWrite("EEPROM fail :(", "reboot now"); // Display message on LCD
	}
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
			lcdWrite("EEPROM Pass!", "Press any key"); // Display message on LCD
			while ((GPIOA->IDR & 0x000F) == 0x000F); // Wait for button press
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
			lcdWrite("EEPROM Pass!", "Press any key"); // Display message on LCD
			while ((GPIOA->IDR & 0x000F) == 0x000F); // Wait for button press
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