/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "I2C EEPROM" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "at24xx.h"


#define SLA     0x50


static uint32_t baseaddr = 0;
static uint8_t writeBuffer[8];
static uint8_t readBuffer[8];


volatile uint32_t cycleCounter;


void DelayMs(uint32_t delay)
{
    uint32_t startTime = cycleCounter;
    while ((uint32_t)(cycleCounter - startTime) <= delay);
}


void SysTick_Handler(void)
{
    cycleCounter++;
}


int main(void)
{
    int i;

    SystemInit();
    CGU_Init();
    // Init SysTick to 1ms
    SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 1000);

    AT24_Init();

    // Prepare buffer
    for (i = 0; i < sizeof(writeBuffer); i++)
    {
        writeBuffer[i] = i;
    }

    // Write page
    AT24_Write(SLA, baseaddr, 1, writeBuffer, sizeof(writeBuffer));
    DelayMs(10); // wait

    while (1)
    {
        // Clear read buffer
        for (i = 0; i < sizeof(writeBuffer); i++)
        {
            readBuffer[i] = ~writeBuffer[i];
        }

        // Read data back
        AT24_Read(SLA, baseaddr, 1, readBuffer, sizeof(readBuffer));

        // Verify
        for (i = 0; i < sizeof(writeBuffer); i++)
        {
            if ( writeBuffer[i] != readBuffer[i])
            {
                while (1);  // catch th error
            }
        }
    }
}
