/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 30/04/2018
Date modified: 01/05/2018

Module: TIM
Functions for using the timers (re-write)

*/

/* INCLUDES */

#ifndef STM32F0_TIM_H
#include "STM32F0_TIM.h"
#define STM32F0_TIM_H
#endif

/* FUNCTIONS */

int __validPWMChannel(TIM_TypeDef* timer, uint8_t channel) {
	// Returns (boolean) whether the specified timer has the specified output capture/compare channel (PWM)
	if (timer == TIM1) {
		return ((channel >= 1) && (channel <= 4)); // TIM1 - supported channels 1-4
	}
	else if (timer == TIM2) {
		return ((channel >= 1) && (channel <= 4)); // TIM2 - supported channels 1-4
	}
	else if (timer == TIM3) {
		return ((channel >= 1) && (channel <= 4)); // TIM3 - supported channels 1-4
	}
	else if (timer == TIM14) {
		return (channel == 1); // TIM14 - supported channels 1
	}
	else if (timer == TIM15) {
		return ((channel == 1) || (channel == 2)); // TIM15 - supported channels 1-2
	}
	else if (timer == TIM16) {
		return (channel == 1); // TIM16 - supported channels 1
	}
	else if (timer == TIM17) {
		return (channel == 1); // TIM17 - supported channels 1
	}

	// TIM6 and TIM7 have no output capture/compare (PWM) support

	return 0; // By default return false
}

void init_timer(TIM_TypeDef* timer, uint16_t prescaler) {
	// Initialises a timer and sets its prescaler (Use prescaler = 0 if timer is going to be used for PWM)
	if (timer == TIM1) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Enable the clock for TIM1
	}
	else if (timer == TIM2) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable the clock for TIM2
	}
	else if (timer == TIM3) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Enable the clock for TIM3
	}
	else if (timer == TIM6) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; // Enable the clock for TIM6
	}
	else if (timer == TIM7) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; // Enable the clock for TIM7
	}
	else if (timer == TIM14) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM14EN; // Enable the clock for TIM14
	}
	else if (timer == TIM15) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; // Enable the clock for TIM15
	}
	else if (timer == TIM16) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; // Enable the clock for TIM16
	}
	else if (timer == TIM17) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM17EN; // Enable the clock for TIM17
	}

	timer->PSC = prescaler; // Set the timer's prescaler

}

void configure_PWM(TIM_TypeDef* timer) {
	// Configures a timer to be used for output capture/compare (PWM)
	init_timer(timer, 0); // Initialise the timer with prescaler 0
	
	timer->ARR = 0xFF; // Set the auto-reload register value (sets frequency)

	if ((timer == TIM1) || (timer == TIM2) || (timer == TIM3)) {
		// Case for timers with 4 channels
		timer->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // Set CH1 to PWM mode 1
		timer->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); // Set CH2 to PWM mode 1
		timer->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // Set CH3 to PWM mode 1
		timer->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // Set CH4 to PWM mode 1
	}
	else if (timer == TIM15) {
		// Case for timers with 2 channels
		timer->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // Set CH1 to PWM mode 1
		timer->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); // Set CH2 to PWM mode 1
	}
	else if ((timer == TIM14) || (timer == TIM16) || (timer == TIM17)) {
		// Case for timers with 1 channel
		timer->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // Set CH1 to PWM mode 1
	} // Ignore cases where timer has no PWM functionality

	timer->CR1 |= TIM_CR1_CEN; // Enable the counter
}


// Timing functions
void startTimer(TIM_TypeDef* timer, uint32_t ticks) {
	// Starts a timer to count up to <ticks>
	// Note that only TIM2 supports a full 32 bit wide counter, so MSBs are chopped for other timers
	timer->CR1 &= ~TIM_CR1_CEN; // Disable the timer if enabled
	timer->CR1 |= (TIM_CR1_OPM | TIM_CR1_URS); // Enable One-Pulse-Mode (stop timer on completion), set Update Request Source to overflow only
	if (timer == TIM2) {
		// TIM2 supports full 32 bit counter/ARR
		timer->ARR = ticks; // Set the auto-reload value (value to count up to)
	}
	else {
		// Timer only supports 16 bit wide count - chop MSBs
		timer->ARR = (uint16_t)ticks; // Set the auto-reload value (value to count up to)
	}
	timer->EGR |= TIM_EGR_UG; // Reset the timer
	timer->SR &= ~TIM_SR_UIF; // Clear the status flag
	timer->CR1 |= TIM_CR1_CEN; // Enable the counter
}

void startRepeatingTimer(TIM_TypeDef* timer, uint32_t ticks) {
	// Starts a timer to repeatedly count up to <ticks>
	// Note that only TIM2 supports a full 32 bit wide counter, so MSBs are chopped for other timers
	timer->CR1 &= ~TIM_CR1_CEN; // Disable the timer if enabled
	timer->CR1 &= ~TIM_CR1_OPM; // Disables One-Pulse-Mode (timer starts counting again when complete)
	timer->CR1 |= TIM_CR1_URS; // Set Update Request Source to overflow only
	if (timer == TIM2) {
		// TIM2 supports full 32 bit counter/ARR
		timer->ARR = ticks; // Set the auto-reload value (value to count up to)
	}
	else {
		// Timer only supports 16 bit wide count - chop MSBs
		timer->ARR = (uint16_t)ticks; // Set the auto-reload value (value to count up to)
	}
	timer->EGR |= TIM_EGR_UG; // Reset the timer
	timer->SR &= ~TIM_SR_UIF; // Clear the status flag
	timer->CR1 |= TIM_CR1_CEN; // Enable the counter
}

uint32_t queryTimer(TIM_TypeDef* timer) {
	// Returns the current status of a timer (number of ticks counted)
	if (timer == TIM2) {
		// TIM2 supports full 32 bit wide counter
		return timer->CNT; // Return the counter
	}
	else {
		// Timer only supports 16 bit wide counter
		return (0x0000FFFF & timer->CNT); // Return the counter without MSBs
	}
}

int timerComplete(TIM_TypeDef* timer) {
	// Returns whether a timer has completed (boolean)
	return (timer->SR & TIM_SR_UIF); // Return the UIF status bit of the timer
}

void clearStatusFlag(TIM_TypeDef* timer) {
	// Clears the status flag of timer completion
	timer->SR &= ~TIM_SR_UIF; // Clear the UIF status bit of the timer
}

void timerInterruptEnable(TIM_TypeDef* timer, uint8_t priority) {
	// Enables a timers update interrupt (triggered when timer completes)
	timer->DIER |= TIM_DIER_UIE; // Enable update interrupt generation on the timer

	int irqn = -1; // Get IRQn (see stm32f0xx.h for IRQns
	if (timer == TIM1) {
		irqn = 13;
	}
	else if (timer == TIM2) {
		irqn = 15;
	}
	else if (timer == TIM3) {
		irqn = 16;
	}
	else if (timer == TIM6) {
		irqn = 17;
	}
	else if (timer == TIM14) {
		irqn = 19;
	}
	else if (timer == TIM15) {
		irqn = 20;
	}
	else if (timer == TIM16) {
		irqn = 21;
	}
	else if (timer == TIM17) {
		irqn = 22;
	}

	nvicSetPriority(irqn, priority); // Set the interrupt priority in the NVIC
	nvicEnableInterrupt(irqn); // Enable the interrupt in the NVIC

}


// PWM functions
void pwmEnable(TIM_TypeDef* timer, uint8_t channel) {
	// Enables output capture/compare (PWM) on a timer channel
	if (__validPWMChannel(timer, channel)) {
		// Only enable PWM on timers/channels that support it
		timer->CCER |= (1 << (4 * (channel - 1))); // Set the corresponding bit in the Capture/Compare Enable Register
	}
}

void pwmDisable(TIM_TypeDef* timer, uint8_t channel) {
	// Disables output capture/compare (PWM) on a timer channel
	if (__validPWMChannel(timer, channel)) {
		// Only disable PWM on timers/channels that support it
		timer->CCER &= ~(1 << (4 * (channel - 1))); // Set the corresponding bit in the Capture/Compare Enable Register
	}
}

void pwmWrite(TIM_TypeDef* timer, uint8_t channel, uint8_t dutyCycle) {
	// Sets the PWM duty cycle on a timer channel
	if (__validPWMChannel(timer, channel)) {
		// Only set duty cycle on timers/channels that support it
		if (channel == 1) {
			timer->CCR1 = dutyCycle; // Set the duty cycle for channel 1 in CCR1
		}
		else if (channel == 2) {
			timer->CCR2 = dutyCycle; // Set the duty cycle for channel 2 in CCR2
		}
		else if (channel == 3) {
			timer->CCR3 = dutyCycle; // Set the duty cycle for channel 3 in CCR3
		}
		else if (channel == 4) {
			timer->CCR4 = dutyCycle; // Set the duty cycle for channel 4 in CCR4
		}
	}
}
