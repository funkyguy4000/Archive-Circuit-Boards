/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "FreeRTOS" example.
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

// Kernel includes.
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"


// Prototypes
void InitHardware(void);
extern void vEasywebTask(void *pvParameters);
extern void vUSBTask(void *pvParameters);
extern void vLEDTask(void *pvParameters);


int main(void)
{
    // Initialize hardware
    InitHardware();

    // Create tasks
    xTaskCreate(vEasywebTask, "EASYWEB", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vUSBTask, "USBHID", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Create LED tasks
    xTaskCreate(vLEDTask, "LED_D2", configMINIMAL_STACK_SIZE, (void *)0x0100, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(vLEDTask, "LED_D3", configMINIMAL_STACK_SIZE, (void *)0x0201, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(vLEDTask, "LED_D4", configMINIMAL_STACK_SIZE, (void *)0x0302, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(vLEDTask, "LED_D5", configMINIMAL_STACK_SIZE, (void *)0x0403, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(vLEDTask, "LED_D6", configMINIMAL_STACK_SIZE, (void *)0x0504, tskIDLE_PRIORITY + 2, NULL);


	// Start the scheduler.
	vTaskStartScheduler();

    while (1);
}


void vApplicationMallocFailedHook( void )
{
	taskDISABLE_INTERRUPTS();
	for( ;; );
}


void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}


void InitHardware(void)
{
    SystemInit();
    MemoryPinInit();
    EMCFlashInit();

    CGU_Init();

    CGU_SetPLL0();
    CGU_EnableEntity(CGU_CLKSRC_PLL0, ENABLE);
    CGU_EntityConnect(CGU_CLKSRC_PLL0, CGU_BASE_USB0);

    CGU_EnableEntity(CGU_CLKSRC_ENET_RX_CLK, ENABLE);
    CGU_EnableEntity(CGU_CLKSRC_ENET_TX_CLK, ENABLE);
    CGU_EntityConnect(CGU_CLKSRC_ENET_TX_CLK, CGU_BASE_PHY_TX);
    CGU_EntityConnect(CGU_CLKSRC_ENET_RX_CLK, CGU_BASE_PHY_RX);

    // Configure LEDs
    scu_pinmux(D2_SCU_PORT, D2_SCU_PIN, MD_PLN, FUNC0);
    scu_pinmux(D3_SCU_PORT, D3_SCU_PIN, MD_PLN, FUNC4);
    scu_pinmux(D4_SCU_PORT, D4_SCU_PIN, MD_PLN, FUNC4);
    scu_pinmux(D5_SCU_PORT, D5_SCU_PIN, MD_PLN, FUNC4);
    scu_pinmux(D6_SCU_PORT, D6_SCU_PIN, MD_PLN, FUNC4);
    GPIO_SetDir(D2_GPIO_PORT, D2_GPIO_MASK, 1);
    GPIO_SetDir(D3_GPIO_PORT, D3_GPIO_MASK, 1);
    GPIO_SetDir(D4_GPIO_PORT, D4_GPIO_MASK, 1);
    GPIO_SetDir(D5_GPIO_PORT, D5_GPIO_MASK, 1);
    GPIO_SetDir(D6_GPIO_PORT, D6_GPIO_MASK, 1);
    GPIO_ClearValue(D2_GPIO_PORT, D2_GPIO_MASK);
    GPIO_ClearValue(D3_GPIO_PORT, D3_GPIO_MASK);
    GPIO_ClearValue(D4_GPIO_PORT, D4_GPIO_MASK);
    GPIO_ClearValue(D5_GPIO_PORT, D5_GPIO_MASK);
    GPIO_ClearValue(D6_GPIO_PORT, D6_GPIO_MASK);

    // Configure buttons
    scu_pinmux(S1_SCU_PORT, S1_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S2_SCU_PORT, S2_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S3_SCU_PORT, S3_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S4_SCU_PORT, S4_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    GPIO_SetDir(S1_GPIO_PORT, S1_GPIO_MASK, 0);
    GPIO_SetDir(S2_GPIO_PORT, S2_GPIO_MASK, 0);
    GPIO_SetDir(S3_GPIO_PORT, S3_GPIO_MASK, 0);
    GPIO_SetDir(S4_GPIO_PORT, S4_GPIO_MASK, 0);
}
