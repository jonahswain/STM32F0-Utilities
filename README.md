# STM32F0-Utilities
A collection of utilities for STM32F0 microcontrollers, primarily targeted at the STM32F051C6-based UCT development board.

I made this set of libraries while enrolled in the second-year embedded systems course as a homebrew alternative to the STM32F0 HAL libraries. It provides more Arduino-like abstractions, and specific functions and example code for the STM32F051C6-based UCT development board. It is no longer maintained because I no longer use the STM32F051C6 UCT dev board for my projects, and I use the HAL libraries where appropriate.

## Basic features/functionality
- [x] ADC (configuration and single shot/read)
- [x] DAC (configuration, single shot, continuous output, and DMA support)
- [x] GPIO (configuration and read/write)
- [x] DMA (configuration and block memory copy)
- [x] I2C (configuration and read/write)
- [x] SPI (configuration and read/write)
- [x] Timers (initialisation, timing, and PWM)
- [x] Interrupts (NVIC configuration and auto configuration for GPIO interrupts)
- [x] USART (configuration and read/write) **UNTESTED** (see USART branch)
- [ ] USART (DMA)

## Additional features/functionality
- [x] LCD support for ADM1602K-NSA-FBS 2-line displays in 4-bit mode
- [x] Read/write for CAT25040 SPI EEPROM
- [x] Read from TC74A0-3.3VAT I2C temperature sensor
- [x] Self-test code for testing all onboard peripherals for the STM32F051C6 UCT development board

## Project organisation
```
STM32F0-Utilities
│
├── src                         (library source files (.c))
│
├── include                     (library include/header files (.h), includes dependency ST's stm32f0xx.h)
│
├── example code                (example code using library functions for the STM32F051C6 UCT dev board)
│
├── waveforms                   (waveform samples for DAC output)
│   ├── WAVE_SINE256.h          (256-sample sinusiod)
│   ├── WAVE_TABLE_MOUNTAIN.h   (778 sample reproduction of Table Mountain)
```

## How to use
Import this library's src/ and include/ directories into your project, and `#include` the relevant library components where required.

If using the interrupt functionality, you must implement a `void pinInterruptTriggered(IOPin_TypeDef* iopin)` function in your code to handle GPIO pin interrupts.