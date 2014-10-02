/*
 * @brief LPC11xx ADC example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
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
#include <stdio.h>

#if !defined(CHIP_LPC1125)

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

static ADC_CLOCK_SETUP_T ADCSetup;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void Init_ADC_PinMux(void)
{
#if (defined(BOARD_NXP_XPRESSO_11U14) || defined(BOARD_NGX_BLUEBOARD_11U24))
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 11, FUNC2);
#elif defined(BOARD_NXP_XPRESSO_11C24)
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_11, FUNC2);
#else
	#error "Pin muxing for ADC not configured"
#endif
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for ADC example
 * @return	Function should not exit
 */
int main(void)
{
	uint16_t dataADC;
	int j;

	SystemCoreClockUpdate();
	Board_Init();
	Init_ADC_PinMux();
	DEBUGSTR("ADC Demo\r\n");

	/* ADC Init */
	Chip_ADC_Init(LPC_ADC, &ADCSetup);
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE);

	while (1) {
		/* Start A/D conversion */
		Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

		/* Waiting for A/D conversion complete */
		while (Chip_ADC_ReadStatus(LPC_ADC, ADC_CH0, ADC_DR_DONE_STAT) != SET) {}

		/* Read ADC value */
		Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &dataADC);

		/* Print ADC value */
		DEBUGOUT("ADC value is 0x%x\r\n", dataADC);

		/* Delay */
		j = 500000;
		while (j--) {}
	}

	/* Should not run to here */
	return 0;
}

#endif /* !defined(CHIP_LPC1125) */
