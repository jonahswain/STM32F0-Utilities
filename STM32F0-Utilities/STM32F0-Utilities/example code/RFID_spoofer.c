/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 24/05/2018
Date modified: 25/05/2018

RFID Spoofer
Code to spoof 125kHz RFID tags
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

// Not included in repository, as it contains sensitive data
#ifndef CARD_TAGS_H
#include "card_tags.h"
#define CARD_TAGS_H
#endif

/*
The card_tags.h file contains:
NUM_TAGS - A symbolic constant representing how many tags are contained in the file
uint64_t tagData[] - The tag data stored in the lower 5 bytes/40 bits of a uint64_t
char* tagStrings[] - A string descriptor of the tag (to display on an LCD)
*/

/* CARD SPOOFER 1.1 (Mini) */

#define CLK_DELAY 256 // Delay between clock half cycles (in microseconds)

/* Pin declarations */
#define LED_R PA0 // Red status LED
#define LED_B PA0 // Blue status LED

#define COIL_INT PA0 // Built in (glued on) coil
#define COIL_EXT PA0 // External coil

// DIP switches for card selection
#define DIPSW_0 PA0
#define DIPSW_1 PA0
#define DIPSW_2 PA0
#define DIPSW_3 PA0

/* FUNCTIONS */

uint8_t evenParity(uint16_t data) {
	// Returns the parity bit for 2 bytes of data (or a single byte/nibble)
	uint8_t parity = 0;
	for (uint8_t i = 0; i < 16; i++) {
		if ((data & (1 << i))) {
			parity = !parity;
		}
	}
	return parity;
}

uint64_t binaryData(uint64_t cardTag) {
	// Generates the 64 bits of data to send (start/stop bits, card tag, and parity)
	uint64_t binary = 0;
	uint64_t cols[4] = { 0, 0, 0, 0 };
	uint64_t row;
	uint64_t rowParity;
	uint8_t colParity;

	binary |= 0xFF80000000000000; // Add 9 start bits

	for (int8_t i = 9; i >= 0; i--) {
		row = ((cardTag >> (4 * i)) & 0x0F);
		rowParity = evenParity(row);
		binary |= ((row << (5 * i + 6)) | (rowParity << (5 * i + 5))); // Add row nibble and parity
		// Add values to columns
		cols[0] |= (((row & 0x8) >> 3) << i);
		cols[1] |= (((row & 0x4) >> 2) << i);
		cols[2] |= (((row & 0x2) >> 1) << i);
		cols[3] |= (((row & 0x1) >> 0) << i);
	}
	for (uint8_t i = 0; i < 4; i++) {
		colParity = evenParity(cols[i]);
		binary |= (colParity << (3 - i + 1)); // Add column parity bit
	}

	return binary;
}

uint8_t manchesterEncode(uint8_t clkHalf, uint8_t dataBit) {
	// Generates the manchester encoded value for a data bit and given clock half cycle
	// clkHalf and dataBit are booleans
	return (clkHalf ^ dataBit);
}

void transmitData(uint64_t binaryCardData, uint16_t delay) {
	// Transmits a binary card tag (with a specified delay between bits)
	uint8_t currentBit;
	// Cycle through the bits
	for (uint8_t i = 63; i >= 0; i--) {
		currentBit = !!(binaryCardData & (1 << i)); // Isolate the current bit
		if (manchesterEncode(0, currentBit)) { // Manchester encode the bit, set the coil LOW (tuned) for 1, set the coil HIGH (detuned) for 0
			digitalWrite(COIL_INT, LOW);
			digitalWrite(COIL_EXT, LOW);
		}
		else {
			digitalWrite(COIL_INT, HIGH);
			digitalWrite(COIL_EXT, HIGH);
		}

		startTimer(TIM14, delay); // Delay between bits
		while (!timerComplete(TIM14));

		if (manchesterEncode(1, currentBit)) { // Manchester encode the bit, set the coil LOW (tuned) for 1, set the coil HIGH (detuned) for 0
			digitalWrite(COIL_INT, LOW);
			digitalWrite(COIL_EXT, LOW);
		}
		else {
			digitalWrite(COIL_INT, HIGH);
			digitalWrite(COIL_EXT, HIGH);
		}

		startTimer(TIM14, delay); // Delay between bits
		while (!timerComplete(TIM14));
	}
}

/* MAIN FUNCTION */
void main() {
	// Setup code (to be run once)
	// Pin setup
	pinMode(LED_R, GPIO_OUTPUT);
	pinMode(LED_B, GPIO_OUTPUT);
	pinMode(COIL_INT, GPIO_OUTPUT);
	pinMode(COIL_EXT, GPIO_OUTPUT);
	pinMode(DIPSW_0, GPIO_INPUT);
	pinMode(DIPSW_1, GPIO_INPUT);
	pinMode(DIPSW_2, GPIO_INPUT);
	pinMode(DIPSW_3, GPIO_INPUT);
	pinState(DIPSW_0, GPIO_PULLUP);
	pinState(DIPSW_1, GPIO_PULLUP);
	pinState(DIPSW_2, GPIO_PULLUP);
	pinState(DIPSW_3, GPIO_PULLUP);

	init_timer(TIM14, 48); // Setup timer 14 to tick every microsecond

	uint8_t cardNumber = 0;
	if (!digitalRead(DIPSW_0)) { cardNumber += 1; }
	if (!digitalRead(DIPSW_1)) { cardNumber += 2; }
	if (!digitalRead(DIPSW_2)) { cardNumber += 4; }
	if (!digitalRead(DIPSW_3)) { cardNumber += 8; }

	uint64_t cardData = 0;

	if (cardNumber >= NUM_TAGS) {
		digitalWrite(LED_R, HIGH); // Invalid tag, turn on error LED
		while (1); // Hang until reset
	}
	else {
		uint64_t cardData = binaryData(tagData[cardNumber]);
	}

	uint8_t ledState = 0;
	while (1) { // Main loop (runs infinitely)
		ledState = !ledState;
		digitalWrite(LED_B, ledState);
		transmitData(cardData, CLK_DELAY);
	}

}