/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "IPC Blinking" example (M0 core).
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "ipc_protocol.h"


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


static void HandleCommand(void);


void M0_M4CORE_IRQHandler(void)
{
    // Clear event flag
    LPC_CREG->M4TXEVENT = 0;
    // Handle command
    HandleCommand();
}


int main(void)
{
    int i;
    uint8_t *message = (uint8_t *)MSG_BASE_ADDR;

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


    NVIC_ClearPendingIRQ(M0_M4CORE_IRQn);
    NVIC_EnableIRQ(M0_M4CORE_IRQn);


    // Send "init finish" message
    message[0] = MSG_INIT_FINISH;
    __DSB();    // ensure completion of memory access
    __SEV();    // send event

    // Handle commands in interrupt
    while (1)
    {
    }
}


static void HandleCommand(void)
{
    uint8_t *command = (uint8_t *)CMD_BASE_ADDR;
    uint8_t *message = (uint8_t *)MSG_BASE_ADDR;
    uint8_t i;

    // Set return code
    message[0] = command[0];

    switch (command[0])
    {
    case CMD_SET_LED:

        for (i = 0; i < 4; i++)
        {
            if (command[1] & (1 << i))
            {
                GPIO_SetValue(leds[i].port, leds[i].mask);
            }
            else
            {
                GPIO_ClearValue(leds[i].port, leds[i].mask);
            }
        }

        message[1] = STATUS_CMD_OK;
        break;

    default:
        message[1] = STATUS_CMD_UNKNOWN;
        break;
    }

    // Send message
    __DSB();    // ensure completion of memory access
    __SEV();    // send event
}
