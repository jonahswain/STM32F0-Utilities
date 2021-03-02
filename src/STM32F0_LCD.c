/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 19/04/2018
Date modified: 19/04/2018

Module: LCD

*/

/* INCLUDES */

#ifndef STM32F0_LCD_H
#include "STM32F0_LCD.h"
#define STM32F0_LCD_H
#endif

/* FUNCTIONS */

void __cycleEN() {
	// Cycles the LCD_EN line to tell the LCD data is ready
	__cpuHoldDelay(5); // Wait before starting EN cycle
	digitalWrite(LCD_EN, HIGH); // Make sure the EN line is high
	__cpuHoldDelay(5); // Wait before pulling EN line low
	digitalWrite(LCD_EN, LOW); // Set the EN line low (to let the LCD know data is ready)
	__cpuHoldDelay(10); // Wait for the LCD to get the signal
	digitalWrite(LCD_EN, HIGH); // Set the EN line high again
}

void init_LCD() {
	// Initialises the LCD (requires the GPIO ports to be initialised already)
	// Set pin modes of LCD connections to output
	pinMode(LCD_RS, GPIO_OUTPUT);
	pinMode(LCD_EN, GPIO_OUTPUT);
	pinMode(LCD_D4, GPIO_OUTPUT);
	pinMode(LCD_D5, GPIO_OUTPUT);
	pinMode(LCD_D6, GPIO_OUTPUT);
	pinMode(LCD_D7, GPIO_OUTPUT);
	digitalWrite(LCD_EN, HIGH); // Set the EN line high (pulling it low later tells the LCD data is ready)

	__cpuHoldDelay(20000); // Give the LCD time to power up

	lcdCommand(LCD_ENABLE); // Tell the controller to initialise the LCD
	lcdCommand(LCD_4BIT_MODE); // Tell the controller that data will be transmitted over LCD_D4-7
	lcdCommand(LCD_DISPLAY_ENABLE); // Enable the display and configure the cursor
	lcdCommand(LCD_2LINE_MODE); // Set the LCD to 2 line configuration (with 5x8 pixel characters)
	lcdCommand(LCD_CLEAR_DISPLAY); // Clears any text from the display

}

void lcdCommand(uint8_t command) {
	// Sends a command to the LCD

	digitalWrite(LCD_RS, LOW); // Set the RS line low so data will be interpreted as a command
	// Transmit the most significant 4 bits
	digitalWrite(LCD_D7, command & 0x80); // Set the D7 dataline to the first bit
	digitalWrite(LCD_D6, command & 0x40); // Set the D6 dataline to the second bit
	digitalWrite(LCD_D5, command & 0x20); // Set the D5 dataline to the third bit
	digitalWrite(LCD_D4, command & 0x10); // Set the D4 dataline to the fourth bit
	// Send the data
	__cycleEN(); // Cycle the EN line to tell the LCD data is ready
	// Transmit the least significant 4 bits
	digitalWrite(LCD_D7, command & 0x08); // Set the D7 dataline to the fifth bit
	digitalWrite(LCD_D6, command & 0x04); // Set the D6 dataline to the sixth bit
	digitalWrite(LCD_D5, command & 0x02); // Set the D5 dataline to the seventh bit
	digitalWrite(LCD_D4, command & 0x01); // Set the D4 dataline to the eighth bit
	// Send the data
	__cycleEN(); // Cycle the EN line to tell the LCD data is ready
	if ((command == LCD_CLEAR_DISPLAY) || (command == LCD_CURSOR_HOME)) {
		// Allow 1530 uS for execution (as per datasheet)
		__cpuHoldDelay(1530);
	}
	else {
		// Allow 43 uS for execution (as per datasheet)
		__cpuHoldDelay(43);
	}
}

void lcdCursorPosition(uint8_t line, uint8_t position) {
	// Moves the cursor to a specified position
	position &= 0x1F; // Drop the 3 MSBs of the position (note that this does allow the cursor to be moved off-screen, but not out of range)
	if (line == 1) {
		// Line 1
		lcdCommand(0x80 | position); // Send the command to set DDRAM address to cursor position
	}
	else if (line == 2) {
		// Line 2
		lcdCommand(0xC0 | position); // Send the command to set DDRAM address to cursor position, with line 2 offset
	}
}

void lcdPlaceChar(unsigned char character) {
	// Places a character at the current cursor position

	digitalWrite(LCD_RS, HIGH); // Set the RS line low so data will be interpreted as a character
	// Transmit the most significant 4 bits
	digitalWrite(LCD_D7, character & 0x80); // Set the D7 dataline to the first bit
	digitalWrite(LCD_D6, character & 0x40); // Set the D6 dataline to the second bit
	digitalWrite(LCD_D5, character & 0x20); // Set the D5 dataline to the third bit
	digitalWrite(LCD_D4, character & 0x10); // Set the D4 dataline to the fourth bit
	// Send the data
	__cycleEN(); // Cycle the EN line to tell the LCD data is ready
	// Transmit the least significant 4 bits
	digitalWrite(LCD_D7, character & 0x08); // Set the D7 dataline to the fifth bit
	digitalWrite(LCD_D6, character & 0x04); // Set the D6 dataline to the sixth bit
	digitalWrite(LCD_D5, character & 0x02); // Set the D5 dataline to the seventh bit
	digitalWrite(LCD_D4, character & 0x01); // Set the D4 dataline to the eighth bit
	// Send the data
	__cycleEN(); // Cycle the EN line to tell the LCD data is ready
	__cpuHoldDelay(43); // Allow 43 uS for execution
}

void lcdWriteString(char* string) {
	// Places a string starting at the current cursor position
	uint8_t cnt = 0; // A counter to iterate through the string
	while (string[cnt] && (cnt < 16)) {
		// Iterate through the string until the null terminator is reached or the display length is exceeded
		lcdPlaceChar(string[cnt]); // Place the current character on the LCD
		cnt++;
	}
}

void lcdWriteLine1(char* string) {
	// Writes a string to the first line of the LCD
	lcdCommand(LCD_CURSOR_HOME); // Move the cursor back to the home position
	lcdWriteString("                "); // Clear the line
	lcdCommand(LCD_CURSOR_HOME); // Move the cursor back to the home position
	lcdWriteString(string); // Write the string to the LCD
}

void lcdWriteLine2(char* string) {
	// Writes a string to the second line of the LCD
	lcdCommand(LCD_CURSOR_LINE2); // Move the cursor to line 2
	lcdWriteString("                "); // Clear the line
	lcdCommand(LCD_CURSOR_LINE2); // Move the cursor to line 2
	lcdWriteString(string); // Write the string to the LCD
}

void lcdWrite(char* line1, char* line2) {
	// Writes strings to both lines of the LCD
	lcdCommand(LCD_CLEAR_DISPLAY); // Clear the display
	lcdCommand(LCD_CURSOR_HOME); // Move the cursor back to the home position
	lcdWriteString(line1); // Write line 1 of the LCD
	lcdCommand(LCD_CURSOR_LINE2); // Move the cursor to line 2
	lcdWriteString(line2); // Write line 2 of the LCD
}

