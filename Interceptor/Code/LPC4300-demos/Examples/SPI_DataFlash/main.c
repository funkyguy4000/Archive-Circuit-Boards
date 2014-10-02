/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "SPI DataFlash" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/


#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "at45xx.h"


static uint32_t baseaddr = 0;
static uint8_t writeBuffer[64];
static uint8_t readBuffer[64];


int main(void)
{
    int i;

    SystemInit();
    CGU_Init();

    AT45_Init();

    // Prepare buffer
    for (i = 0; i < sizeof(writeBuffer); i++)
    {
        writeBuffer[i] = i;
    }

    // Write page
    AT45_ErasePage(baseaddr);
    AT45_WritePage(baseaddr, writeBuffer, sizeof(writeBuffer));

    while (1)
    {
        // Clear read buffer
        for (i = 0; i < sizeof(writeBuffer); i++)
        {
            readBuffer[i] = ~writeBuffer[i];
        }

        // Read data back
        AT45_ReadPage(baseaddr, readBuffer, sizeof(readBuffer));

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
