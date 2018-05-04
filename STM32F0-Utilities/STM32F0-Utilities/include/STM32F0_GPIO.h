#pragma once
/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 11/04/2018
Date modified: 30/04/2018

Module: GPIO
Definitions for the GPIO pins and functions to manipulate them

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

/* CONSTANT DEFINITIONS */

typedef struct {
	// A type definition for IO pins, in terms of their port (address) and their pin number within the port
	GPIO_TypeDef* port;
	uint8_t pin;
} IOPin_TypeDef;

// GPIO Port A
#define PA0 &((IOPin_TypeDef){GPIOA, (uint8_t)0})
#define PA1 &((IOPin_TypeDef){GPIOA, (uint8_t)1})
#define PA2 &((IOPin_TypeDef){GPIOA, (uint8_t)2})
#define PA3 &((IOPin_TypeDef){GPIOA, (uint8_t)3})
#define PA4 &((IOPin_TypeDef){GPIOA, (uint8_t)4})
#define PA5 &((IOPin_TypeDef){GPIOA, (uint8_t)5})
#define PA6 &((IOPin_TypeDef){GPIOA, (uint8_t)6})
#define PA7 &((IOPin_TypeDef){GPIOA, (uint8_t)7})
#define PA8 &((IOPin_TypeDef){GPIOA, (uint8_t)8})
#define PA9 &((IOPin_TypeDef){GPIOA, (uint8_t)9})
#define PA10 &((IOPin_TypeDef){GPIOA, (uint8_t)10})
#define PA11 &((IOPin_TypeDef){GPIOA, (uint8_t)11})
#define PA12 &((IOPin_TypeDef){GPIOA, (uint8_t)12})
#define PA13 &((IOPin_TypeDef){GPIOA, (uint8_t)13})
#define PA14 &((IOPin_TypeDef){GPIOA, (uint8_t)14})
#define PA15 &((IOPin_TypeDef){GPIOA, (uint8_t)15})

// GPIO Port B
#define PB0 &((IOPin_TypeDef){GPIOB, (uint8_t)0})
#define PB1 &((IOPin_TypeDef){GPIOB, (uint8_t)1})
#define PB2 &((IOPin_TypeDef){GPIOB, (uint8_t)2})
#define PB3 &((IOPin_TypeDef){GPIOB, (uint8_t)3})
#define PB4 &((IOPin_TypeDef){GPIOB, (uint8_t)4})
#define PB5 &((IOPin_TypeDef){GPIOB, (uint8_t)5})
#define PB6 &((IOPin_TypeDef){GPIOB, (uint8_t)6})
#define PB7 &((IOPin_TypeDef){GPIOB, (uint8_t)7})
#define PB8 &((IOPin_TypeDef){GPIOB, (uint8_t)8})
#define PB9 &((IOPin_TypeDef){GPIOB, (uint8_t)9})
#define PB10 &((IOPin_TypeDef){GPIOB, (uint8_t)10})
#define PB11 &((IOPin_TypeDef){GPIOB, (uint8_t)11})
#define PB12 &((IOPin_TypeDef){GPIOB, (uint8_t)12})
#define PB13 &((IOPin_TypeDef){GPIOB, (uint8_t)13})
#define PB14 &((IOPin_TypeDef){GPIOB, (uint8_t)14})
#define PB15 &((IOPin_TypeDef){GPIOB, (uint8_t)15})

// GPIO Port C
#define PC0 &((IOPin_TypeDef){GPIOC, (uint8_t)0})
#define PC1 &((IOPin_TypeDef){GPIOC, (uint8_t)1})
#define PC2 &((IOPin_TypeDef){GPIOC, (uint8_t)2})
#define PC3 &((IOPin_TypeDef){GPIOC, (uint8_t)3})
#define PC4 &((IOPin_TypeDef){GPIOC, (uint8_t)4})
#define PC5 &((IOPin_TypeDef){GPIOC, (uint8_t)5})
#define PC6 &((IOPin_TypeDef){GPIOC, (uint8_t)6})
#define PC7 &((IOPin_TypeDef){GPIOC, (uint8_t)7})
#define PC8 &((IOPin_TypeDef){GPIOC, (uint8_t)8})
#define PC9 &((IOPin_TypeDef){GPIOC, (uint8_t)9})
#define PC10 &((IOPin_TypeDef){GPIOC, (uint8_t)10})
#define PC11 &((IOPin_TypeDef){GPIOC, (uint8_t)11})
#define PC12 &((IOPin_TypeDef){GPIOC, (uint8_t)12})
#define PC13 &((IOPin_TypeDef){GPIOC, (uint8_t)13})
#define PC14 &((IOPin_TypeDef){GPIOC, (uint8_t)14})
#define PC15 &((IOPin_TypeDef){GPIOC, (uint8_t)15})

// GPIO Port D
#define PD0 &((IOPin_TypeDef){GPIOD, (uint8_t)0})
#define PD1 &((IOPin_TypeDef){GPIOD, (uint8_t)1})
#define PD2 &((IOPin_TypeDef){GPIOD, (uint8_t)2})
#define PD3 &((IOPin_TypeDef){GPIOD, (uint8_t)3})
#define PD4 &((IOPin_TypeDef){GPIOD, (uint8_t)4})
#define PD5 &((IOPin_TypeDef){GPIOD, (uint8_t)5})
#define PD6 &((IOPin_TypeDef){GPIOD, (uint8_t)6})
#define PD7 &((IOPin_TypeDef){GPIOD, (uint8_t)7})
#define PD8 &((IOPin_TypeDef){GPIOD, (uint8_t)8})
#define PD9 &((IOPin_TypeDef){GPIOD, (uint8_t)9})
#define PD10 &((IOPin_TypeDef){GPIOD, (uint8_t)10})
#define PD11 &((IOPin_TypeDef){GPIOD, (uint8_t)11})
#define PD12 &((IOPin_TypeDef){GPIOD, (uint8_t)12})
#define PD13 &((IOPin_TypeDef){GPIOD, (uint8_t)13})
#define PD14 &((IOPin_TypeDef){GPIOD, (uint8_t)14})
#define PD15 &((IOPin_TypeDef){GPIOD, (uint8_t)15})

// GPIO Port E
#define PE0 &((IOPin_TypeDef){GPIOE, (uint8_t)0})
#define PE1 &((IOPin_TypeDef){GPIOE, (uint8_t)1})
#define PE2 &((IOPin_TypeDef){GPIOE, (uint8_t)2})
#define PE3 &((IOPin_TypeDef){GPIOE, (uint8_t)3})
#define PE4 &((IOPin_TypeDef){GPIOE, (uint8_t)4})
#define PE5 &((IOPin_TypeDef){GPIOE, (uint8_t)5})
#define PE6 &((IOPin_TypeDef){GPIOE, (uint8_t)6})
#define PE7 &((IOPin_TypeDef){GPIOE, (uint8_t)7})
#define PE8 &((IOPin_TypeDef){GPIOE, (uint8_t)8})
#define PE9 &((IOPin_TypeDef){GPIOE, (uint8_t)9})
#define PE10 &((IOPin_TypeDef){GPIOE, (uint8_t)10})
#define PE11 &((IOPin_TypeDef){GPIOE, (uint8_t)11})
#define PE12 &((IOPin_TypeDef){GPIOE, (uint8_t)12})
#define PE13 &((IOPin_TypeDef){GPIOE, (uint8_t)13})
#define PE14 &((IOPin_TypeDef){GPIOE, (uint8_t)14})
#define PE15 &((IOPin_TypeDef){GPIOE, (uint8_t)15})

// GPIO Port F
#define PF0 &((IOPin_TypeDef){GPIOF, (uint8_t)0})
#define PF1 &((IOPin_TypeDef){GPIOF, (uint8_t)1})
#define PF2 &((IOPin_TypeDef){GPIOF, (uint8_t)2})
#define PF3 &((IOPin_TypeDef){GPIOF, (uint8_t)3})
#define PF4 &((IOPin_TypeDef){GPIOF, (uint8_t)4})
#define PF5 &((IOPin_TypeDef){GPIOF, (uint8_t)5})
#define PF6 &((IOPin_TypeDef){GPIOF, (uint8_t)6})
#define PF7 &((IOPin_TypeDef){GPIOF, (uint8_t)7})
#define PF8 &((IOPin_TypeDef){GPIOF, (uint8_t)8})
#define PF9 &((IOPin_TypeDef){GPIOF, (uint8_t)9})
#define PF10 &((IOPin_TypeDef){GPIOF, (uint8_t)10})
#define PF11 &((IOPin_TypeDef){GPIOF, (uint8_t)11})
#define PF12 &((IOPin_TypeDef){GPIOF, (uint8_t)12})
#define PF13 &((IOPin_TypeDef){GPIOF, (uint8_t)13})
#define PF14 &((IOPin_TypeDef){GPIOF, (uint8_t)14})
#define PF15 &((IOPin_TypeDef){GPIOF, (uint8_t)15})

// GPIO keywords
#define LOW 0
#define HIGH 1

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1
#define GPIO_ALTFN 2
#define GPIO_ANALOG 3

#define GPIO_PUSH_PULL 0
#define GPIO_OPEN_DRAIN 1

#define GPIO_FLOATING 0
#define GPIO_PULLUP 1
#define GPIO_PULLDOWN 2

// For alternate function mappings refer to STM32F051 datasheet, page 37
#define GPIO_AF0 0 // TIM14 (PB1), TIM15 (PA[2, 3, 9]), TIM17 (PA10), SPI (PA[4..7, 15], PB[3..5, 12..15]), USART (PB[6, 7]), CEC (PB[8, 10]), IR (PB9), MCO (PA8), SWDIO (PA13), SWCLK (PA14), Event out (PA[1, 11, 12], PB[0, 11])
#define GPIO_AF1 1 // TIM3 (PA[6, 7], PB[0, 1, 4, 5]), TIM15 (PB[14, 15]), I2C (PB[6..11]), USART (PA[0..4, 8..12, 14, 15]), CEC (PA5), IR (PA13), Event out (PB[3, 12])
#define GPIO_AF2 2 // TIM1 (PA[6..12], PB[0, 1, 12..15]), TIM2 (PA[0..3, 5, 15], PB[3, 10, 11]), TIM16 (PB[5, 6, 8]), TIM17 (PB[7, 9]), Event out (PB4)
#define GPIO_AF3 3 // Capacitive Touch Sense (PA[0..7, 9..12], PB[0..4, 6..8, 10..14]), I2C (PB5), TIM15 (PB15), Event out (PA[8, 15], PB9)
#define GPIO_AF4 4 // TIM14 (PA[4, 7])
#define GPIO_AF5 5 // TIM16 (PA6), TIM17 (PA7)
#define GPIO_AF6 6 // Event out (PA[6, 7])
#define GPIO_AF7 7 // Comparator (PA[0, 3, 6, 7, 11, 12])


/* FUNCTIONS */

void init_GPIOA(); // Enables the clock for GPIO port A
void init_GPIOB(); // Enables the clock for GPIO port B
void init_GPIOC(); // Enables the clock for GPIO port C
void init_GPIOD(); // Enables the clock for GPIO port D
void init_GPIOE(); // Enables the clock for GPIO port E
void init_GPIOF(); // Enables the clock for GPIO port F
void init_STD_GPIO(); // Enables the clock for GPIO ports A, B, C, and F (those available on the STM32F051)
void init_ALL_GPIO(); // Enables the clock for all GPIO ports

int pinMatches(IOPin_TypeDef* iopin1, IOPin_TypeDef* iopin2); // Returns whether 2 GPIO pins are the same (boolean)

void pinMode(IOPin_TypeDef* iopin, int mode); // Sets the mode of a GPIO pin (input/output/alternate function/analog)
void pinState(IOPin_TypeDef* iopin, int state); // Sets the pin input state (floating/pullup/pulldown)
void pinOutputType(IOPin_TypeDef* iopin, int type); // Sets the pin type (push-pull/open drain)
void afSelect(IOPin_TypeDef* iopin, int altfn); // Maps a GPIO pin to one of its alternate functions (note: not all alternate functions are supported on all GPIO ports/pins)

int digitalRead(IOPin_TypeDef* iopin); // Returns the current input state of a GPIO pin (high/low)
void digitalWrite(IOPin_TypeDef* iopin, int state); // Sets the output state of a GPIO pin (high/low)