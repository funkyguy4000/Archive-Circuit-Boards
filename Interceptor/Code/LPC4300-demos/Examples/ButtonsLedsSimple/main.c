/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "Buttons Leds Simple" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/


#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"

int main(void)
{
    SystemInit();
    CGU_Init();


    // Configure GPIO pins connected to LEDs as outputs
    scu_pinmux(D3_SCU_PORT, D3_SCU_PIN, MD_BUK, FUNC4);
    GPIO_SetDir(D3_GPIO_PORT, D3_GPIO_MASK, 1);
    scu_pinmux(D4_SCU_PORT, D4_SCU_PIN, MD_BUK, FUNC4);
    GPIO_SetDir(D4_GPIO_PORT, D4_GPIO_MASK, 1);
    scu_pinmux(D5_SCU_PORT, D5_SCU_PIN, MD_BUK, FUNC4);
    GPIO_SetDir(D5_GPIO_PORT, D5_GPIO_MASK, 1);
    scu_pinmux(D6_SCU_PORT, D6_SCU_PIN, MD_BUK, FUNC4);
    GPIO_SetDir(D6_GPIO_PORT, D6_GPIO_MASK, 1);

    // Configure GPIO pins connected to push buttons as inputs
    scu_pinmux(S1_SCU_PORT, S1_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    GPIO_SetDir(S1_GPIO_PORT, S1_GPIO_MASK, 0);
    scu_pinmux(S2_SCU_PORT, S2_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    GPIO_SetDir(S2_GPIO_PORT, S2_GPIO_MASK, 0);
    scu_pinmux(S3_SCU_PORT, S3_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    GPIO_SetDir(S3_GPIO_PORT, S3_GPIO_MASK, 0);
    scu_pinmux(S4_SCU_PORT, S4_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    GPIO_SetDir(S4_GPIO_PORT, S4_GPIO_MASK, 0);

    while(1)
    {
        if (GPIO_ReadValue(S1_GPIO_PORT) & S1_GPIO_MASK)
            GPIO_ClearValue(D3_GPIO_PORT, D3_GPIO_MASK);
        else
            GPIO_SetValue(D3_GPIO_PORT, D3_GPIO_MASK);

        if (GPIO_ReadValue(S2_GPIO_PORT) & S2_GPIO_MASK)
            GPIO_ClearValue(D4_GPIO_PORT, D4_GPIO_MASK);
        else
            GPIO_SetValue(D4_GPIO_PORT, D4_GPIO_MASK);

        if (GPIO_ReadValue(S3_GPIO_PORT) & S3_GPIO_MASK)
            GPIO_ClearValue(D5_GPIO_PORT, D5_GPIO_MASK);
        else
            GPIO_SetValue(D5_GPIO_PORT, D5_GPIO_MASK);

        if (GPIO_ReadValue(S4_GPIO_PORT) & S4_GPIO_MASK)
            GPIO_ClearValue(D6_GPIO_PORT, D6_GPIO_MASK);
        else
            GPIO_SetValue(D6_GPIO_PORT, D6_GPIO_MASK);
    }
}


