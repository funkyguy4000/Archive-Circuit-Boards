/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "IPC Blinking DualDebug" example (M4 core).
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "ipc_comm.h"


#define DEBOUNCE    20000


typedef struct
{
    uint8_t port;
    uint32_t mask;
} GPIO_PIN;

static const GPIO_PIN buttons[4] = {
    { S1_GPIO_PORT, S1_GPIO_MASK },
    { S2_GPIO_PORT, S2_GPIO_MASK },
    { S3_GPIO_PORT, S3_GPIO_MASK },
    { S4_GPIO_PORT, S4_GPIO_MASK }
};


int main(void)
{
    int i;
    uint32_t deb[4] = { 0, 0, 0, 0 };

    SystemInit();
    CGU_Init();

    // Configure buttons
    scu_pinmux(S1_SCU_PORT, S1_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S2_SCU_PORT, S2_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S3_SCU_PORT, S3_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S4_SCU_PORT, S4_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    for (i = 0; i < 4; i++)
    {
        GPIO_SetDir(buttons[i].port, buttons[i].mask, 0);   // input
    }

    while (1)
    {
        // Check if buttons are pressed
        for (i = 0; i < 4; i++)
        {
            if ((GPIO_ReadValue(buttons[i].port) & buttons[i].mask) == 0)
            {
                if (deb[i] < DEBOUNCE)
                {
                    if (++deb[i] >= DEBOUNCE)
                    {
                        CMD_M4 = CMD_SET_LED3 + i;  // send command
                        while (RSP_M0 == 0);        // wait for response
                        RSP_M0 = 0;                 // clear response
                    }
                }
            }
            else
            {
                deb[i] = 0;
            }
        }
    }
}
