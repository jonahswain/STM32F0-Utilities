#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 17/04/2018
Date modified: 17/04/2018

Module: ADC
Functions to use the analog to digital converter

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
// ADC resolutions
#define ADC_12BIT 0
#define ADC_10BIT 1
#define ADC_8BIT 2
#define ADC_6BIT 3

/* FUNCTIONS */

void init_ADC(int resolution); // Initialise and calibrate the ADC
int analogRead(IOPin_TypeDef* iopin); // Read an analog value from a pin
int analogReadChannel(int channel); // Read an analog value from a specific ADC channel