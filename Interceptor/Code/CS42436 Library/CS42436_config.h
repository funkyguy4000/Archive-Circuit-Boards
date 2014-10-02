/*
 * CS42436_config.h
 *
 * Created: 3/20/2014 11:21:28 PM
 *  Author: student
 */ 


#ifndef CS42436_CONFIG_H_
#define CS42436_CONFIG_H_

//1 for Software Mode, 0 for Hardware Mode
int Transfer_Mode = 1;

//both address pins to GND
#define CS42436_ADDRESS_WRITE	0x90h
#define CS42436_ADDRESS_READ	0x91h

#define RST_PIN				//Define Reset Pin Here
#define FS_PIN				//SS on SPI
#define SCLK_PIN			//SCK on SPI
#define DAC_DATA_PIN		//MOSI on SPI
#define ADC_DATA_PIN		//MISO on SPI

#endif /* CS42436_CONFIG_H_ */