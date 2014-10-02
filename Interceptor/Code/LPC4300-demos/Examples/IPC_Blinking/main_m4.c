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
#include "ipc_protocol.h"
#include "CM0_image.h"


// Button scan interval
#define SCAN_PERIOD     100


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


static volatile uint8_t m0started;

static void HandleMessage(void);
static void InitM0aProcessor(void);
static void StartM0aProcessor(void);


void M0CORE_IRQHandler(void)
{
    // Clear event flag
    LPC_CREG->M0TXEVENT = 0x00000000;
    // Handle message
    HandleMessage();
}


void SysTick_Handler(void)
{
    static uint32_t prsk = SCAN_PERIOD;
    uint8_t i, led;
    uint8_t *command = (uint8_t *)CMD_BASE_ADDR;

    if (--prsk == 0)
    {
        prsk = SCAN_PERIOD;

        led = 0;
        for (i = 0; i < 4; i++)
        {
            if ((GPIO_ReadValue(buttons[i].port) & buttons[i].mask) == 0)
            {
                led |= (1 << i);
            }
        }

        // Send command
        command[0] = CMD_SET_LED;
        command[1] = led;
        __DSB();    // ensure completion of memory access
        __SEV();    // send event
    }
}


int main(void)
{
    uint8_t i;

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

    // Configure LED D2
    scu_pinmux(D2_SCU_PORT, D2_SCU_PIN, MD_BUK, FUNC0);
    GPIO_SetDir(D2_GPIO_PORT, D2_GPIO_MASK, 1);             // output
    GPIO_ClearValue(D2_GPIO_PORT, D2_GPIO_MASK);            // low

    // Start M0
    m0started = 0;
    InitM0aProcessor();
    NVIC_ClearPendingIRQ(M0CORE_IRQn);
    NVIC_EnableIRQ(M0CORE_IRQn);
    StartM0aProcessor();
    while (!m0started);

    // Init SysTick to 1ms
    SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 1000);

    while (1)
    {
    }
}


static void HandleMessage(void)
{
    uint8_t *message = (uint8_t *)MSG_BASE_ADDR;

    // Handle messages
    if (message[0] >= 0x80)
    {
        switch (message[0])
        {
        case MSG_INIT_FINISH:
            m0started = 1;      // set flag
            break;
        }
    }
    // Handle command responses
    else
    {
        if (message[0] == CMD_SET_LED && message[1] == STATUS_CMD_OK)
        {
            LPC_GPIO_PORT->NOT[D2_GPIO_PORT] = D2_GPIO_MASK;    // toggle LED D2
        }
    }
}


static void InitM0aProcessor(void)
{
    uint32_t i;
    uint32_t imageSize = sizeof(LR0);
    volatile uint8_t *pRAM = (volatile uint8_t *)CM0_RAM_BASE;

    for (i = 0; i < imageSize; i++)
    {
        *pRAM++ = LR0[i];
    }

    LPC_CREG->M0APPMEMMAP = CM0_RAM_BASE;
}


static void StartM0aProcessor(void)
{
    uint32_t reg, val;

    // Release M0 from reset, first read status
    reg = LPC_RGU->RESET_ACTIVE_STATUS1;

    // If the M0 is being held in reset, release it...
    while (!(reg & (1 << 24)))
    {
        val = (~(reg) & (~(1 << 24)));
        LPC_RGU->RESET_CTRL1 = val;
        reg = LPC_RGU->RESET_ACTIVE_STATUS1;
    }
}
