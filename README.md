#Circuit-Boards


A collection of my circuit boards and devices

##Strutz Library.lbr 
###This is a collection the parts I've made.  Some of the packages are copied from other libraries.
I'll list the important devices here:

Device | Package 1 | Package 2 | Package 3 | Description 
| --- | --- | --- | --- | --- |
**25AA1024**  |  DFN8-5x6 | 8-PDIP | SO8 | 1MBit SPI EEPROM from Microchip
**74AVC8T245** | QFN-60 | TSSOP-48 | | 8-biit logic level converter from NXP
**AD1939**  |  QFP64  | | | 4 ADC/ 8 DAC, 192 kHz, 24-Bit codec from Analog Devices
**ADP5589**  | 24-LFCSP (QFN) | 25-WLCSP (BGA) | | Keypad decoder / 19-bit IO Expander from Analog Devices
**ADSP-21479** | QFN-88 | | | SHARC DSP processor from Analog Devices
**ADSP-21483/6/7/9** | LQFP-176 | | | SHARC DSP processsor from analog devices
**AS1130** | TSSOP-28 | WL_CSP-20 (BGA) | | 132 LED driver from austriamicrosystems 
**AS3935** | QFN-16 | | | Lightning Sensor from austriamicrosystems
**AT25128B/256B** | 8VBGA | | | 128k/256k SPI EEPROM from Atmel
**ATMEGA8U2** | QFN-32 | | | 8-bit AVR microcontroller with embedded USB and 8kB flash from Atmel
**ATMEGA16** | 40-PDIP | | | 8-bit AVR microcontroller with 16kB flash from Atmel
**ATTINY4/5/6/9/10** | SOT-23-6 | | | 8-bit AVR micrcontroller for tiny applications from Atmel
**BGF120A** | TSLP-4-8 | | | Dual channel, ultra-low capacitance ESD diode from Infineon
**CS4234** | QFN-40 | | | 4 ADC/ 5 DAC Codec with programmable class H driver from Cirrus Logic
**CS4344** | TSSOP-10 | | | 24-bit, 192kHz Stereo DAC from Cirrus Logic
**CS5343** | TSSOP-10 | | | 24-bit Stereo ADC from Cirrus Logic
**CS42436** | QFP-52 | | | 6 ADC/ 6 DAC, 192kHz TDM audio codec from Cirrus Logic
**DS1077** | TSSOP-8 | | | Dual-programmable oscillator from Maxim
**FRDM-KL26Z**  | | | |  ***NOT TO BE USED FOR LAYOUT*** Only schematic purposes
**FT221X** | 20-QFN | 20-SSOP | | USB to FT1248 bridge from FTDI
**FT234XD** | DFN12-3x3 | | | USB to basic UART bridge from FTDI
**HIH-6130** | SO-8 | | | Digital humidity / temperature sensor from Honeywell
**HMC5983** | QFN-16 | | | 3-Axis digital compass from Honeywell
**L3GD20TR** | LGA-16 | | | 3-Axis digital gyroscope from ST Microelectronics
**LIS3MDL** | LGA-12 | | | 3-Axis digital magnetic sensor from ST Microelectronics
**LSM303DLHC** | LGA-14 | | | 3D accelerometer and 3D magnetometer from ST Microelectronics
**LT8500** | QFN-56 | | | 48-Channel LED PWM Generator with 12-bit resolution and 50MHz cascadeable serial interface from Linear Technologies
**LTC3539** | DFN8-3x3 | | | 2A, 1MHz/2MHz Synchronous Boost DC/DC Converter from Linear Technologies
**LTC3605** | QFN24 | | | 15V, 5A Synchronouse Buck Regulator from Linear Technologies
**LTC3811** | QFN38 | | | High Speed Dual, Multi-phase Buck DC/DC Controller from Linear Technologies
**M24M01** | BGA-8 | | | 1MBit I2C EEPROM from ST Microelectronics
**MCP2210** | QFN-20 | SSOP-20 | | USB to SPI bridge with GPIO from Microchip
**MMA8453Q** | QFN-16 | | | 3-Axis 10-bit/8-bit Digital Accelerometer from Freescale Semiconductor
**MMBT3904** | SOT-23 | | | 40V NPN small signal transistor
**MT48LC4M16A2** | TSOP-54 | | | 64MB x4 SDRAM from Micron
**OP275** | SO-8 | | | Bipolar/JFET Dual audio op-amp from Analog Devices
**PMOD** | 2x6 .1" Header | | | Custom part for PMOD breakout **UNTESTED**
**PT15** | 1206 | | | Phototransistor from Everlight
**RCA_Double** | Custom | | | Unkown
**SJ1-3514-SMT** | Custom | | |  Squarish Audio connector with tip switch from CUI Inc.  
**SJ3524** | Custom | | | Rectangular Audio connector with tip switch from CUI Inc
**TLC5940** | DIL28-3 | QFN-32 | TSSOP28-GROUNDPAD | 16-Channel LED driver with PWM control from Texas Instruments
**TLC5947** | QFN-32  | | | 24-Channel LED driver with PWM control from Texas Instruments
**TLC5958** | QFN-56 | | | 48-Channel LED driver with PWM control and 48kBit SRAM from Texas Instruments
**TPS62130A** | QFN16-3x3 | | | 3-17V 3A Buck DC/DC converter from Texas Instruments
**TPS84320** | RUQ_47(QFN) | | | 4.5-14.5V 3A Buck DC/DC converter from Texas Instruments
**TS3USB221** | 10-QFN | 10-SON  | | USB-2.0HS 1:2 Mux/Demux switch from Texas Instruments
**USB5534B** | QFN-64 | | | 4-Port SS/HS USB hub controller from Microchip
**USBA-DOUBLE** | Custom | | | Unknown
**WS2812-5050** | Custom | | | RGB LED driver with integrated WS2812 LED driver from Unknown
**XS1-L8A-128** | QFN-124 | | | 8-Core 32-Bit Microcontroller with 128kB SRAM from XMOS 
**XS1-U8A-128** | BGA-217 | | | 8-Core 32-Bit Microcontroller with USB Phy from XMOS
**XTAG-2** | MOLEX-52745-2097 | | | Proprietary debug adapter for XMOS devices

##On to the boards!

* **ADSP-21477...** - Breaks out the 88-QFN package of this DSP from Analog Devices
* **CS4344**  - Breaks out this Analog to Digital Converter from Cirrus
* **CS5343**  - Breaks out this Digital to Analog Converter from Cirrus
* **TLC5947 Old** - A board from when I used to layout without using a schematic.  Uses two TLC5947 ICs to manage 16 5050 RGB LEDS
* **Lego type WS2812** - A modular LED board made to connect to other pieces to make any LED display
* **Neopixel** - My board made after the Neopixel from Adafruit
* **Version 1 WS2812** - A square board using the WS2812 LEDs to make an LED matrix
* **Smod Boards** - These boards were made for half fun and half the University of Wisconsin - Stout IEEE
* **Stout IEEE Dev Board** - This is a development board in the form of an Arduino based off of the ATmega16 for UW-Stout IEEE.  Includes onboard programming based off of an ATtiny45.


More projects and such will be added soon.  I pretty much make a new one everyday.


<a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/us/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/3.0/us/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/us/">Creative Commons Attribution-ShareAlike 3.0 United States License</a>.




