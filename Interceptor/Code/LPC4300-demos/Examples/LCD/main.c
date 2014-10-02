/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "LCD" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc4350_db1_emc.h"


// Flash can be erased by sectors, so images must be placed in different sectors
// and differ from application sectors
// 320*240*4byte(24bpp unpacked) = 307200 bytes = 5 sectors per image
#define IMAGE0_FLASH_ADDR       0x1C020000  // sector 2
#define IMAGE1_FLASH_ADDR       0x1C070000  // sector 7
#define IMAGE2_FLASH_ADDR       0x1C0C0000  // sector 12

// We accept touches between this coordinates
#define SHOW_INTRO_X1       254
#define SHOW_INTRO_Y1       0
#define SHOW_INTRO_X2       319
#define SHOW_INTRO_Y2       100
#define SHOW_DIOLAN_X1      172
#define SHOW_DIOLAN_Y1      150
#define SHOW_DIOLAN_X2      319
#define SHOW_DIOLAN_Y2      229
#define SHOW_ARROW_X1       0
#define SHOW_ARROW_Y1       150
#define SHOW_ARROW_X2       143
#define SHOW_ARROW_Y2       229

// Debounce is not really needed for touch-controller, it is mainly used
// as a keypress delay and single press detection.
#define DEBOUNCE            20


// Pointers to images in flash
static void * const intro  = (void *)IMAGE0_FLASH_ADDR;
static void * const diolanInfo = (void *)IMAGE1_FLASH_ADDR;
static void * const arrowInfo  = (void *)IMAGE2_FLASH_ADDR;


// Extern functions
void LTV350QV_Init(void *framebuffer);
void LTV350QV_SetFrameBuffer(void *frameBuffer);
void TSC2007_Init(void);
int TSC2007_CheckPressure(uint16_t *x, uint16_t *y);


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
    // Init SysTick to 1ms
    SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 1000);

    // Configure external flash
    MemoryPinInit();
    EMCFlashInit();

    // Init LCD
    LTV350QV_Init(intro);
    // Enable backligth
    scu_pinmux(0x9, 5, MD_PLN, FUNC4);  // P9_5 = GPIO5[18]
    GPIO_SetDir(5, (1 << 18), 1);
    GPIO_SetValue(5, (1 << 18));

    // Init touch controller
    TSC2007_Init();

    while (1)
    {
        static uint8_t state;
        static uint32_t debounce;
        uint16_t adcx, adcy;
        int pressure = TSC2007_CheckPressure(&adcx, &adcy);

        if (pressure > 0)
        {
            if (debounce < DEBOUNCE)
            {
                debounce++;
                if (debounce >= DEBOUNCE)
                {
                    uint32_t x = adcx * 319 / 4095;
                    uint32_t y = adcy * 239 / 4095;

                    switch (state)
                    {
                    // startup screen
                    case 0:
                        if (x > SHOW_DIOLAN_X1 && x < SHOW_DIOLAN_X2 && y > SHOW_DIOLAN_Y1 && y < SHOW_DIOLAN_Y2)
                        {
                            state = 1;
                            LTV350QV_SetFrameBuffer((void *)diolanInfo);
                        }
                        else if (x > SHOW_ARROW_X1 && x < SHOW_ARROW_X2 && y > SHOW_ARROW_Y1 && y < SHOW_ARROW_Y2)
                        {
                            state = 2;
                            LTV350QV_SetFrameBuffer((void *)arrowInfo);
                        }
                        break;
                    // info 1 screen
                    // info 2 screen
                    case 1:
                    case 2:
                        if (x > SHOW_INTRO_X1 && x < SHOW_INTRO_X2 && y > SHOW_INTRO_Y1 && y < SHOW_INTRO_Y2)
                        {
                            state = 0;
                            LTV350QV_SetFrameBuffer((void *)intro);
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            debounce = 0;
        }
    }
}
