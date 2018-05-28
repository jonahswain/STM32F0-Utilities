#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 17/04/2018
Date modified: 28/05/2018

Module: UCTDEV
Utilities specifically for the UCT development board
Includes macros and defines for things on the board

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

#ifndef STM32F0_ADC_H
#include "STM32F0_ADC.h"
#define STM32F0_ADC_H
#endif

#ifndef STM32F0_LCD_H
#include "STM32F0_LCD.h"
#define STM32F0_LCD_H
#endif

#ifndef STM32F0_TIM_H
#include "STM32F0_TIM.h"
#define STM32F0_TIM_H
#endif

#ifndef STM32F0_I2C_H
#include "STM32F0_I2C.h"
#define STM32F0_I2C_H
#endif

#ifndef STM32F0_INTERRUPTS_H
#include "STM32F0_INTERRUPTS.h"
#define STM32F0_INTERRUPTS_H
#endif

#ifndef STM32F0_SPI_H
#include "STM32F0_SPI.h"
#define STM32F0_SPI_H
#endif

#ifndef STM32F0_DAC_H
#include "STM32F0_DAC.h"
#define STM32F0_DAC_H
#endif

#ifndef STM32F0_OTHER_H
#include "STM32F0_OTHER.h"
#define STM32F0_OTHER_H
#endif

/* CONSTANT DEFINITIONS */

// Buttons
#define SW0 PA0
#define SW1 PA1
#define SW2 PA2
#define SW3 PA3

// Potentiometers
#define POT0 PA5
#define POT1 PA6

// RG LED
#define RGLED_R PB10
#define RGLED_G PB11

// Temperature sensor
#define TS_SCL PF6
#define TS_SDA PF7
#define TS_READ_ADDRESS 0x48

// EEPROM
// Instructions and registers
#define EEPROM_MEM_SIZE 8192 // EEPROM memory capacity (bytes)
#define EEPROM_WREN 0x06 // write enable instruction
#define EEPROM_WRDI 0x04 // write disable instruction
#define EEPROM_RDSR 0x05 // read status register instruction
#define EEPROM_WRSR 0x01 // write status register
#define EEPROM_READ 0x03 // read instruction
#define EEPROM_WRITE 0x02 // write instruction
#define EEPROM_SR_WPEN 0x80 // Status register write protect pin enable
#define EEPROM_SR_BP 0x0C // Status register block protection
#define EEPROM_SR_BP_1 0x08 // Status register block protection bit 1
#define EEPROM_SR_BP_0 0x04 // Status register block protection bit 0
#define EEPROM_SR_WEL 0x02 // Status register write enable latch (read only)
#define EEPROM_SR_WIP 0x01 // Status register write in progress (read only)
// Pins
#define EEPROM_MOSI PB15
#define EEPROM_MISO PB14
#define EEPROM_SCK PB13
#define EEPROM_CS PB12

/* FUNCTIONS */

void init_peripherals(); // Initialise all the board peripherals

void init_tempSensor(); // Initialise I2C for the temperature sensor

void init_EEPROM(); // Initialise SPI for the EEPROM

void ledWrite(uint8_t pattern); // Displays a pattern on the red LEDs on the board
void rgLedWrite(uint8_t red, uint8_t green); // Sets the colour of the RG led with 2 8-bit pwm values
uint8_t tempSensorRead(); // Reads a value from the temperature sensor
void eepromWrite(uint16_t address, uint8_t data); // Writes a byte of data to an address in the EEPROM
uint8_t eepromRead(uint16_t address); // Reads a byte of data from an address in the EEPROM