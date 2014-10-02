/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "SDRAM test" example.
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


// Comment this for a single test
// Uncomment for infinite test (e.g. to obtain fastest SDRAM timings or cold/hot stress test)
//#define CONTINUOUS_TEST


int main(void)
{
    SystemInit();
    CGU_Init();

    MemoryPinInit();
    EMCSRDRAMInit();
    EMCFlashInit();

    // Configure LEDs
    scu_pinmux(D2_SCU_PORT, D2_SCU_PIN, MD_BUK, FUNC0);
    scu_pinmux(D3_SCU_PORT, D3_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D4_SCU_PORT, D4_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D5_SCU_PORT, D5_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D6_SCU_PORT, D6_SCU_PIN, MD_BUK, FUNC4);
    GPIO_SetDir(D2_GPIO_PORT, D2_GPIO_MASK, 1);
    GPIO_ClearValue(D2_GPIO_PORT, D2_GPIO_MASK);
    GPIO_SetDir(D3_GPIO_PORT, D3_GPIO_MASK, 1);
    GPIO_ClearValue(D3_GPIO_PORT, D3_GPIO_MASK);
    GPIO_SetDir(D4_GPIO_PORT, D4_GPIO_MASK, 1);
    GPIO_ClearValue(D4_GPIO_PORT, D4_GPIO_MASK);
    GPIO_SetDir(D5_GPIO_PORT, D5_GPIO_MASK, 1);
    GPIO_ClearValue(D5_GPIO_PORT, D5_GPIO_MASK);
    GPIO_SetDir(D6_GPIO_PORT, D6_GPIO_MASK, 1);
    GPIO_ClearValue(D6_GPIO_PORT, D6_GPIO_MASK);

    while (1)
    {
        uint32_t i;
        uint32_t value;
        volatile uint32_t *pData;

        GPIO_SetValue(D3_GPIO_PORT, D3_GPIO_MASK);

        // Running "1" test
        value = 0x1;

        while (value != 0)
        {
            // Write
            pData = (volatile uint32_t *)SDRAM_BASE_ADDR;
            for (i = 0; i < (SDRAM_SIZE_BYTES / 4); i++)
            {
                *pData++ = value;
            }
            // Verify
            pData = (volatile uint32_t *)SDRAM_BASE_ADDR;
            for (i = 0; i < (SDRAM_SIZE_BYTES / 4); i++)
            {
                if (*pData++ != value)
                {
                    GPIO_SetValue(D2_GPIO_PORT, D2_GPIO_MASK);
                    while (1);      // catch the error
                }
            }

            value <<= 1;    // next bit
        }

        GPIO_SetValue(D4_GPIO_PORT, D4_GPIO_MASK);

        // Running "0" test
        value = ~0x1;

        while (value != ~0)
        {
            // Write
            pData = (volatile uint32_t *)SDRAM_BASE_ADDR;
            for (i = 0; i < (SDRAM_SIZE_BYTES / 4); i++)
            {
                *pData++ = value;
            }
            // Verify
            pData = (volatile uint32_t *)SDRAM_BASE_ADDR;
            for (i = 0; i < (SDRAM_SIZE_BYTES / 4); i++)
            {
                if (*pData++ != value)
                {
                    GPIO_SetValue(D2_GPIO_PORT, D2_GPIO_MASK);
                    while (1);      // catch the error
                }
            }

            value = (value << 1) | 0x1; // next bit
        }

        GPIO_SetValue(D5_GPIO_PORT, D5_GPIO_MASK);

        // "Const" test
        value = 0x55555555;

        // Write
        pData = (volatile uint32_t *)SDRAM_BASE_ADDR;
        for (i = 0; i < (SDRAM_SIZE_BYTES / 4 / 2); i++)
        {
            *pData++ = value;
            *pData++ = ~value;
        }
        // Verify
        pData = (volatile uint32_t *)SDRAM_BASE_ADDR;
        for (i = 0; i < (SDRAM_SIZE_BYTES / 4 / 2); i++)
        {
            if (*pData++ != value || *pData++ != ~value)
            {
                GPIO_SetValue(D2_GPIO_PORT, D2_GPIO_MASK);
                while (1);      // catch the error
            }
        }

        // For single test this will set D6 (D3..D5 are already on) indicating success.
        // For continuous test D6 will toggle every pass.
        //GPIO_ToggleValue(D6_GPIO_PORT, D6_GPIO_MASK);
				LPC_GPIO_PORT->NOT[D6_GPIO_PORT] = D6_GPIO_MASK;

#ifndef CONTINUOUS_TEST
        while(1);
#else
        GPIO_ClearValue(D3_GPIO_PORT, D3_GPIO_MASK);
        GPIO_ClearValue(D4_GPIO_PORT, D4_GPIO_MASK);
        GPIO_ClearValue(D5_GPIO_PORT, D5_GPIO_MASK);
#endif
    }
}
