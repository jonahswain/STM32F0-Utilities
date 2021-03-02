/*
STM32F0 Utilities
A Collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board

Author: Jonah Swain (SWNJON003)
Date created: 17/04/2018
Date modified: 24/05/2018

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
	init_EEPROM(); // Initialise the SPI EEPROM
	init_DAC(1, 0, 0, 0);

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
	pinMode(PA4, GPIO_ANALOG); // Set PA4 (DAC out) to analog

	configure_PWM(TIM2); // Enable PWM on TIM2
	pinMode(RGLED_R, GPIO_ALTFN); // Set the pin mode of the red part of the RG LED to alternate function
	pinMode(RGLED_G, GPIO_ALTFN); // Set the pin mode of the green part of the RG LED to alternate function
	afSelect(RGLED_R, GPIO_AF2); // Map the red part of the RG LED to TIM2 CH3
	afSelect(RGLED_G, GPIO_AF2); // Map the green part of the RG LED to TIM2 CH4
	pwmEnable(TIM2, 3); // Enable PWM output on TIM2 CH3
	pwmEnable(TIM2, 4); // Enable PWM output on TIM2 CH4

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

void init_EEPROM() {
	// Initialise SPI for the EEPROM
	pinMode(EEPROM_MOSI, GPIO_ALTFN); // Set MOSI as alternate function
	pinMode(EEPROM_MISO, GPIO_ALTFN); // Set MISO as alternate function
	pinMode(EEPROM_SCK, GPIO_ALTFN); // Set SCK as alternate functionm
	pinMode(EEPROM_CS, GPIO_OUTPUT); // Set chip select as output
	afSelect(EEPROM_MOSI, GPIO_AF0); // Map MOSI to AF0 (SPI)
	afSelect(EEPROM_MISO, GPIO_AF0); // Map MISO to AF0 (SPI)
	afSelect(EEPROM_SCK, GPIO_AF0); // Map SCK to AF0 (SPI)
	digitalWrite(EEPROM_CS, HIGH); // Set CS HIGH

	init_SPI(SPI2, SPI_BAUD_3MHZ, SPI_MASTER_MODE, SPI_MSB_FIRST, SPI_FRAMESIZE_8BIT, SPI_BIDIOE, SPI_MULTIMASTER_DISABLE, 0, 0, 0, SPI_RXNE_8BIT); // Initialise and configure SPI in 8-bit mode 0,0

}

void ledWrite(uint8_t pattern) {
	// Displays a pattern on the red LEDs on the board
	GPIOB->ODR &= (0xFF00 | (0xFF & pattern)); // Turn off the LEDs that need to be turned off
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

void eepromWrite(uint16_t address, uint8_t data) {
	// Writes a byte of data to an address in the EEPROM
	uint8_t dummyData; // A variable to store any rubbish that is received
	__spiFlushRXBuffer(SPI2); // Flush RX buffer before starting

	// Set the write enable latch
	digitalWrite(EEPROM_CS, LOW); // Set chip select low
	__cpuHoldDelay(1);
	spiTransmitFrame(SPI2, EEPROM_WREN); // Send the EEPROM write enable instruction
	while (!(SPI2->SR & SPI_SR_RXNE)); // Wait for the EEPROM to send rubbish, acknowledging the request
	dummyData = *((uint8_t*)(&SPI2->DR)); // Pull the rubbish from the RX buffer
	digitalWrite(EEPROM_CS, HIGH); // Set chip select high
	__cpuHoldDelay(5000); // Wait for instruction to complete

	// Send the write instruction
	digitalWrite(EEPROM_CS, LOW); // Set chip select low
	__cpuHoldDelay(1);
	spiTransmitFrame(SPI2, EEPROM_WRITE); // Send the EEPROM write instruction
	while (!(SPI2->SR & SPI_SR_RXNE)); // Wait for the EEPROM to send rubbish, acknowledging the request
	dummyData = *((uint8_t*)(&SPI2->DR)); // Pull the rubbish from the RX buffer

	// Send the address
	spiTransmitFrame(SPI2, (address >> 8)); // Send the address MSBs
	while (!(SPI2->SR & SPI_SR_RXNE)); // Wait for the EEPROM to send rubbish, acknowledging
	dummyData = *((uint8_t*)(&SPI2->DR)); // Pull the rubbish from the RX buffer
	spiTransmitFrame(SPI2, (address & 0x00FF)); // Send the address LSBs
	while (!(SPI2->SR & SPI_SR_RXNE)); // Wait for the EEPROM to send rubbish, acknowledging
	dummyData = *((uint8_t*)(&SPI2->DR)); // Pull the rubbish from the RX buffer

	// Send the data
	spiTransmitFrame(SPI2, data); // Send the data
	while (!(SPI2->SR & SPI_SR_RXNE)); // Wait for the EEPROM to send rubbish, acknowledging
	dummyData = *((uint8_t*)(&SPI2->DR)); // Pull the rubbish from the RX buffer

	digitalWrite(EEPROM_CS, HIGH); // Set chip select high
	__cpuHoldDelay(5000); // Wait for instruction to complete
}

uint8_t eepromRead(uint16_t address) {
	// Reads a byte of data from an address in the EEPROM
	uint8_t dummyData; // A variable to store any rubbish that is received
	__spiFlushRXBuffer(SPI2); // Flush RX buffer before starting

	// Send the read instruction
	digitalWrite(EEPROM_CS, LOW); // Set chip select low
	__cpuHoldDelay(1);
	spiTransmitFrame(SPI2, EEPROM_READ); // Send the EEPROM write instruction
	while (!(SPI2->SR & SPI_SR_RXNE)); // Wait for the EEPROM to send rubbish, acknowledging the request
	dummyData = *((uint8_t*)(&SPI2->DR)); // Pull the rubbish from the RX buffer

	// Send the address
	spiTransmitFrame(SPI2, (address >> 8)); // Send the address MSBs
	while (!(SPI2->SR & SPI_SR_RXNE)); // Wait for the EEPROM to send rubbish, acknowledging
	dummyData = *((uint8_t*)(&SPI2->DR)); // Pull the rubbish from the RX buffer
	spiTransmitFrame(SPI2, (address & 0x00FF)); // Send the address LSBs
	while (!(SPI2->SR & SPI_SR_RXNE)); // Wait for the EEPROM to send rubbish, acknowledging
	dummyData = *((uint8_t*)(&SPI2->DR)); // Pull the rubbish from the RX buffer

	// Get the data
	uint8_t data = spiReceiveFrame(SPI2); // Get the data

	digitalWrite(EEPROM_CS, HIGH); // Set chip select high
	__cpuHoldDelay(5000); // Wait for instruction to complete
	return data;
}