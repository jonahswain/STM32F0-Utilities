#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 26/05/2018
Date modified: 26/05/2018

Module: DAC
Functions for operating the Digital to Analog converter

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
#define DAC_BUFFER_ENABLE 1
#define DAC_BUFFER_DISABLE 0

#define DAC_TRIGGER_ENABLE 1
#define DAC_TRIGGER_DISABLE 0

#define DAC_MODE_RESOLUTION 0x1 // Bit 0 is resolution bit in functions
#define DAC_MODE_DATAALIGNMENT 0x2 // Bit 1 is data alignment bit in functions
#define DAC_MODE_8BIT 0
#define DAC_MODE_12BIT 1
#define DAC_MODE_RIGHTALIGN 0
#define DAC_MODE_LEFTALIGN 2
// Please note: 8-bit left aligned data is not permitted. 8-bit data is automatically treated as right-aligned.

// Trigger sources
#define DAC_TRIGGER_TIM6 0x0 // TIM6 TRGO event
#define DAC_TRIGGER_TIM3 0x1 // TIM3 TRGO event
#define DAC_TRIGGER_TIM7 0x2 // TIM7 TRGO event
#define DAC_TRIGGER_TIM15 0x3 // TIM15 TRGO event
#define DAC_TRIGGER_TIM2 0x4 // TIM2 TRGO event
#define DAC_TRIGGER_EXTI 0x6 // EXTI line 9 interrupt event
#define DAC_TRIGGER_SOFTWARE 0x7 // Software trigger

/* FUNCTIONS */
void init_DAC(uint8_t channel, uint8_t buffer, uint8_t triggerEnable, uint8_t triggerSource); // Initialises and configures  DAC channel
/*
channel - the DAC channel to configure
buffer - whether to use the internal buffer to increase current drive
triggerEnable - whether to wait for trigger to change output
triggerSource - the source of the trigger
*/

void analogWrite(uint8_t channel, uint8_t value); // Outputs an 8 bit analog value using the DAC

void dacValueOut(uint8_t channel, uint16_t value, uint8_t mode); // Outputs an analog value using the DAC
/*
channel - the DAC channel to output on
value - the value to output on the DAC
mode - 8/12 bit, left/right aligned
*/

void dacWaveOut(uint8_t channel, uint16_t* values, uint8_t mode, uint16_t length); // Outputs a series of analog values in sequence using the DAC
/*
channel - the DAC channel to output on
values - a pointer to an array of analog values to output
mode - 8/12 bit, left/right aligned
length - the number of values in the array
*/

void dacWaveGen(uint8_t channel, uint16_t* values, uint8_t mode, uint16_t length, uint32_t repetitions); // Outputs a series of analog values multiple times using the DAC
/*
channel - the DAC channel to output on
values -  a pointer to an array of analog values to output
mode - 8/12 bit, left/right aligned
length - the number of values in the array
repetitions - the number of times to generate the waveform
*/

void dacDMAWaveGen(uint8_t channel, uint16_t* values, uint8_t mode, uint16_t length, uint16_t period); // Enables waveform generation using DMA (repeats forever...)
/*
NOTE: Uses DMA channel 2 and TIM17 for DAC channel 1 and DMA channel 3 and TIM16 for DAC channel 2
channel - the DAC channel to output on
values - a pointer to an array of analog values to output
mode - 8/12 bit, left/right aligned
length - the number of values in the array
period - the approximate delay between values (in microseconds)
*/

void dacDMAWaveGenDisable(uint8_t channel); // Disables waveform generation using DMA