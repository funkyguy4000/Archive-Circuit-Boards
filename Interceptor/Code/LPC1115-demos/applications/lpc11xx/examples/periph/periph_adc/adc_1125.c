/*
 * @brief LPC1125 ADC example
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

#if defined(CHIP_LPC1125)

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

static volatile int ticks;
static bool sequenceComplete, thresholdCrossed;

#define TICKRATE_HZ (100)	/* 100 ticks per second */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	static uint32_t count;

	/* Every 1/2 second */
	count++;
	if (count >= (TICKRATE_HZ / 2)) {
		count = 0;

		/* Manual start for ADC conversion sequence A */
		Chip_ADC_StartSequencer(LPC_ADC, ADC_SEQA_IDX);
	}
}

/**
 * @brief	Handle interrupt from ADC sequencer A
 * @return	Nothing
 */
void ADCA_IRQHandler(void)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		sequenceComplete = true;
	}

	/* Threshold crossing interrupt on channel 1? */
	if (pending & ADC_FLAGS_THCMP_MASK(1)) {
		thresholdCrossed = true;
	}

	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC, pending);
}

/**
 * @brief	main routine for ADC example
 * @return	Function should not exit
 */
int main(void)
{
	uint32_t rawSample;
	int j;

	SystemCoreClockUpdate();
	Board_Init();
	DEBUGSTR("ADC Demo\r\n");

	/* Setup ADC for 12-bit mode and normal power */
	Chip_ADC_Init(LPC_ADC, 0);

	/* Setup for maximum ADC clock rate */
	Chip_ADC_SetClockRate(LPC_ADC, ADC_MAX_SAMPLE_RATE);

	/* Setup sequencer A for ADC channel 1, EOS interrupt */
#if defined(BOARD_MCORE48_1125)
	/* Setup a sequencer to do the following:
	   Perform ADC conversion of ADC channel 1 only
	   Trigger on low edge of PIO0_7 */
	Chip_ADC_SetupSequencer(LPC_ADC, ADC_SEQA_IDX, (ADC_SEQ_CTRL_CHANSEL(1) |
													ADC_SEQ_CTRL_MODE_EOS | ADC_SEQ_CTRL_HWTRIG_PIO0_7));

	/* Select ADC_1 mux for PIO1_11 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_11, (IOCON_FUNC1 | IOCON_ADMODE_EN));

	/* Setup GPIO PIO0_7 as an input (will kill LED out) */
	Chip_GPIO_WriteDirBit(LPC_GPIO, 0, 7, false);

	/* Use higher voltage trim for MCore48 board */
	Chip_ADC_SetTrim(LPC_ADC, ADC_TRIM_VRANGE_HIGHV);
#endif

	/* Need to do a calibration after initialization and trim */
	Chip_ADC_StartCalibration(LPC_ADC);
	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC))) {}

	/* Setup threshold 0 low and high values to about 25% and 75% of max */
	Chip_ADC_SetThrLowValue(LPC_ADC, 0, ((1 * 0xFFF) / 4));
	Chip_ADC_SetThrHighValue(LPC_ADC, 0, ((3 * 0xFFF) / 4));

	/* Clear all pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC, Chip_ADC_GetFlags(LPC_ADC));

	/* Enable ADC overrun and sequence A completion interrupts */
	Chip_ADC_EnableInt(LPC_ADC, (ADC_INTEN_SEQA_ENABLE | ADC_INTEN_OVRRUN_ENABLE));

	/* Use threshold 0 for channel 1 and enable threshold interrupt mode for
	   channel as crossing */
	Chip_ADC_SelectTH0Channels(LPC_ADC, ADC_THRSEL_CHAN_SEL_THR1(1));
	Chip_ADC_SetThresholdInt(LPC_ADC, 1, ADC_INTEN_THCMP_CROSSING);

	/* Enable ADC NVIC interrupt */
	NVIC_EnableIRQ(ADC_A_IRQn);

	/* Enable sequencer */
	Chip_ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);

	/* Setup SyTick for a periodic rate */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	/* Endless loop */
	while (1) {
		/* Sleep until something happens */
		__WFI();

		if (thresholdCrossed) {
			thresholdCrossed = false;
			DEBUGSTR("********ADC threshold event********\r\n");
		}

		/* Is a conversion sequence complete? */
		if (sequenceComplete) {
			sequenceComplete = false;

			/* Get raw sample data for channels 1-8 */
			for (j = 1; j <= 1; j++) {
				rawSample = Chip_ADC_GetDataReg(LPC_ADC, j);

				/* Show some ADC data */
				DEBUGOUT("Sample value    = 0x%x\r\n", ADC_DR_RESULT(rawSample));
				DEBUGOUT("Threshold range = 0x%x\r\n", ADC_DR_THCMPRANGE(rawSample));
				DEBUGOUT("Threshold cross = 0x%x\r\n", ADC_DR_THCMPCROSS(rawSample));
				DEBUGOUT("Overrun         = %d\r\n", ((rawSample & ADC_DR_OVERRUN) != 0));
				DEBUGOUT("Data valid      = %d\r\n", ((rawSample & ADC_SEQ_GDAT_DATAVALID) != 0));
				DEBUGSTR("\r\n");
			}
		}
	}

	/* Should not run to here */
	return 0;
}

#endif /* defined(CHIP_LPC1125) */
