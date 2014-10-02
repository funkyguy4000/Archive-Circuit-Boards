/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "IPC Blinking DualDebug" example (M0 core).
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "ipc_comm.h"


// Interval when LED remains ON after command receive
#define BLINK_INTERVAL      100000


typedef struct
{
    uint8_t port;
    uint32_t mask;
} GPIO_PIN;

static const GPIO_PIN leds[4] = {
    { D3_GPIO_PORT, D3_GPIO_MASK },
    { D4_GPIO_PORT, D4_GPIO_MASK },
    { D5_GPIO_PORT, D5_GPIO_MASK },
    { D6_GPIO_PORT, D6_GPIO_MASK }
};


int main(void)
{
    int i;
    uint32_t cmd;
    uint32_t blink[4] = { 0, 0, 0, 0 };

    // Configure LEDs
    scu_pinmux(D3_SCU_PORT, D3_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D4_SCU_PORT, D4_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D5_SCU_PORT, D5_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D6_SCU_PORT, D6_SCU_PIN, MD_BUK, FUNC4);
    for (i = 0; i < 4; i++)
    {
        GPIO_SetDir(leds[i].port, leds[i].mask, 1);     // output
        GPIO_ClearValue(leds[i].port, leds[i].mask);    // led off
    }

    while (1)
    {
        // Check for command
        if ((cmd = CMD_M4) != 0)
        {
            CMD_M4 = 0;
            RSP_M0 = RSP_DONE;

            switch (cmd)
            {
            case CMD_SET_LED3:
                blink[0] = BLINK_INTERVAL;
                GPIO_SetValue(leds[0].port, leds[0].mask);  // on
                break;
            case CMD_SET_LED4:
                blink[1] = BLINK_INTERVAL;
                GPIO_SetValue(leds[1].port, leds[1].mask);  // on
                break;
            case CMD_SET_LED5:
                blink[2] = BLINK_INTERVAL;
                GPIO_SetValue(leds[2].port, leds[2].mask);  // on
                break;
            case CMD_SET_LED6:
                blink[3] = BLINK_INTERVAL;
                GPIO_SetValue(leds[3].port, leds[3].mask);  // on
                break;
            }
        }

        // Check if it is needed to turn LED off
        for (i = 0; i < 4; i++)
        {
            if (blink[i] > 0)
            {
                if (--blink[i] == 0)
                {
                    GPIO_ClearValue(leds[i].port, leds[i].mask);    // off
                }
            }
        }

    }
}
