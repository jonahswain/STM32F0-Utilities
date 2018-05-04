/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: /2018
Date modified: /2018

Module: Interrupts

*/

/* INCLUDES */

#ifndef STM32F0_INTERRUPTS_H
#include "STM32F0_INTERRUPTS.h"
#define STM32F0_INTERRUPTS_H
#endif

/* FUNCTIONS */

void init_SYSCFG() {
	// Enables the clock for the SYSCFG module (needed for interrupts)
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
}

void nvicSetPriority(int irqn, uint8_t priority) {
	// Sets the priority of an interrupt in the NVIC from 0 (highest) to 255 (lowest)
	if (irqn >= 0) { // Handle only the maskable interrupts
		uint8_t NVIC_IP_INDEX = irqn / 4; // Establish which interrupt priority register we are writing to
		uint8_t NVIC_IP_OFFSET = (irqn % 4) * 8; // Establish the bit shift needed within the IP register

		NVIC->IP[NVIC_IP_INDEX] &= ~(0xFF << NVIC_IP_OFFSET); // Reset the bits in the IP register
		NVIC->IP[NVIC_IP_INDEX] |= ((priority & 0xC0) << NVIC_IP_OFFSET); // Set the bits in the IP register
	}
}

void nvicEnableInterrupt(int irqn) {
	// Enable an interrupt in the NVIC
	if (irqn >= 0) { // Handle only the maskable interrupts
		NVIC->ISER[0] |= (1 << irqn); // Set the corresponding bit in the NVIC Interrupt Set Enable Register
	}
}

void nvicDisableInterrupt(int irqn) {
	// Disable an interrupt in the NVIC
	if (irqn >= 0) { // // Handle only the maskable interrupts
		NVIC->ICER[0] |= (1 << irqn); // Set the corresponding bit in the NVIC Interrupt Clear Enable Register
	}
}


void pinInterruptEnable(IOPin_TypeDef* iopin, int risingEdgeEnable, int fallingEdgeEnable, int priority) {
	// Enables a pin change interrupt on the specified edge/s
	uint8_t EXTICRx_bits = 0; // The configuration bits to place in the EXTI control register
	if (iopin->port == GPIOA) {
		EXTICRx_bits = 0b000; // Set the configuration bits for GPIOA
	}
	else if (iopin->port == GPIOB) {
		EXTICRx_bits = 0b001; // Set the configuration bits for GPIOB
	}
	else if (iopin->port == GPIOC) {
		EXTICRx_bits = 0b010; // Set the configuration bits for GPIOC
	}
	else if (iopin->port == GPIOD) {
		EXTICRx_bits = 0b011; // Set the configuration bits for GPIOD
	}
	else if (iopin->port == GPIOE) {
		EXTICRx_bits = 0b100; // Set the configuration bits for GPIOE
	}
	else if (iopin->port == GPIOF) {
		EXTICRx_bits = 0b101; // Set the configuration bits for GPIOF
	}


	// Configure the EXTI
	if (iopin->pin <= 3) {
		// Pins 0 to 3
		SYSCFG->EXTICR[0] &= ~(0b111 << ((iopin->pin % 4) * 4)); // Reset corresponding the EXTI control register bits
		SYSCFG->EXTICR[0] |= (EXTICRx_bits << ((iopin->pin % 4) * 4)); // Reset corresponding the EXTI control register bits
	}
	else if (iopin->pin <= 7) {
		// Pins 4 to 7
		SYSCFG->EXTICR[1] &= ~(0b111 << ((iopin->pin % 4) * 4)); // Reset corresponding the EXTI control register bits
		SYSCFG->EXTICR[1] |= (EXTICRx_bits << ((iopin->pin % 4) * 4)); // Reset corresponding the EXTI control register bits
	}
	else if (iopin->pin <= 11) {
		// Pins 8 to 11
		SYSCFG->EXTICR[2] &= ~(0b111 << ((iopin->pin % 4) * 4)); // Reset corresponding the EXTI control register bits
		SYSCFG->EXTICR[2] |= (EXTICRx_bits << ((iopin->pin % 4) * 4)); // Reset corresponding the EXTI control register bits
	}
	else if (iopin->pin <= 15) {
		// Pins 12 to 15
		SYSCFG->EXTICR[3] &= ~(0b111 << ((iopin->pin % 4) * 4)); // Reset corresponding the EXTI control register bits
		SYSCFG->EXTICR[3] |= (EXTICRx_bits << ((iopin->pin % 4) * 4)); // Reset corresponding the EXTI control register bits
	}


	// Set the edges (rising/falling) that trigger the interrupt
	if (risingEdgeEnable) {
		// Enable interrupt on rising edge
		EXTI->RTSR |= (1 << iopin->pin); // Enable the corresponding bit in the rising trigger selection register
	}
	else {
		EXTI->RTSR &= ~(1 << iopin->pin); // Disable the corresponding bit in the rising trigger selection register
	}

	if (fallingEdgeEnable) {
		// Enable interrupt on rising edge
		EXTI->FTSR |= (1 << iopin->pin); // Enable the corresponding bit in the falling trigger selection register
	}
	else {
		EXTI->FTSR &= ~(1 << iopin->pin); // Disable the corresponding bit in the falling trigger selection register
	}

	EXTI->IMR |= (1 << iopin->pin); // Set the corresponding bit to unmask the interrupt line


	int pin_irqn = 0; // IRQn for the pin interrupt
	if ((iopin->pin == 0) || (iopin->pin == 1)) {
		pin_irqn = 5; // EXTI0_1_IRQn
	}
	else if ((iopin->pin == 2) || (iopin->pin == 3)) {
		pin_irqn = 6; // EXTI2_3_IRQn
	}
	else {
		pin_irqn = 7; // EXTI4_15_IRQn
	}

	nvicSetPriority(pin_irqn, priority); // Set the interrupt priority in the NVIC
	nvicEnableInterrupt(pin_irqn); // Enable the interrupt in the NVIC

}

void clearPendingInterruptFlag(int interrupt) {
	// Clears the pending interrupt flag for a specific interrupt number
	if (((interrupt >= 0) && (interrupt <= 17)) || ((interrupt >= 19) && (interrupt <= 22))) { // Check if interrupt number is valid
		EXTI->PR |= (1 << interrupt); // Set the corresponding bit in the EXTI pending register
	}
}

// Interrupt Handlers
void EXTI0_1_IRQHandler() {
	// Interrupt handler for EXTI 0-1
	/* USER INTERRUPT HANDLER CODE */
	/* Remember to clear the corresponding pending interrupt flag with clearPendingInterruptFlag() when you have handled the interrupt */

	/* END OF USER INTERRUPT HANDLER CODE */

	// The following code calls pinInterruptTriggered(), remove it if you want to write your own
	IOPin_TypeDef* triggeringPin = &((IOPin_TypeDef) { 0, 0 });

	if (EXTI->PR & (1 << 0)) {
		// Line 0 triggered the interrupt
		triggeringPin->pin = 0;
	}
	else {
		// Line 1 triggered the interrupt
		triggeringPin->pin = 1;
	}

	if (((SYSCFG->EXTICR[0] >> (((triggeringPin->pin % 4) * 4) * 4) & 0b0111) == 0b0000)) {
		// Interrupt is configured on PAx
		triggeringPin->port = GPIOA;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0001)) {
		// Interrupt is configured on PBx
		triggeringPin->port = GPIOB;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0010)) {
		// Interrupt is configured on PCx
		triggeringPin->port = GPIOC;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0011)) {
		// Interrupt is configured on PDx
		triggeringPin->port = GPIOD;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0100)) {
		// Interrupt is configured on PEx
		triggeringPin->port = GPIOE;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0101)) {
		// Interrupt is configured on PFx
		triggeringPin->port = GPIOF;
	}

	pinInterruptTriggered(triggeringPin); // Call pinInterruptTriggered
	clearPendingInterruptFlag(triggeringPin->pin); // Clear the pending interrupt flag
}

void EXTI2_3_IRQHandler() {
	// Interrupt handler for EXTI 2-3
	/* USER INTERRUPT HANDLER CODE */
	/* Remember to clear the corresponding pending interrupt flag with clearPendingInterruptFlag() when you have handled the interrupt */

	/* END OF USER INTERRUPT HANDLER CODE */

	// The following code calls pinInterruptTriggered(), remove it if you want to write your own
	IOPin_TypeDef* triggeringPin = &((IOPin_TypeDef) { 0, 0 });

	if (EXTI->PR & (1 << 2)) {
		// Line 2 triggered the interrupt
		triggeringPin->pin = 2;
	}
	else {
		// Line 3 triggered the interrupt
		triggeringPin->pin = 3;
	}

	if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0000)) {
		// Interrupt is configured on PAx
		triggeringPin->port = GPIOA;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0001)) {
		// Interrupt is configured on PBx
		triggeringPin->port = GPIOB;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0010)) {
		// Interrupt is configured on PCx
		triggeringPin->port = GPIOC;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0011)) {
		// Interrupt is configured on PDx
		triggeringPin->port = GPIOD;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0100)) {
		// Interrupt is configured on PEx
		triggeringPin->port = GPIOE;
	}
	else if (((SYSCFG->EXTICR[0] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0101)) {
		// Interrupt is configured on PFx
		triggeringPin->port = GPIOF;
	}

	pinInterruptTriggered(triggeringPin); // Call pinInterruptTriggered
	clearPendingInterruptFlag(triggeringPin->pin); // Clear the pending interrupt flag
}

void EXTI4_15_IRQHandler() {
	// Interrupt handler for EXTI 4-15
	/* USER INTERRUPT HANDLER CODE */
	/* Remember to clear the corresponding pending interrupt flag with clearPendingInterruptFlag() when you have handled the interrupt */

	/* END OF USER INTERRUPT HANDLER CODE */

	// The following code calls pinInterruptTriggered(), remove it if you want to write your own
	IOPin_TypeDef* triggeringPin = &((IOPin_TypeDef) { 0, 0 });

	if (EXTI->PR & (1 << 4)) {
		// Line 4 triggered the interrupt
		triggeringPin->pin = 4;
	}
	else if (EXTI->PR & (1 << 5)) {
		// Line 5 triggered the interrupt
		triggeringPin->pin = 5;
	}
	else if (EXTI->PR & (1 << 6)) {
		// Line 6 triggered the interrupt
		triggeringPin->pin = 6;
	}
	else if (EXTI->PR & (1 << 7)) {
		// Line 7 triggered the interrupt
		triggeringPin->pin = 7;
	}
	else if (EXTI->PR & (1 << 8)) {
		// Line 8 triggered the interrupt
		triggeringPin->pin = 8;
	}
	else if (EXTI->PR & (1 << 9)) {
		// Line 9 triggered the interrupt
		triggeringPin->pin = 9;
	}
	else if (EXTI->PR & (1 << 10)) {
		// Line 10 triggered the interrupt
		triggeringPin->pin = 10;
	}
	else if (EXTI->PR & (1 << 11)) {
		// Line 11 triggered the interrupt
		triggeringPin->pin = 11;
	}
	else if (EXTI->PR & (1 << 12)) {
		// Line 12 triggered the interrupt
		triggeringPin->pin = 12;
	}
	else if (EXTI->PR & (1 << 13)) {
		// Line 13 triggered the interrupt
		triggeringPin->pin = 13;
	}
	else if (EXTI->PR & (1 << 14)) {
		// Line 14 triggered the interrupt
		triggeringPin->pin = 14;
	}
	else if (EXTI->PR & (1 << 15)) {
		// Line 15 triggered the interrupt
		triggeringPin->pin = 15;
	}

	if ((triggeringPin->pin / 4) == 2) {
		// Check EXTICR[1] for port
		if (((SYSCFG->EXTICR[1] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0000)) {
			// Interrupt is configured on PAx
			triggeringPin->port = GPIOA;
		}
		else if (((SYSCFG->EXTICR[1] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0001)) {
			// Interrupt is configured on PBx
			triggeringPin->port = GPIOB;
		}
		else if (((SYSCFG->EXTICR[1] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0010)) {
			// Interrupt is configured on PCx
			triggeringPin->port = GPIOC;
		}
		else if (((SYSCFG->EXTICR[1] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0011)) {
			// Interrupt is configured on PDx
			triggeringPin->port = GPIOD;
		}
		else if (((SYSCFG->EXTICR[1] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0100)) {
			// Interrupt is configured on PEx
			triggeringPin->port = GPIOE;
		}
		else if (((SYSCFG->EXTICR[1] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0101)) {
			// Interrupt is configured on PFx
			triggeringPin->port = GPIOF;
		}
	} 
	else if ((triggeringPin->pin / 4) == 3) {
		// Check EXTICR[2] for port
		if (((SYSCFG->EXTICR[2] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0000)) {
			// Interrupt is configured on PAx
			triggeringPin->port = GPIOA;
		}
		else if (((SYSCFG->EXTICR[2] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0001)) {
			// Interrupt is configured on PBx
			triggeringPin->port = GPIOB;
		}
		else if (((SYSCFG->EXTICR[2] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0010)) {
			// Interrupt is configured on PCx
			triggeringPin->port = GPIOC;
		}
		else if (((SYSCFG->EXTICR[2] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0011)) {
			// Interrupt is configured on PDx
			triggeringPin->port = GPIOD;
		}
		else if (((SYSCFG->EXTICR[2] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0100)) {
			// Interrupt is configured on PEx
			triggeringPin->port = GPIOE;
		}
		else if (((SYSCFG->EXTICR[2] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0101)) {
			// Interrupt is configured on PFx
			triggeringPin->port = GPIOF;
		}
	}
	else {
		// Check EXTICR[3] for port
		if (((SYSCFG->EXTICR[3] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0000)) {
			// Interrupt is configured on PAx
			triggeringPin->port = GPIOA;
		}
		else if (((SYSCFG->EXTICR[3] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0001)) {
			// Interrupt is configured on PBx
			triggeringPin->port = GPIOB;
		}
		else if (((SYSCFG->EXTICR[3] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0010)) {
			// Interrupt is configured on PCx
			triggeringPin->port = GPIOC;
		}
		else if (((SYSCFG->EXTICR[3] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0011)) {
			// Interrupt is configured on PDx
			triggeringPin->port = GPIOD;
		}
		else if (((SYSCFG->EXTICR[3] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0100)) {
			// Interrupt is configured on PEx
			triggeringPin->port = GPIOE;
		}
		else if (((SYSCFG->EXTICR[3] >> ((triggeringPin->pin % 4) * 4) & 0b0111) == 0b0101)) {
			// Interrupt is configured on PFx
			triggeringPin->port = GPIOF;
		}
	}

	pinInterruptTriggered(triggeringPin); // Call pinInterruptTriggered
	clearPendingInterruptFlag(triggeringPin->pin); // Clear the pending interrupt flag

}
