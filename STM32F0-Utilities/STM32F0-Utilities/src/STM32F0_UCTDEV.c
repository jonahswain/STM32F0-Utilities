/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 17/04/2018
Date modified: 30/04/2018

Module: UCTDEV

*/

/* INCLUDES */

#ifndef STM32F0_UCTDEV_H
#include "STM32F0_UCTDEV.h"
#define STM32F0_UCTDEV_H
#endif

/* FUNCTIONS */

void init_peripherals() {
	// Initialise all the board peripherals
	init_STD_GPIO(); // Initialise all the available GPIO ports
	init_timer(TIM6, 47999); // Initialise timer 6 to tick every ms
	init_tempSensor(); // Initialise the temperature sensor
	init_ADC(ADC_10BIT); // Initialise the ADC with 10 bit resolution
	init_SYSCFG(); // Initialise SYSCFG module (needed for interrupts)
	init_LCD(); // Initialise the LCD

	GPIOB->MODER |= 0x00005555; // Set the red LED pin modes to output

	pinMode(SW0, GPIO_INPUT); // Set SW0 to input
	pinState(SW0, GPIO_PULLUP); // Set SW0 to pullup
	pinMode(SW1, GPIO_INPUT); // Set SW1 to input
	pinState(SW1, GPIO_PULLUP); // Set SW1 to pullup
	pinMode(SW2, GPIO_INPUT); // Set SW2 to input
	pinState(SW2, GPIO_PULLUP); // Set SW2 to pullup
	pinMode(SW3, GPIO_INPUT); // Set SW3 to input
	pinState(SW3, GPIO_PULLUP); // Set SW3 to pullup

	pinMode(POT0, GPIO_ANALOG); // Set Pot 0 to analog
	pinMode(POT1, GPIO_ANALOG); // Set Pot 1 to analog

	configure_PWM(TIM2); // Enable PWM on TIM2
	pinMode(RGLED_R, GPIO_ALTFN); // Set the pin mode of the red part of the RG LED to alternate function
	pinMode(RGLED_G, GPIO_ALTFN); // Set the pin mode of the green part of the RG LED to alternate function
	afSelect(RGLED_R, GPIO_AF2); // Map the red part of the RG LED to TIM2 CH3
	afSelect(RGLED_G, GPIO_AF2); // Map the green part of the RG LED to TIM2 CH4
	pwmEnable(TIM2, 3); // Enable PWM output on TIM2 CH3
	pwmEnable(TIM2, 4); // Enable PWM output on TIM2 CH4

	//TODO add more
}

void init_tempSensor() {
	// Initialise I2C for the temperature sensor
	pinOutputType(TS_SCL, GPIO_OPEN_DRAIN); // Configure SCL as open drain
	pinMode(TS_SCL, GPIO_OUTPUT); // Set SCL as output
	for (int i = 0; i < 20; i++) {
		// If the micro was reset during data transfer, the temp sensor might be stuck trying to transmit data
		// Cycle the SCL (clock) 20 times to make the temp sensor clock out any remaining data
		__cpuHoldDelay(5); // Delay
		digitalWrite(TS_SCL, LOW); // Set SCL low
		__cpuHoldDelay(5); // Delay
		digitalWrite(TS_SCL, HIGH); // Set SCL high
	}

	pinMode(TS_SCL, GPIO_ALTFN); // Set SCL as alternate function (I2C2 SCL)
	pinOutputType(TS_SDA, GPIO_OPEN_DRAIN); // Configure SDA to open drain
	pinMode(TS_SDA, GPIO_ALTFN); // Set SCL as alternate function (I2C2 SDA)
	// PF6 & PF7 (SCL & SDA) have only 1 alternate function, I2C, so no need to use afSelect()

	init_I2C(I2C2, 1, 0xC7, 0xC2, 0x04, 0x02); // Initialise I2C2 with the required timing settings for the module
}

void ledWrite(uint8_t pattern) {
	// Displays a pattern on the red LEDs on the board
	GPIOB->ODR &= 0xFFFF & pattern; // Turn off the LEDs that need to be turned off
	GPIOB->ODR |= pattern; // Turn on the LEDs that need to be turned on
}

void rgLedWrite(uint8_t red, uint8_t green) {
	// Sets the colour of the RG led with 2 8-bit pwm values
	pwmWrite(TIM2, 3, red); // PWM on red LED
	pwmWrite(TIM2, 4, green); // PWN on green LED
}

uint8_t tempSensorRead() {
	// Reads a value from the temperature sensor
	unsigned char temperature = 0;
	i2cReadFromSlave(I2C2, &temperature, 1, I2C_7BIT_ADDRESSING, TS_READ_ADDRESS, TS_READ_ADDRESS, 0x00); // Read the temperature sensor
	return temperature;
}