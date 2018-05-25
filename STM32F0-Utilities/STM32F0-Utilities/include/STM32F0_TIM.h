#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 30/04/2018
Date modified: 25/05/2018

Module: TIM
Functions for using the timers (re-write)

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

#ifndef STM32F0_INTERRUPTS_H
#include "STM32F0_INTERRUPTS.h"
#define STM32F0_INTERRUPTS_H
#endif

#ifndef STM32F0_OTHER_H
#include "STM32F0_OTHER.h"
#define STM32F0_OTHER_H
#endif

/* CONSTANT DEFINITIONS */


/* FUNCTIONS */

int __validPWMChannel(TIM_TypeDef* timer, uint8_t channel); // Returns (boolean) whether the specified timer has the specified output capture/compare channel (PWM)

void init_timer(TIM_TypeDef* timer, uint16_t prescaler); // Initialises a timer and sets its prescaler (Use prescaler = 0 if timer is going to be used for PWM)

void configure_PWM(TIM_TypeDef* timer); // Configures a timer to be used for output capture/compare (PWM)

// Timing functions
void startTimer(TIM_TypeDef* timer, uint32_t ticks); // Starts a timer to count up to <ticks>
void startRepeatingTimer(TIM_TypeDef* timer, uint32_t ticks); // Starts a timer to repeatedly count up to <ticks>
void stopTimer(TIM_TypeDef* timer); // Stops a timer (causes an update event and triggers the interrupt if enabled)
uint32_t queryTimer(TIM_TypeDef* timer); // Returns the current status of a timer (number of ticks counted)
int timerComplete(TIM_TypeDef* timer); // Returns whether a timer has completed (boolean)
void clearStatusFlag(TIM_TypeDef* timer); // Clears the status flag of timer completion
void timerInterruptEnable(TIM_TypeDef* timer, uint8_t priority); // Enables a timers update interrupt (triggered when timer completes) (remember to write your IRQHandler)

// PWM functions
void pwmEnable(TIM_TypeDef* timer, uint8_t channel); // Enables output capture/compare (PWM) on a timer channel
void pwmDisable(TIM_TypeDef* timer, uint8_t channel); // Disables output capture/compare (PWM) on a timer channel
void pwmWrite(TIM_TypeDef* timer, uint8_t channel, uint8_t dutyCycle); // Sets the PWM duty cycle on a timer channel