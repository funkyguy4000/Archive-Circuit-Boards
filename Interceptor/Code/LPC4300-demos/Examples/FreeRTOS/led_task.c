/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: LED task code for "FreeRTOS" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_gpio.h"

// Kernel includes.
#include "FreeRTOS.h"
#include "task.h"


struct LedDesc
{
    uint8_t port;
    uint32_t mask;
};

static const struct LedDesc leds[5] =
{
    { D2_GPIO_PORT, D2_GPIO_MASK },
    { D3_GPIO_PORT, D3_GPIO_MASK },
    { D4_GPIO_PORT, D4_GPIO_MASK },
    { D5_GPIO_PORT, D5_GPIO_MASK },
    { D6_GPIO_PORT, D6_GPIO_MASK },
};


static const uint8_t blinkModes[6][20] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // off
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, // on
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // slow blink
    { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 }, // fast blink
    { 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0 }, // double blink
    { 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 }, // tripple blink
};

//static

void vLEDTask(void *pvParameters)
{
    uint8_t nled = (uint32_t)pvParameters;
    uint8_t mode = (uint32_t)pvParameters >> 8;
    portTickType lastWakeTime;
    int i;

    lastWakeTime = xTaskGetTickCount();

    while (1)
    {
        for (i = 0; i < 20; i++)
        {
            if (blinkModes[mode][i])
                GPIO_SetValue(leds[nled].port, leds[nled].mask);
            else
                GPIO_ClearValue(leds[nled].port, leds[nled].mask);

            vTaskDelayUntil(&lastWakeTime, 100 / portTICK_RATE_MS);
        }
    }
}