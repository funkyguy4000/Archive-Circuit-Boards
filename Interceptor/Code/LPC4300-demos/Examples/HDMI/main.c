/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "LCD_HDMI" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc43xx_cgu.h"
#include "lpc4350_db1_emc.h"
#include <string.h>

#define IMAGE_FLASH_ADDR    0x1C020000

extern void TFP410_Init(void *framebuffer);


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
    SystemInit();
    CGU_Init();

    // Configure external flash
    MemoryPinInit();
    EMCSRDRAMInit();
    EMCFlashInit();

    // Init SysTick to 1ms
    SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 1000);

    // Copy image to SDRAM
    memcpy((void *)SDRAM_BASE_ADDR, (void *)IMAGE_FLASH_ADDR, 2UL*1024*768);

    // Initialize TFP410
    TFP410_Init((void *)SDRAM_BASE_ADDR);

    while (1)
    {
    }
}
