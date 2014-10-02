/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "LCD with External SDRAM" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc4350_db1_emc.h"


// =0 LCD extension board
// =1 HDMI extension board with external monitor
#define USE_HDMI        0


#if USE_HDMI == 0
    #define LCD_WIDTH       320
    #define LCD_HEIGHT      240
#else
    #define LCD_WIDTH       1024
    #define LCD_HEIGHT      768
#endif


volatile uint32_t cycleCounter;
static uint32_t *framebuffer = (uint32_t *)SDRAM_BASE_ADDR;


void LTV350QV_Init(void *frameBuffer);
void TFP410_Init(void *frameBuffer);


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
    int x, y;

    SystemInit();
    CGU_Init();
    // Init SysTick to 1ms
    SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 1000);

    // Configure external flash
    MemoryPinInit();
    EMCSRDRAMInit();
    EMCFlashInit();


    // Create color bars
    for (x = 0; x < LCD_WIDTH; x++)
    {
        uint8_t n = x * 8 / LCD_WIDTH; // 8 colorbars
        uint32_t color = 0;

        if (n & 0x01) color |= 0x0000FF; // +blue
        if (n & 0x02) color |= 0x00FF00; // +green
        if (n & 0x04) color |= 0xFF0000; // +red

        for (y = 0; y < LCD_HEIGHT; y++)
        {
            framebuffer[y * LCD_WIDTH + x] = color;
        }
    }

#if USE_HDMI == 0
    LTV350QV_Init(framebuffer);
#else
    TFP410_Init(framebuffer);
#endif

    while (1)
    {
        uint32_t color0 = framebuffer[0];

        // Slide color bars
        for (x = 0; x < LCD_WIDTH; x++)
        {
            uint32_t color = (x < (LCD_WIDTH)) ? framebuffer[x + 1] : color0;

            for (y = 0; y < LCD_HEIGHT; y++)
            {
                framebuffer[y * LCD_WIDTH + x] = color;
            }
        }

        DelayMs(5 * 1000 / LCD_WIDTH);  // 5 sec cycle
    }
}
