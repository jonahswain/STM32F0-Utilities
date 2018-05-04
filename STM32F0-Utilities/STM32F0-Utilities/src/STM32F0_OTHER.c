/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 17/04/2018
Date modified: 17/04/2018

Module: OTHER

*/

/* INCLUDES */

#ifndef STM32F0_OTHER_H
#include "STM32F0_OTHER.h"
#define STM32F0_OTHER_H
#endif

/* FUNCTIONS */

void __cpuHoldDelay(uint32_t uS) {
	// Holds the CPU in a loop for the specified duration in microseconds
	volatile unsigned int cnt;
	uS *= (HSI_VALUE / 2000000); // Scale microsecond value to be the number of required for loop iterations
	for (cnt = 0; cnt < uS; cnt++); // Hold in a loop
}