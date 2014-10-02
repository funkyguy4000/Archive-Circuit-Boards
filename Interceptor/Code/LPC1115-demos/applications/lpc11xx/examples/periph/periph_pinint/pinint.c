/*
 * @brief GPIO pin Interrupt example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#if defined (BOARD_NXP_XPRESSO_11U14)
/* GPIO pin for PININT interrupt */
#define GPIO_PININT					4 /* GPIO pin number mapped to PININT */
#define GPIO_PININT_PORT		0 /* GPIO port number mapped to PININT */
#define GPIO_PININT_INDEX		0 /* PININT index used for GPIO mapping */
#define PININT_IRQ_HANDLER	FLEX_INT0_IRQHandler /* PININT IRQ function name */
#define PININT_NVIC_NAME		PIN_INT0_IRQn /* PININT NVIC interrupt name */
#endif /* defined (BOARD_NXP_XPRESSO_11U14) */

#if defined (BOARD_NXP_XPRESSO_11C24)
/* GPIO pin for GPIO pin interrupt */
#define GPIO_PININT					3 /* GPIO pin number mapped to PININT */
#define GPIO_PININT_PORT		0 /* GPIO port number mapped to PININT */
#define IOCON_PIN_ID				IOCON_PIO0_3 /* IOCON pin identifer */
#define PININT_IRQ_HANDLER	PIOINT0_IRQHandler /* GPIO interrupt IRQ function name */
#define PININT_NVIC_NAME		EINT0_IRQn /* GPIO interrupt NVIC interrupt name */
#endif /* defined (BOARD_NXP_XPRESSO_11C24) */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

#if defined(CHIP_LPC11AXX) || defined(CHIP_LPC11EXX) || defined(CHIP_LPC11UXX)
/**
 * @brief	Handle interrupt from GPIO pin or GPIO pin mapped to PININT
 * @return	Nothing
 */
void PININT_IRQ_HANDLER(void)
{
	Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
}

#else
/**
 * @brief	Handle interrupt from GPIO pin or GPIO pin mapped to PININT
 * @return	Nothing
 */
void PININT_IRQ_HANDLER(void)
{
	/* Clear interrupt */
	Chip_GPIO_ClearInts(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));
}
#endif /* defined(CHIP_LPC11AXX) || defined(CHIP_LPC11EXX) || defined(CHIP_LPC11UXX) */

/**
 * @brief	Main program body
 * @return	Does not return
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();

	/* Board_Init calls Chip_GPIO_Init and enables GPIO clock if needed,
	   Chip_GPIO_Init is not called again */
	Board_Init();
	Board_LED_Set(0, false);

	/* Configure GPIO pin as input pin */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT);

	/* Different families require different setup for GPIO pin interrupts */
#if defined(CHIP_LPC11AXX) || defined(CHIP_LPC11EXX) || defined(CHIP_LPC11UXX)
#if defined (BOARD_NXP_XPRESSO_11U14)
	/* Configure pin as GPIO with pullup */
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_PININT_PORT, GPIO_PININT,
		(IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
#else
#error "Configure pin correctly for this example for your board"
#endif

	/* Enable PININT clock */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PINT);

	/* Configure interrupt channel for the GPIO pin in SysCon block */
	Chip_SYSCTL_SetPinInterrupt(GPIO_PININT_INDEX, GPIO_PININT_PORT, GPIO_PININT);

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(GPIO_PININT_INDEX));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(GPIO_PININT_INDEX));

#else
#if defined (BOARD_NXP_XPRESSO_11C24)
	/* Configure pin as GPIO with pullup */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIN_ID,
		(IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
#else
#error "Configure pin correctly for this example for your board"
#endif

/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_GPIO_SetPinModeEdge(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));
	Chip_GPIO_SetEdgeModeSingle(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));
	Chip_GPIO_SetModeLow(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));
	/* The 3 above calls can be done with a single call as follows:
	   Chip_GPIO_SetupPinInt(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT, GPIO_INT_FALLING_EDGE); */

	/* Enable GPIO pin intrerrupt */
	Chip_GPIO_EnableInt(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));
#endif /* defined(CHIP_LPC11AXX) || defined(CHIP_LPC11EXX) || defined(CHIP_LPC11UXX) */

#if defined(CHIP_LPC11EXX) || defined(CHIP_LPC11UXX)
	/* Configure channel as wake up interrupt in SysCon block */
	Chip_SYSCTL_EnablePINTWakeup(GPIO_PININT_INDEX);

#else
#if defined(CHIP_LPC11CXX) || defined(CHIP_LPC110X) || defined(CHIP_LPC11XXLV) || defined(CHIP_LPC1125)
#if defined (BOARD_NXP_XPRESSO_11C24)
	/* Enable wakeup for GPIO pin used in this example */
	Chip_SYSCTL_EnableStartPin(0x08); /* Hardcoded to PIO0_3 right for this board */
	Chip_SYSCTL_ResetStartPin(0x08); /* Hardcoded to PIO0_3 right for this board */
#else
#error "Configure pin correctly for this example for your board"
#endif

#else
	/* LPC11AXX example needs some work here */
#endif /* defined(CHIP_LPC11CXX) || defined(CHIP_LPC110X) || defined(CHIP_LPC11XXLV) || defined(CHIP_LPC1125) */
#endif /* defined(CHIP_LPC11AXX) || defined(CHIP_LPC11EXX) || defined(CHIP_LPC11UXX) */

	/* Enable all clocks, even those turned off at wake power up */
	Chip_SYSCTL_SetWakeup(~(SYSCTL_SLPWAKE_IRCOUT_PD | SYSCTL_SLPWAKE_IRC_PD |
		SYSCTL_SLPWAKE_FLASH_PD | SYSCTL_SLPWAKE_SYSOSC_PD | SYSCTL_SLPWAKE_SYSOSC_PD | SYSCTL_SLPWAKE_SYSPLL_PD));

	/* Enable interrupt in the NVIC */
	NVIC_EnableIRQ(PININT_NVIC_NAME);

	/* Go to sleep mode - LED will toggle on each wakeup event */
	while (1) {
#if defined(CHIP_LPC11AXX) || defined(CHIP_LPC11EXX) || defined(CHIP_LPC11UXX) || defined(CHIP_LPC1125)
		/* Go to deep sleep state - will wake up automatically on interrupt */
		Chip_PMU_DeepSleepState(LPC_PMU);

#elif defined(CHIP_LPC11CXX)
		/* No deep sleep on 11cxx */
		__WFI();

#else
		/* Use CPU sleep for devices without a PMU */
		__WFI();
#endif
		Board_LED_Toggle(0);
	}

	return 0;
}
