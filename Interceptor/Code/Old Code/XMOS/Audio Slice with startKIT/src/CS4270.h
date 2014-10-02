/*
 * cs4270.h
 *
 *  Created on: Dec 27, 2013
 *      Author: ShannonS
 */

#ifndef CS4270_H_
#define CS4270_H_

#include <i2c.h>

//CS4270 I2C control register addresses
#define Device_ID       0x01        // ID3 - ID2 - ID1 - ID0 - REV3 - REV2 - REV1 - REV0
#define Power_Control   0x02        // Freeze - Res - PDN_ADC - Res - Res - Res - PDN_DAC - PDN
#define Mode_Control    0x03        // Res - Res - FM1- FM0 - MCLK_FREQ2 - MCLK_FREQ1 - MCLK_FREQ0 - POPG
#define ADC_DAC_Cont    0x04        // ADC_HPF_FRZ_A - ADC_HPF_FRZ_B - DIG_LOOPBK - DAC_DIF1 - DAC_DIF0 - Res - Res - ADC_DIF0
#define Trans_Cont      0x05        // DAC_SNGL_VOL - DAC_SOFT - DAC_ZC - ADC_INV_CHB - ADC_INV_CHA - DAC_INV_CHB - DAC_INV_CHA - DE_EMPH
#define Mute_Control    0x06        // Res - Res - AUTO_MUTE - MUTE_ADC_CHB - MUTE_ADC_CHA - MUTE_POL - MUTE_DAC_CHB - MUTE_DAC_CHA
#define DAC_A_Volume    0x07        // DACA_VOL7 - DACA_VOL6 - DACA_VOL5 - DACA_VOL4 - DACA_VOL3 - DACA_VOL2 - DACA_VOL1 - DACA_VOL0
#define DAC_B_Volume    0x08        // DACB_VOL7 - DACB_VOL6 - DACB_VOL5 - DACB_VOL4 - DACB_VOL3 - DACB_VOL2 - DACB_VOL1 - DACB_VOL0
#define Chip_Address    0x1001100   // This is just the address for the first CS4270 in the Audio Slice.


void CS4270_Init(port i2c_port){
    //Device ID: readonly
    //get device ID, dynamic jazz

    //Local Vars
    char read_register[8];
    char write_register[8];

    //Power Control: 0 - Res - 0 - Res - Res - Res - 0 - 0              This sets everything on
    i2c_master_read_reg(Chip_Address, Power_Control, read_register, 1, i2c_port);
    for (int i = 7; i==0; i--){
        write_register[i] = read_register[i];
    }
    write_register[7] = 0;
    write_register[5] = 0;
    write_register[1] = 0;
    write_register[0] = 0;
    i2c_master_write_reg(Chip_Address, Power_Control, write_register, 1, i2c_port);

    //Mode Control: Res - Res - 1 - 1 - 1 - 0 - 0 - 1                   Sets it in Slave mode, Divide by 4, Popguard Transient enabled
    i2c_master_read_reg(Chip_Address, Mode_Control, read_register, 1, i2c_port);
    for (int i = 7; i==0; i--){
            write_register[i] = read_register[i];
    }
    write_register[5] = 1;
    write_register[4] = 1;
    write_register[3] = 1;
    write_register[2] = 0;
    write_register[1] = 0;
    write_register[0] = 1;
    i2c_master_write_reg(Chip_Address, Mode_Control, write_register, 1, i2c_port);

    //ADC and DAC control: 0 - 0 - 0 - 0 - 0 - Res - Res - 0            ADC HPF A/B off, Digital Loopback disabled,  DAC Left Justified up to 24-bits data, ADC Left Justified up to 24-bits of data
    i2c_master_read_reg(Chip_Address, ADC_DAC_Cont, read_register, 1, i2c_port);
    for (int i = 7; i==0; i--){
            write_register[i] = read_register[i];
    }
    write_register[7] = 0;
    write_register[6] = 0;
    write_register[5] = 0;
    write_register[4] = 0;
    write_register[3] = 0;
    write_register[0] = 0;
    i2c_master_write_reg(Chip_Address, ADC_DAC_Cont, write_register, 1, i2c_port);

    //Transition Control: 1 - 1 - 0 - 0 - 0 - 0 - 0 - 0                 DAC Single Volume enabled, Soft Ramp Enabled
    unsigned char trans[8] = {1,1,0,0,0,0,0,0};
    i2c_master_write_reg(Chip_Address, Trans_Cont, trans, 1, i2c_port);

    //Mute Control: Res - Res - 0 - 0 - 0 - 0 - 0 - 0                   Auto mute off, active low mute polarity, nothing muted
    i2c_master_read_reg(Chip_Address, Mute_Control, read_register, 1, i2c_port);
    for (int i = 7; i==0; i--){
            write_register[i] = read_register[i];
    }
    write_register[5] = 0;
    write_register[4] = 0;
    write_register[3] = 0;
    write_register[2] = 0;
    write_register[1] = 0;
    write_register[0] = 0;
    i2c_master_write_reg(Chip_Address, Mute_Control, write_register, 1 ,i2c_port);

    //DAC Channel A Volume Control : 0 - 0 - 1 - 0 - 1 - 0 - 0 - 0      20dB attenuation
    unsigned char DAC_A_Vol[8] = {0,0,1,0,1,0,0,0};
    i2c_master_write_reg(Chip_Address, Mute_Control, DAC_A_Vol, 1, i2c_port);

    //DAC Channel B Volume Control : 0 - 0 - 1 - 0 - 1 - 0 - 0 - 0      20dB attenuation (not needed to be set, Channel B is controlled by Channel A b/c of Transition control MSB
    unsigned char DAC_B_Vol[8] = {0,0,1,0,1,0,0,0};
    i2c_master_write_reg(Chip_Address, Mute_Control, DAC_B_Vol, 1, i2c_port);
}

//different operating modes
//change register methonds



#endif /* CS4270_H_ */
