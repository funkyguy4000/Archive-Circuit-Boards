/*
 * IncFile1.h
 *
 * Created: 3/20/2014 8:26:30 PM
 *  Author: student
 */ 


#ifndef CS42436_H_
#define CS42436_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "TWI.h"
#include "CS42436_config.h"

#define ID			0x01h	//Identification		[Chip_ID3 , Chip_ID2, Chip_ID1, Chip_ID0 , Rev_ID3, Rev_ID2, Rev_ID1, Rev_ID0 ]
#define Pwr_Cont	0x02h	//Power Control			[PDN_ADC3 , PDN_ADC2 , PDN_ADC1 , Res , PDN_DAC3 , PDN_DAC2 , PDN_DAC1 , PDN]
#define Func_Mode	0x03h	//Functional Mode		[Res, Res, Res, Res, MFreq2, MFreq1, MFreq0, Res]
#define Misc_Cont	0x04h	//Misc Control			[Freeze, Aux_DIF, Res, Res, Res, Res, Res, Res]
#define ADC_Cont	0x05h	//ADC Control			[ADC1-2 HPF Freeze, ADC3_HPF Freeze, DAC_DEM, ADC1 Single, ADC2 Single, ADC3 Single, AIN5_MUX, AIN6_MUX]
#define Trans_Cont	0x06h	//Transition Cont.		[DAC_SNG VOL, DAC_SZC1, DAC_SZC0, AMUTE, MUTE ADC_SP, ADC_SNG VOL, ADC_SZC1, ADC_SZC0]
#define Chan_Mute	0x07h	//Channel Mute			[Res, Res, AOUT6 MUTE, AOUT5 MUTE, AOUT4 MUTE, AOUT3 MUTE, AOUT2 MUTE, AOUT1 MUTE]
#define Vol_Aout1	0x08h	//Volume Cont. AOUT1	[VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Aout2	0x09h	//Volume Control AOUT2	[VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Aout3	0x0Ah	//Volume Control AOUT3  [VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Aout4	0x0Bh	//Volume Control AOUT4  [VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Aout5	0x0Ch	//Volume Control AOUT5  [VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Aout6	0x0Dh	//Volume Control AOUT6  [VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define DAC_Invert	0x10h	//DAC Channel Invert	[Res, Res, INV_AOUT6, INV_AOUT5, INV_AOUT4, INV_AOUT3, INV_AOUT2, INV_AOUT1]
#define Vol_Ain1	0x11h	//Volume Control AIN1	[VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Ain2	0x12h	//Volume Control AIN2	[VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Ain3	0x13h	//Volume Control AIN3	[VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Ain4	0x14h	//Volume Control AIN4	[VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Ain5	0x15h	//Volume Control AIN5	[VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define Vol_Ain6	0x16h	//Volume Control AIN6	[VOL7, VOL6, VOL5, VOL4, VOL3, VOL2, VOL1, VOL0]
#define ADC_Invert	0x17h	//ADC Channel Invert	[Res, Res, INV_A6, INV_A5, INV_A4, INV_A3, INV_A2, INV_A1]
#define Status		0x19h	//Status				[Res, Res, Res, Res, CLK Error, ADC3 OVFL, ADC2 OVFL, ADC1 OVFL]
#define Status_Mask	0x1Ah	//Status Mask			[Res, Res, Res, Res, CLK Error_M, ADC3 OVFL_M, ADC2 OVFL_M, ADC1 OVFL_M]



//registerRead
//uint8_t address, the 8-bit address of the register you want to read from.
//returns 8-bit data
uint8_t registerRead(uint8_t address){
	TWIStart();
	TWIWrite(CS42436_ADDRESS_WRITE);
	TWIReadACK();
	TWIWrite(address);
	TWIReadACK();
	TWIStop();
	TWIStart();
	TWIWrite(CS42436_ADDRESS_READ);
	TWIReadACK();
	uint8_t data = TWIReadNACK();
	TWIStop();
	return data;
}


//registerWrite
//uint8_t address, the 8-bit address of the register you want to write to.
//uint8_t data, the byte of data to be written to the register at the specified address
void registerWrite(uint8_t address, uint8_t data){
	
	//the CS42436 is capable of auto-increment register write
	//using this will be much faster than using multiple registerWrite method calls
	// to do this, modify the following method in accordance to the comments inside

	TWIStart();
	TWIWrite(CS42436_ADDRESS_WRITE);
	TWIReadACK();
	TWIWrite(address);
	
	//TWIWrite(address | 0x80);
	
	TWIReadACK();
	TWIWrite(data);
	TWIReadACK();
	//TWIWrite(data2);
	//TWIReadACK();
	//TWIWrite(data3);
	//TWIReadACK();
	//and so on in the same fashion.  You'll need to add more parameters to the method
	TWIStop();
}


int CS42436_Init(int Transfer_Mode)
{
	TWIInit();
	//TODO:: Please write your application code
	if (Transfer_Mode == 0){		//Hardware Mode
		RST_PIN = 0;
		_delay_ms(5);
		RST_PIN = 1;
		_delay_ms(5);
		
		return 1;
	}
	else if(Transfer_Mode == 1){	//Software Mode
		RST_PIN = 0;
		_delay_ms(5);
		RST_PIN = 1;
		registerWrite(Pwr_Cont, 0x01);
		registerWrite(Func_Mode, 0xF8);
		registerWrite(Vol_Ain1, 0x50);
		registerWrite(Vol_Ain2, 0x50);
		registerWrite(Vol_Ain3, 0x50);
		registerWrite(Vol_Ain4, 0x50);
		registerWrite(Vol_Ain5, 0x50);
		registerWrite(Vol_Ain6, 0x50);
		registerWrite(Vol_Aout1, 0x50);
		registerWrite(Vol_Aout2, 0x50);
		registerWrite(Vol_Aout3, 0x50);
		registerWrite(Vol_Aout4, 0x50);
		registerWrite(Vol_Aout5, 0x50);
		registerWrite(Vol_Aout6, 0x50);
		registerWrite(Trans_Cont, 0x52);
		registerWrite(Chan_Mute, 0x3F);
		registerWrite(Pwr_Cont, 0x00);
		
		/*
		for (int i = 0; i >=2000; i++){
			FS_PIN = 1;
			_delay_us(5);
			FS_PIN = 0;
			_delay_us(5);
		}
		*/
		
		_delay_ms(3);
		registerWrite(Chan_Mute, 0x00);
		
		return 1;
	}
	else{
		return 0;
	}
	
	
}

//Volume change methods
//default is -40dB
int Change_Ouptut_Volume(uint8_t volume){
	registerWrite(Vol_Aout1, volume);
	registerWrite(Vol_Aout2, volume);
	registerWrite(Vol_Aout3, volume);
	registerWrite(Vol_Aout4, volume);
	registerWrite(Vol_Aout5, volume);
	registerWrite(Vol_Aout6, volume);
}
int Change_Input_Volume(uint8_t volume){
	registerWrite(Vol_Ain1, volume);
	registerWrite(Vol_Ain2, volume);
	registerWrite(Vol_Ain3, volume);
	registerWrite(Vol_Ain4, volume);
	registerWrite(Vol_Ain5, volume);
	registerWrite(Vol_Ain6, volume);
}

//converter = "DAC", "ADC", "Input", or "Output"
//channel = 1 through 6
//volume = an 8 bit number representing the volume, note page 
int Change_Channel_Volume(string converter, int channel, uint8_t volume){
	if (converter == "DAC" || converter == "Output"){
		switch (channel)
		{
		case 1:
			registerWrite(Vol_Aout1, volume);
			break;
		
		case 2:
			registerWrite(Vol_Aout2, volume);
			break;
			
		case 3:
			registerWrite(Vol_Aout3, volume);
			break;
			
		case 4:
			registerWrite(Vol_Aout4, volume);
			break;
			
		case 5:
			registerWrite(Vol_Aout5, volume);
			break;
			
		case 6:
			registerWrite(Vol_Aout6, volume);
			break;
		}
	}
	else if(converter == "ADC" || converter == "Input"){
		switch (channel)
		{
			case 1:
			registerWrite(Vol_Ain1, volume);
			break;
			
			case 2:
			registerWrite(Vol_Ain2, volume);
			break;
			
			case 3:
			registerWrite(Vol_Ain3, volume);
			break;
			
			case 4:
			registerWrite(Vol_Ain4, volume);
			break;
			
			case 5:
			registerWrite(Vol_Ain5, volume);
			break;
			
			case 6:
			registerWrite(Vol_Ain6, volume);
			break;
		}
	}
	
}


//returns contents of status register
uint8_t getStatus(){
	return registerRead(Status);
}

//sets Status Mask
//Returns 0 if no changes were made
//Returns 1 if changes were made
int setStatusMask(uint8_t mask){
	uint8_t previous = registerRead(Status_Mask);
	if (previous == mask){
		return 0;
	}
	else{
		registerWrite(Status_Mask, mask);
		return 1;
	}
}


#endif /* CS42436_H_ */