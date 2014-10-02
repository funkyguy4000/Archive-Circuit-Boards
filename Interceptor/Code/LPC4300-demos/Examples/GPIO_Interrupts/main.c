/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "GPIO Interrupts" example.
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

    // Configure LEDs
    scu_pinmux(D3_SCU_PORT, D3_SCU_PIN, MD_PLN, FUNC4);
    scu_pinmux(D4_SCU_PORT, D4_SCU_PIN, MD_PLN, FUNC4);
    scu_pinmux(D5_SCU_PORT, D5_SCU_PIN, MD_PLN, FUNC4);
    scu_pinmux(D6_SCU_PORT, D6_SCU_PIN, MD_PLN, FUNC4);
    GPIO_SetDir(D3_GPIO_PORT, D3_GPIO_MASK, 1);
    GPIO_SetDir(D4_GPIO_PORT, D4_GPIO_MASK, 1);
    GPIO_SetDir(D5_GPIO_PORT, D5_GPIO_MASK, 1);
    GPIO_SetDir(D6_GPIO_PORT, D6_GPIO_MASK, 1);
    GPIO_ClearValue(D3_GPIO_PORT, D3_GPIO_MASK);
    GPIO_ClearValue(D4_GPIO_PORT, D4_GPIO_MASK);
    GPIO_ClearValue(D5_GPIO_PORT, D5_GPIO_MASK);
    GPIO_ClearValue(D6_GPIO_PORT, D6_GPIO_MASK);

    // Configure buttons
    scu_pinmux(S1_SCU_PORT, S1_SCU_PIN, MD_PUP | MD_EZI, FUNC4);
    scu_pinmux(S2_SCU_PORT, S2_SCU_PIN, MD_PUP | MD_EZI, FUNC4);
    scu_pinmux(S3_SCU_PORT, S3_SCU_PIN, MD_PUP | MD_EZI, FUNC4);
    scu_pinmux(S4_SCU_PORT, S4_SCU_PIN, MD_PUP | MD_EZI, FUNC4);
    GPIO_SetDir(S1_GPIO_PORT, S1_GPIO_MASK, 0);
    GPIO_SetDir(S2_GPIO_PORT, S2_GPIO_MASK, 0);
    GPIO_SetDir(S3_GPIO_PORT, S3_GPIO_MASK, 0);
    GPIO_SetDir(S4_GPIO_PORT, S4_GPIO_MASK, 0);


    // Configure button S1 to generate GPIO0 interrupt on both edges
    LPC_SCU->PINTSEL0 &= ~(0xFF << 0);
    LPC_SCU->PINTSEL0 |= (S1_GPIO_PORT << 5) | (S1_GPIO_PIN << 0);
    LPC_GPIO_PIN_INT->ISEL &= ~(1 << 0);    // edge sensitive
    LPC_GPIO_PIN_INT->SIENR = (1 << 0);     // rising edge enable
    LPC_GPIO_PIN_INT->SIENF = (1 << 0);     // falling edge enable
    NVIC_EnableIRQ(PIN_INT0_IRQn);


    // Configure button S2 to generate GPIO group interrupt 0
    LPC_GPIO_GROUP_INT0->CTRL = 0x01;   // clear interrupt, OR, edge
    ((uint32_t *)&(LPC_GPIO_GROUP_INT0->PORT_POL0))[S2_GPIO_PORT] &= ~S2_GPIO_MASK; // falling edge
    ((uint32_t *)&(LPC_GPIO_GROUP_INT0->PORT_ENA0))[S2_GPIO_PORT] |= S2_GPIO_MASK;  // enable interrupt
    NVIC_EnableIRQ(GINT0_IRQn);


    // Configure buttons S3 and S4 to renerate interrupt when BOTH are pressed
    LPC_GPIO_GROUP_INT1->CTRL = 0x07;   // clear interrupt, AND, level
    ((uint32_t *)&(LPC_GPIO_GROUP_INT1->PORT_POL0))[S3_GPIO_PORT] &= ~S3_GPIO_MASK; // low level
    ((uint32_t *)&(LPC_GPIO_GROUP_INT1->PORT_ENA0))[S3_GPIO_PORT] |= S3_GPIO_MASK;  // enable interrupt
    ((uint32_t *)&(LPC_GPIO_GROUP_INT1->PORT_POL0))[S4_GPIO_PORT] &= ~S4_GPIO_MASK; // low level
    ((uint32_t *)&(LPC_GPIO_GROUP_INT1->PORT_ENA0))[S4_GPIO_PORT] |= S4_GPIO_MASK;  // enable interrupt
    NVIC_EnableIRQ(GINT1_IRQn);


    while (1)
    {
    }
}


void GPIO0_IRQHandler(void)
{
    // Falling edge detected
    if (LPC_GPIO_PIN_INT->FALL & (1 << 0))
    {
        // Clear interrupt
        LPC_GPIO_PIN_INT->FALL = (1 << 0);
        // Button pressed - LED on
        GPIO_SetValue(D3_GPIO_PORT, D3_GPIO_MASK);
    }

    // Rising edge detected
    if (LPC_GPIO_PIN_INT->RISE & (1 << 0))
    {
        // Clear interrupt
        LPC_GPIO_PIN_INT->RISE = (1 << 0);
        // Button released - LED off
        GPIO_ClearValue(D3_GPIO_PORT, D3_GPIO_MASK);
    }
}


void GINT0_IRQHandler(void)
{
    uint32_t ctrl = LPC_GPIO_GROUP_INT0->CTRL;

    // Check if interrupt is pending
    if (ctrl & 1)
    {
        // Clear interrupt by writing 1 to the INT bit 0.
        LPC_GPIO_GROUP_INT0->CTRL = ctrl;
        // Check all interrupt sources

        if ((GPIO_ReadValue(S2_GPIO_PORT) & S2_GPIO_MASK) == 0)
        {
            // Button pressed - LED on
            GPIO_SetValue(D4_GPIO_PORT, D4_GPIO_MASK);
            // Configure interrupt to rising edge
            ((uint32_t *)&(LPC_GPIO_GROUP_INT0->PORT_POL0))[S2_GPIO_PORT] |= S2_GPIO_MASK;
        }
        else
        {
            // Button released - LED off
            GPIO_ClearValue(D4_GPIO_PORT, D4_GPIO_MASK);
            // Configure interrupt to falling edge
            ((uint32_t *)&(LPC_GPIO_GROUP_INT0->PORT_POL0))[S2_GPIO_PORT] &= ~S2_GPIO_MASK;
        }
    }
}



void GINT1_IRQHandler(void)
{
    uint32_t ctrl = LPC_GPIO_GROUP_INT1->CTRL;


    // Check if interrupt is pending
    if (ctrl & 1)
    {
        // Clear interrupt by writing 1 to the INT bit 0.
        LPC_GPIO_GROUP_INT1->CTRL = ctrl;
        // Check all interrupt sources

        // Blink LEDs D5 and D6
        LPC_GPIO_PORT->NOT[D5_GPIO_PORT] = D5_GPIO_MASK;
        LPC_GPIO_PORT->NOT[D6_GPIO_PORT] = D6_GPIO_MASK;
    }
}
