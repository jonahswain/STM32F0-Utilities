#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 19/04/2018
Date modified: 19/04/2018

Module: LCD
Functions for interfacing with the ADM1602K-NSA-FBS/3.3V LCD module in 4 bit configuration
4 bit configuration is used to save GPIO pins on the microcontroller

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

#ifndef STM32F0_GPIO_H
#include "STM32F0_GPIO.h"
#define STM32F0_GPIO_H
#endif

#ifndef STM32F0_OTHER_H
#include "STM32F0_OTHER.h"
#define STM32F0_OTHER_H
#endif

/* CONSTANT DEFINITIONS */
// LCD interface connections (configured for UCT STM32F0 board) - modify if your LCD is connected differently
#define LCD_RS PC14 // LCD Register select signal
#define LCD_EN PC15 // LCD Operation enable signal
#define LCD_D4 PB8 // LCD Data pin 4 (MPU high-order bus line)
#define LCD_D5 PB9 // LCD Data pin 5 (MPU high-order bus line)
#define LCD_D6 PA12 // LCD Data pin 6 (MPU high-order bus line)
#define LCD_D7 PA15 // LCD Data pin 7 (MPU high-order bus line)

// LCD commands
#define LCD_ENABLE 0x33 // Power up the LCD
#define LCD_4BIT_MODE 0x32 // Set the LCD to use the high-order bus lines only
#define LCD_2LINE_MODE 0x28 // Sets the LCD to 2 line mode (characters on both lines of the display)

#define LCD_DISPLAY_ENABLE 0x0C // Displays data on the LCD (Also disables cursor flashing and makes the cursor invisible)
#define LCD_DISPLAY_DISABLE 0x08 // Blanks the LCD (but retains data)
#define LCD_CURSOR_ENABLE 0x01 // Enables the cursor visibility (must be transmitted bitwise or-ed with LCD_DISPLAY_ENABLE, reset by transmitting LCD_DISPLAY_ENABLE alone)
#define LCD_CURSOR_BLINKING 0x03 // Enables cursor blinking (must be transmitted bitwise or-ed with LCD_DISPLAY_ENABLE, reset by transmitting LCD_DISPLAY_ENABLE alone)
#define LCD_CLEAR_DISPLAY 0x01 // Removes all data from the LCD
#define LCD_CURSOR_HOME 0x02 // Sets the cursor position to the first character of the first line
#define LCD_CURSOR_LINE2 0xC0 // Moves the cursor to the second line
#define LCD_CURSOR_RIGHT 0x14 // Moves the cursor 1 position right
#define LCD_CURSOR_LEFT 0x10 // Moves the cursor 1 position left

/* FUNCTIONS */

void __cycleEN(); // Cycles the LCD_EN line to tell the LCD data is ready

void init_LCD(); // Initialises the LCD (requires the GPIO ports to be initialised already)

void lcdCommand(uint8_t command); // Sends a command to the LCD
void lcdCursorPosition(uint8_t line, uint8_t position); // Moves the cursor to a specified position
void lcdPlaceChar(unsigned char character); // Places a character at the current cursor position
void lcdWriteString(char* string); // Places a string starting at the current cursor position
void lcdWriteLine1(char* string); // Writes a string to the first line of the LCD
void lcdWriteLine2(char* string); // Writes a string to the second line of the LCD
void lcdWrite(char* line1, char* line2); // Writes strings to both lines of the LCD
