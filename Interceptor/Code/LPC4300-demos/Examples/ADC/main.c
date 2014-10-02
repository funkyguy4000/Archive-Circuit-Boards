/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "ADC" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/


#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_adc.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"


volatile uint16_t adcResults[5] = { ~0, ~0, ~0, ~0, ~0 };


// ADC0 interrupt. Used only to read channel 4 data
void ADC0_IRQHandler(void)
{
    // Check if conversion finishes
    if (ADC_ChannelGetStatus(LPC_ADC0, ADC_CHANNEL_4, ADC_DATA_DONE) == SET)
    {
        // Read ADC result
        adcResults[4] = ADC_ChannelGetData(LPC_ADC0, ADC_CHANNEL_4);
        // Disable interrupt
        ADC_IntConfig(LPC_ADC0, ADC_ADINTEN4, DISABLE);
    }
}


int main(void)
{
    SystemInit();
    CGU_Init();


    // Init ADC0: 200 kHz sampling rate, 10 bits
    ADC_Init(LPC_ADC0, 200000, 10);
    ADC_IntConfig(LPC_ADC0, ADC_ADGINTEN, DISABLE); // global interrupt is enabled by default
    NVIC_EnableIRQ(ADC0_IRQn);                      // enable interupt in NVIC


    //===========================================
    // Get ADC result on channel 3 using polling
    //===========================================

    // Enable channel 3
    ADC_ChannelCmd(LPC_ADC0, ADC_CHANNEL_3, ENABLE);

    // Start single conversion
    ADC_StartCmd(LPC_ADC0, ADC_START_NOW);
    // Wait for DONE bit set
    while (ADC_ChannelGetStatus(LPC_ADC0, ADC_CHANNEL_3, ADC_DATA_DONE) != SET);

    // Read result
    adcResults[3] = ADC_ChannelGetData(LPC_ADC0, ADC_CHANNEL_3);

    // Disable channel
    ADC_ChannelCmd(LPC_ADC0, ADC_CHANNEL_3, DISABLE);


    //=============================================
    // Get ADC result on channel 4 using interrupt
    //=============================================

    // Enable channel 4
    ADC_ChannelCmd(LPC_ADC0, ADC_CHANNEL_4, ENABLE);
    // Enable interrupt for channel 1
    ADC_IntConfig(LPC_ADC0, ADC_ADINTEN4, ENABLE);

    // Start single conversion
    ADC_StartCmd(LPC_ADC0, ADC_START_NOW);
    // Wait for result to be read in interrupt
    while (adcResults[4] == (uint16_t)~0);

    // Disable channel
    ADC_ChannelCmd(LPC_ADC0, ADC_CHANNEL_4, DISABLE);


    //================================================
    // Get ADC result on channels 0 and 1 using burst
    //================================================

    // Enable channel 0
    ADC_ChannelCmd(LPC_ADC0, ADC_CHANNEL_0, ENABLE);
    // Enable channel 1
    ADC_ChannelCmd(LPC_ADC0, ADC_CHANNEL_1, ENABLE);
    // Enable burst
    ADC_StartCmd(LPC_ADC0, ADC_START_CONTINUOUS);   // start must be = 0 when enabling burst
    ADC_BurstCmd(LPC_ADC0, ENABLE);

    while (1)
    {
        uint32_t dr;

        // Data read can be done using ADC_ChannelGetStatus() and ADC_ChannelGetData(),
        // but this functions reads data register 2 times

        // Read channel 0 result
        dr = LPC_ADC0->DR[0];
        if (dr & ADC_DR_DONE_FLAG)
        {
            adcResults[0] = ADC_DR_RESULT(dr);
        }

        // Read channel 1 result
        dr = LPC_ADC0->DR[1];
        if (dr & ADC_DR_DONE_FLAG)
        {
            adcResults[1] = ADC_DR_RESULT(dr);
        }
    }
}
