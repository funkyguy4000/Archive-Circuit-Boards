/*
 * Audio Slice with startKIT.xc
 *
 *  Created on: Dec 25, 2013
 *      Author: ShannonS
 */

#include <xs1.h>                        //Proprietary library for XMOS devices
#include <BRE.h>                        //Product Codename, contains custom functions
#include <startKIT_port_mapping.h>      //Pin mappings for the startKIT to the PCI connector
#include <CS4270.h>                     //Library for the CS4270
#include <fft.h>                        //Library for the Fast-Fourier Transform
#include <i2c.h>                        //I2C library
#include <i2s.h>                        //I2S Library
#include <periodogram.h>                //Library for the periodogram spectral density estimation

//XS1 Clock mapping
clock MCLK_CLK      =   XS1_CLKBLK_1;
clock BCLK_CLK     =   XS1_CLKBLK_2;

in buffered port:32 ADC_DATA0 = XS1_PORT_1K;
in buffered port:32 ADC_DATA1 = XS1_PORT_1L;

out buffered port:32 DAC_DATA0 = XS1_PORT_1G;
out buffered port:32 DAC_DATA1 = XS1_PORT_1H;

int main(void){

    //Set up I2C
    //The I2C bus is only for configuring the Codec, may be used for port expanders to all for a larger direct drive.
    i2c_master_init(SCL_SDA);
    CS4270_Init(SCL_SDA);

    //Set up I2S
    in buffered port:32 data_in[2] = {ADC_DATA1, ADC_DATA0};
    out buffered port:32 data_out[2] = {DAC_DATA1, DAC_DATA0};
    streaming chanend channel_data;
    r_i2s i2s_resources = {.cb1 = MCLK_CLK, .cb2 = BCLK_CLK, .mck = MCLK, .bck = BCLK, .wck = LRCLK, .din = data_in,  .dout = data_out};


    while(1){
        i2s_master(i2s_resources, channel_data, 4 );
    };
    return 0;
};







//GRAVEYARD

/* Return acknowledge
 * For I2C Bus
char AckWait = 0;
while(AckWait == 1){
    for(char i = 0; i == 10; i++){
        char r = recieveAck(SCL_SDA);
        if (r == 1){
            if(i == 10){
                //abort Ack search, resend full register data
            }
            else{}
        }
        else{
            AckWait = 0;
            break;
        }
    }
}

//Maps the 8-bit register to 8 different LEDs on the 3x3 matrix
 * doesn't work!
void map_register(char reg[]){
    int leds = 0b00000000000000000000000000000000;
    if (reg[7] == 1){
        leds = leds + A1;
    }
    if (reg[6] == 1){
        leds = leds + B1;
    }
    if (reg[5] == 1){
        leds = leds + C1;
    }
    if (reg[4] == 1){
        leds = leds + A2;
    }
    if (reg[3] == 1){
        leds = leds + B2;
    }
    if (reg[2] == 1){
        leds = leds + C2;
    }
    if (reg[1] == 1){
        leds = leds + A3;
    }
    if (reg[0] == 1){
        leds = leds + B3;
    }
    else{
        leds = LED3x3_off;
    }
    p32 <: leds;

}

*/







