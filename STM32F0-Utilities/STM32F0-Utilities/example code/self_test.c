/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 24/05/2018
Date modified: 25/05/2018

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

#ifndef WAVE_SINE256_H
#include "WAVE_SINE256.h"
#define WAVE_SINE256_H
#endif

#ifndef WAVE_TABLE_MOUNTAIN_H
#include "WAVE_TABLE_MOUNTAIN.h"
#define WAVE_TABLE_MOUNTAIN_H
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

// Function declarations
void fullSelfTest();
void menu();
int testEEPROMWrite();
int testEEPROMRead();
uint8_t readPot(uint8_t pot);
void moveLEDs();
void testLEDs();
void testPushButtons();
void testPotentiometers();
void testRGLED();
void testTempSensor();
void testEEPROM();
void testDAC();

void fullSelfTest() {
	// The full complement of self tests
	// Initial setup
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
	rgLedWrite(0, 0); // Turn off the RG LED

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

	testEEPROM(); // Test EEPROM

}

void menu() {
	// Menu of self-tests to display on LCD
	if (menuPosition == 0) {
		lcdWrite("Self-tests:", "         \x7E      ");
	}
	else if (menuPosition == 1) {
		lcdWrite("Red LEDs", " RUN  \x7F  \x7E      ");
	}
	else if (menuPosition == 2) {
		lcdWrite("Push buttons", " RUN  \x7F  \x7E      ");
	}
	else if (menuPosition == 3) {
		lcdWrite("Potentiometers", " RUN  \x7F  \x7E      ");
	}
	else if (menuPosition == 4) {
		lcdWrite("Red-Green LED", " RUN  \x7F  \x7E      ");
	}
	else if (menuPosition == 5) {
		lcdWrite("Temp sensor", " RUN  \x7F  \x7E      ");
	}
	else if (menuPosition == 6) {
		lcdWrite("EEPROM", " RUN  \x7F  \x7E      ");
	}
	else if (menuPosition == 7) {
		lcdWrite("DAC", " RUN  \x7F  \x7E      ");
	}
	else if (menuPosition == 8) {
		lcdWrite("Factory config", " SET  \x7F         ");
	}
}

void pinInterruptTriggered(IOPin_TypeDef* iopin) {
	// Function called when a pin interrupt is triggered
	__cpuHoldDelay(1000); // Wait to avoid switch bounce
	if (menuPosition != 255) {
		// Ensure that the interrupt was not caused by a button being pressed for a self-test
		if (!digitalRead(SW0)) {
			// SW0 (Run) pressed
			if (menuPosition == 1) {
				// Run LED test
				menuPosition = 255;
				testLEDs();
				menuPosition = 1;
			}
			else if (menuPosition == 2) {
				// Run Push-button test
				menuPosition = 255;
				testPushButtons();
				menuPosition = 2;
			}
			else if (menuPosition == 3) {
				// Run Potentiometer test
				menuPosition = 255;
				testPotentiometers();
				menuPosition = 3;
			}
			else if (menuPosition == 4) {
				// Run Red-Green LED test
				menuPosition = 255;
				testRGLED();
				menuPosition = 4;
			}
			else if (menuPosition == 5) {
				// Run Temperature sensor test
				menuPosition = 255;
				testTempSensor();
				menuPosition = 5;
			}
			else if (menuPosition == 6) {
				// Run EEPROM test
				menuPosition = 255;
				testEEPROM();
				menuPosition = 6;
			}
			else if (menuPosition == 7) {
				// Run EEPROM test
				menuPosition = 255;
				testDAC();
				menuPosition = 7;
			}
			else if (menuPosition == 8) {
				// Reset test completion byte in EEPROM to load default configuration on next reset
				eepromWrite(EEPROM_TESTCOMPL_OFFSET, 0); // Write to EEPROM
				lcdWrite("Reboot to load", "Factory config");
				while (1); // Wait forever
			}
		}
		else if (!digitalRead(SW1)) {
			// SW1 (Left) pressed
			if (menuPosition > 0) {
				menuPosition--; // Decrement the menu position
			}
		}
		else if (!digitalRead(SW2)) {
			// SW2 (Right) pressed
			if (menuPosition < 8) {
				menuPosition++; // Increment the menu position
			}
		}
		menu(); // Display the menu
	}
}

void TIM6_DAC_IRQHandler() {
	// TIM6 ISR
	static int rLED = 0;
	static int gLED = 255;
	static int rIncrement = 5;
	static int gIncrement = 5;
	static uint8_t ledPattern;
	static signed int ledDirection = 0;
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
	if (counter > 30) {
		// Move LEDs
		counter = 0;
		
		if (ledDirection == 0) {
			ledPattern = 1;
			ledDirection = -1;
		}
		else if (ledDirection == -1) {
			ledPattern <<= 1;
		}
		else if (ledDirection == 1) {
			ledPattern >>= 1;
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

void moveLEDs() {
	static uint8_t rLedPattern = 0;
	static signed int rLedDirection = 0;

	// Move the LEDs every second for LED test
	if (rLedDirection == 0) {
		rLedPattern = 1;
		rLedDirection = -1;
	}
	else if (rLedDirection == -1) {
		rLedPattern <<= 1;
	}
	else if (rLedDirection == 1) {
		rLedPattern >>= 1;
	}
	if (rLedPattern == 0) {
		if (rLedDirection == -1) {
			rLedPattern = 0x40;
		}
		else if (rLedDirection == 1) {
			rLedPattern = 0x02;
		}
		rLedDirection *= -1;
	}
	ledWrite(rLedPattern);

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
		return (uint8_t)(analogRead(POT1) >> 2);
	}
}

void testLEDs() {
	// Test red LEDs
	
	lcdWrite("Testing", "Red LEDs"); // Display message on LCD
	startTimer(TIM6, 999); // Start a timer for 1s
	while (!timerComplete(TIM6)); // Wait for timer to complete
	lcdWrite("Press any key", "to continue"); // Display message on LCD
	startRepeatingTimer(TIM6, 499); // Start a timer for 0.5s
	while ((GPIOA->IDR & 0x000F) == 0x000F) {
		// Wait for button press
		if (timerComplete(TIM6)) {
			clearStatusFlag(TIM6);
			moveLEDs(); // Cycle LEDs when timer completes
		}
	}
	stopTimer(TIM6); // Stop timer
	ledWrite(0); // Turn off LEDs
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
	while (digitalRead(SW0));
	lcdWrite("Press SW1", "");
	while (digitalRead(SW1));
	lcdWrite("Press SW2", "");
	while (digitalRead(SW2));
	lcdWrite("Press SW3", "");
	while (digitalRead(SW3));

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
	ledWrite(0);
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
	startTimer(TIM6, 1999); // Set timer for 2 seconds
	while (!timerComplete(TIM6)); // Wait for timer to complete
	lcdWrite("Press any key", "when complete"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
	uint8_t redVal = 0;
	uint8_t greenVal = 0;
	char redStr[4];
	char greenStr[4];
	lcdWrite("Red: ", "Green: "); // Display message on LCD
	startRepeatingTimer(TIM6, 100); // Set TIM6 to update every 100ms
	while ((GPIOA->IDR & 0x000F) == 0x000F) {
		if (timerComplete(TIM6)) {
			clearStatusFlag(TIM6); // Reset the flag
			// Get pot values and convert to strings
			redVal = readPot(0);
			greenVal = readPot(1);
			sprintf(redStr, "%d", redVal);
			sprintf(greenStr, "%d", greenVal);
			
			rgLedWrite(redVal, greenVal); // Update the LED

			// Display values on LCD
			lcdCursorPosition(1, 6);
			lcdWriteString("   ");
			lcdCursorPosition(1, 6);
			lcdWriteString(redStr);
			lcdCursorPosition(2, 8);
			lcdWriteString("   ");
			lcdCursorPosition(2, 8);
			lcdWriteString(greenStr);
		}
	}
	stopTimer(TIM6); // Stop TIM6
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
		uint8_t tempVal = 0;
		char tempStr[4];
		lcdWrite("Temp: ", "Press any key"); // Display message on LCD
		startRepeatingTimer(TIM6, 100); // Start a repeating timer
		while ((GPIOA->IDR & 0x000F) == 0x000F) { // Wait for button press and display current temperature on LCD
			if (timerComplete(TIM6)) {
				tempVal = tempSensorRead(); // Read the temp sensor
				ledWrite(tempVal);
				sprintf(tempStr, "%d", tempVal);
				lcdCursorPosition(1, 7);
				lcdWriteString("   ");
				lcdCursorPosition(1, 7);
				lcdWriteString(tempStr);
			}
		}
		stopTimer(TIM6); // Stop TIM6
	}
	else {
		lcdWrite("Temp sensor", "fail :("); // Display message on LCD
		startTimer(TIM6, 1999); // Set timer for 2 seconds
		while (!timerComplete(TIM6)); // Wait for timer to complete
	}

}

void testEEPROM() {
	// Test the EEPROM

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
	while (1); // Wait for reboot
}

void testDAC() {
	// Test the DAC (and DMA)
	lcdWrite("Testing DAC", "Scope PA4"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
	dacDMAWaveGen(1, WAVE_SINE256, 1, WAVE_SINE256_LENGTH, 30); // Generate waveform
	lcdWrite("Press any key", "to continue"); // Display message on LCD
	while ((GPIOA->IDR & 0x000F) == 0x000F);
	lcdWrite("DAC test", "complete"); // Display message on LCD
	startTimer(TIM6, 999); // Set timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
}

/* MAIN FUNCTION */
void main() {
	// Setup code, to run once
	init_peripherals(); // Initialise all the onboard peripherals
	lcdWrite("Welcome!", "STM32F0 ready"); // Display welcome message
	startTimer(TIM6, 999); // Start a timer for 1 second
	while (!timerComplete(TIM6)); // Wait for timer to complete
	if (eepromRead(EEPROM_TESTCOMPL_OFFSET) == 0xAB) {
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
			eepromWrite(EEPROM_TESTCOMPL_OFFSET, 0xAB); // Write test complete data to EEPROM
			for (uint16_t i = 0; i < 2; i++) {
				eepromWrite(i + EEPROM_SIG_OFFSET, eepromSignature[i]); // A personal touch
			}
			dacDMAWaveGen(1, WAVE_TABLE_MOUNTAIN, 1, WAVE_TABLE_MOUNTAIN_LENGTH, 20); // Easter egg
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