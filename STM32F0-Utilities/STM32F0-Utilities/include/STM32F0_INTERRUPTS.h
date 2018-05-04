#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: /2018
Date modified: /2018

Module: Interrupts
Functions to configure interrupts as well as interrupt handlers

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
#include "STM32F0_GPIO.h"
#define STM32F0_OTHER_H
#endif

#ifndef STM32F0_OTHER_H
#include "STM32F0_OTHER.h"
#define STM32F0_OTHER_H
#endif

/* CONSTANT DEFINITIONS */
typedef struct {
	volatile uint32_t ISER[1]; // Interrupt Set Enable Register (Offset: 0x000)
	uint32_t RESERVED0[31]; // Reserved
	volatile uint32_t ICER[1]; // Interrupt Clear Enable Register (Offset: 0x080)
	uint32_t RSERVED1[31]; // Reserved
	volatile uint32_t ISPR[1]; // Interrupt Set Pending Register (Offset: 0x100)
	uint32_t RESERVED2[31]; // Reserved
	volatile uint32_t ICPR[1]; // Interrupt Clear Pending Register (Offset: 0x180)
	uint32_t RESERVED3[31]; // Reserved
	uint32_t RESERVED4[64]; // Reserved
	volatile uint32_t IP[8]; // Interrupt Priority Register (Offset: 0x300)
} NVIC_TypeDef; // A Type definition for the NVIC to map its registers

#define NVIC ((NVIC_TypeDef*)0xE000E100) // Set NVIC to point to the NVIC registers

/* FUNCTIONS */

void init_SYSCFG(); // Enables the clock for the SYSCFG module (needed for interrupts)

void nvicSetPriority(int irqn, uint8_t priority); // Sets the priority of an interrupt in the NVIC from 0 (highest) to 255 (lowest)
void nvicEnableInterrupt(int irqn); // Enable an interrupt in the NVIC
void nvicDisableInterrupt(int irqn); // Disable an interrupt in the NVIC

void pinInterruptEnable(IOPin_TypeDef* iopin, int risingEdgeEnable, int fallingEdgeEnable, int priority); // Enables a pin change interrupt on the specified edge/s
/*
risingEdgeEnable - whether to trigger on a rising edge
fallingEdgeEnable - whether to trigger on a falling edge
priority - the interrupt priority from 0 (highest) to 3 (lowest), 3 is recommended
NOTE: Due to the internal configuration of the EXTI lines and the NVIC (they are multiplexed), only 1 port per line can be selected
E.g. if PA0 has an interrupt enabled, PB0 can not have an interrupt enabled, so enabling an interrupt for PB0 will disable the interrupt for PA0 and vice versa (also applies to PC0, PD0, PE0, and PF0)
*/

void clearPendingInterruptFlag(int interrupt); // Clears the pending interrupt flag for a specific interrupt number

void pinInterruptTriggered(IOPin_TypeDef* iopin); // This function is called by default when a pin interrupt is triggered, with the triggering pin as an argument
// Implement this function in your main.c (or wherever) as an easy interrupt handler

// Interrupt handlers
void EXTI0_1_IRQHandler(); // Interrupt handler for EXTI 0-1
void EXTI2_3_IRQHandler(); // Interrupt handler for EXTI 2-3
void EXTI4_15_IRQHandler(); // Interrupt handler for EXTI 4-15