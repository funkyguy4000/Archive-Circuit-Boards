/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Driver for LTV350QV LCD.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_lcd.h"
#include "lpc43xx_ssp.h"


// Flash is too slow for 32 bit mode
// 32bit mode used if framebuffer is located in external SDRAM
#define USE_16BIT_MODE


//#define LCD_PINCONFIG   (MD_PLN | MD_EHS | MD_EZI)
#define SPI_PINCONFIG   (MD_PLN | MD_EHS | MD_EZI)


// LCD configured struct
static const LCD_CFG_Type Samsung_LTV350QV = {
	320,                        /* Pixels per line */
	240,                        /* Lines per panel */
	{10,0x21-16},               /* porch setting for horizontal */
	{10,0x0A-3},                /* porch setting for vertical */
	16,                         /* HSYNC pulse width in clocks */
	4,                          /* VSYNC pulse width in clocks */
	0,                          /* AC bias frequency in clocks */
	LCD_SIGNAL_ACTIVE_LOW,      /* HSYNC polarity */
	LCD_SIGNAL_ACTIVE_LOW,      /* VSYNC polarity */
	LCD_CLK_FALLING,            /* Panel Clock Edge Polarity */
	LCD_SIGNAL_ACTIVE_LOW,      /* Output Enable polarity */
	0,                          /* line_end_delay 0 if not use */
#ifdef USE_16BIT_MODE
    LCD_BPP16_565,              /* Maximum bits per pixel the display supports */
#else
	LCD_BPP24,                  /* Maximum bits per pixel the display supports */
#endif
	LCD_TFT,                    /* LCD panel type */
	LCD_COLOR_FORMAT_RGB,       /* BGR or RGB */
	FALSE                       /* Dual panel, TRUE = dual panel display */
};


void DelayMs(uint32_t delay);
static void LTV350QV_WriteReg(uint8_t reg, uint16_t value);


void LTV350QV_Init(void *frameBuffer)
{
//    SSP_CFG_Type sspCfg = {
//        .Databit = SSP_DATABIT_8,
//        .CPHA = SSP_CPHA_SECOND,
//        .CPOL = SSP_CPOL_LO,
//        .Mode = SSP_MASTER_MODE,
//        .FrameFormat = SSP_FRAME_SPI,
//        .ClockRate = 40000000
//    };

	SSP_CFG_Type sspCfg;
	
	sspCfg.Databit = SSP_DATABIT_8;
	sspCfg.CPHA = SSP_CPHA_SECOND;
	sspCfg.CPOL = SSP_CPOL_LO;
	sspCfg.Mode = SSP_MASTER_MODE;
	sspCfg.FrameFormat = SSP_FRAME_SPI;
	sspCfg.ClockRate = 40000000;

    // Enable clocks
    // LTV350 maximum pixel clock is 10 MHz, so core frequency must be divided
    // The best way to do this is to set divider in LPC_LCD->POL register.
    // But we don't want to edit NXP drivers, so pass LCD clock thrue divider E.
	CGU_SetDIV(CGU_CLKSRC_IDIVE, 16/*CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 8000000UL*/);
    CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_CLKSRC_IDIVE);
    CGU_EntityConnect(CGU_CLKSRC_IDIVE, CGU_BASE_LCD);


    // Configure LCD pins
    scu_pinmux(0x07, 7, LCD_PINCONFIG, FUNC0);  // GPIO3[15] = RESET
    GPIO_SetDir(3, 1 << 15, 1);
    GPIO_SetValue(3, 1 << 15);

//    scu_pinmux(0x07, 7, LCD_PINCONFIG, FUNC3); 	/* LCD_PWR @ P7.7 */
    scu_pinmux(0x04, 7, LCD_PINCONFIG, FUNC0);	/* LCD_DCLK @ P4.7 */
//    scu_pinmux(0x04, 6, LCD_PINCONFIG, FUNC2); 	/* LCD_ENAB_M @ P4.6 */
    scu_pinmux(0x04, 5, LCD_PINCONFIG, FUNC2);	/* LCD_FP @ P4.5 */
//    scu_pinmux(0x07, 0, LCD_PINCONFIG, FUNC3);	/* LCD_LE @ P7.0 */
    scu_pinmux(0x07, 6, LCD_PINCONFIG, FUNC3);	/* LCD_LP @ P7.6 */
    // Red
#ifndef USE_16BIT_MODE
    scu_pinmux(0x04, 1, LCD_PINCONFIG, FUNC2);	/* LCD_VD_0 @ P4.1 */
    scu_pinmux(0x04, 4, LCD_PINCONFIG, FUNC2);	/* LCD_VD_1 @ P4.4*/
    scu_pinmux(0x04, 3, LCD_PINCONFIG, FUNC2);	/* LCD_VD_2 @ P4.3 */
#endif
    scu_pinmux(0x04, 2, LCD_PINCONFIG, FUNC2);	/* LCD_VD_3 @ P4.2 */
    scu_pinmux(0x08, 7, LCD_PINCONFIG, FUNC3);	/* LCD_VD_4 @ P8.7 */
    scu_pinmux(0x08, 6, LCD_PINCONFIG, FUNC3);	/* LCD_VD_5 @ P8.6 */
    scu_pinmux(0x08, 5, LCD_PINCONFIG, FUNC3);	/* LCD_VD_6 @ P8.5 */
    scu_pinmux(0x08, 4, LCD_PINCONFIG, FUNC3);	/* LCD_VD_7 @ P8.4 */
    // Green
#ifndef USE_16BIT_MODE
    scu_pinmux(0x07, 5, LCD_PINCONFIG, FUNC3);	/* LCD_VD_8 @ P7.5 */
    scu_pinmux(0x04, 8, LCD_PINCONFIG, FUNC2);	/* LCD_VD_9 @ P4.8 */
#endif
    scu_pinmux(0x04, 10, LCD_PINCONFIG, FUNC2);	/* LCD_VD_10 @ P4.10 */
    scu_pinmux(0x04, 9, LCD_PINCONFIG, FUNC2); 	/* LCD_VD_11 @ P4.9 */
    scu_pinmux(0x08, 3, LCD_PINCONFIG, FUNC3); 	/* LCD_VD_12 @ P8.3 */
    scu_pinmux(0x0B, 6, LCD_PINCONFIG, FUNC2); 	/* LCD_VD_13 @ PB.6 */
    scu_pinmux(0x0B, 5, LCD_PINCONFIG, FUNC2); 	/* LCD_VD_14 @ PB.5 */
    scu_pinmux(0x0B, 4, LCD_PINCONFIG, FUNC2); 	/* LCD_VD_15 @ PB.4 */
    // Blue
#ifndef USE_16BIT_MODE
    scu_pinmux(0x07, 4, LCD_PINCONFIG, FUNC3);	/* LCD_VD_16 @ P7.4 */
    scu_pinmux(0x07, 3, LCD_PINCONFIG, FUNC3);	/* LCD_VD_17 @ P7.3 */
    scu_pinmux(0x07, 2, LCD_PINCONFIG, FUNC3); 	/* LCD_VD_18 @ P7.2 */
#endif
    scu_pinmux(0x07, 1, LCD_PINCONFIG, FUNC3); 	/* LCD_VD_19 @ P7.1 */
    scu_pinmux(0x0B, 3, LCD_PINCONFIG, FUNC2); 	/* LCD_VD_20 @ PB.3 */
    scu_pinmux(0x0B, 2, LCD_PINCONFIG, FUNC2); 	/* LCD_VD_21 @ PB.2 */
    scu_pinmux(0x0B, 1, LCD_PINCONFIG, FUNC2); 	/* LCD_VD_22 @ PB.1 */
    scu_pinmux(0x0B, 0, LCD_PINCONFIG, FUNC2); 	/* LCD_VD_23 @ PB.0 */

// Unused pins for 16-bit mode (R[0..2], G[0..1], B[0..2])
// Set to output 0 to not affect colors
#ifdef USE_16BIT_MODE
    scu_pinmux(0x04, 1, LCD_PINCONFIG, FUNC0);	/* GPIO2[1] @ P4.1 */
    scu_pinmux(0x04, 4, LCD_PINCONFIG, FUNC0);	/* GPIO2[4] @ P4.4*/
    scu_pinmux(0x04, 3, LCD_PINCONFIG, FUNC0);	/* GPIO2[3] @ P4.3 */
    scu_pinmux(0x07, 5, LCD_PINCONFIG, FUNC0);	/* GPIO3[13] @ P7.5 */
    scu_pinmux(0x04, 8, LCD_PINCONFIG, FUNC4);	/* GPIO5[12] @ P4.8 */
    scu_pinmux(0x07, 4, LCD_PINCONFIG, FUNC0);	/* GPIO3[12] @ P7.4 */
    scu_pinmux(0x07, 3, LCD_PINCONFIG, FUNC0);	/* GPIO3[11] @ P7.3 */
    scu_pinmux(0x07, 2, LCD_PINCONFIG, FUNC0); 	/* GPIO3[10] @ P7.2 */
    GPIO_SetDir(2, (1 << 1) | (1 << 4) | (1 << 3), 1);
    GPIO_ClearValue(2, (1 << 1) | (1 << 4) | (1 << 3));
    GPIO_SetDir(3, (1 << 10) | (1 << 11) | (1 << 12) | (1 << 13), 1);
    GPIO_ClearValue(3, (1 << 10) | (1 << 11) | (1 << 12) | (1 << 13));
    GPIO_SetDir(5, (1 << 12), 1);
    GPIO_ClearValue(2, (1 << 12));
#endif

    // Configure SPI pins
    scu_pinmux(0x03, 3, SPI_PINCONFIG, FUNC2);  // SSP0_SCK
    scu_pinmux(0x03, 7, SPI_PINCONFIG, FUNC2);  // SSP0_MISO
    scu_pinmux(0x03, 8, SPI_PINCONFIG, FUNC2);  // SSP0_MOSI
    scu_pinmux(0x03, 6, SPI_PINCONFIG, FUNC0);  // SSEL = GPIO0[6]
    GPIO_SetDir(0, 1 << 6, 1);
    GPIO_SetValue(0, 1 << 6);


    // Reset LCD controller
    LPC_RGU->RESET_CTRL0 = (1UL << 16);

    // Configure LCD controller
	LCD_Init(LPC_LCD, (LCD_CFG_Type*)&Samsung_LTV350QV);
	LCD_SetFrameBuffer(LPC_LCD, frameBuffer);
	LCD_Power(LPC_LCD, ENABLE);

    // Configure SPI
    SSP_Init(LPC_SSP0, &sspCfg);
    SSP_Cmd(LPC_SSP0, ENABLE);

    // Reset LCD
    GPIO_ClearValue(3, 1 << 15);
    DelayMs(1);
    GPIO_SetValue(3, 1 << 15);
    DelayMs(1);

    // Init sequence
    LTV350QV_WriteReg(0x09, 0x0000);
    DelayMs(15);
    LTV350QV_WriteReg(0x09, 0x4000);
    LTV350QV_WriteReg(0x0A, 0x2000);
    LTV350QV_WriteReg(0x09, 0x4055);
    DelayMs(55);
    LTV350QV_WriteReg(0x01, 0x409D);
    LTV350QV_WriteReg(0x02, 0x0204);
    LTV350QV_WriteReg(0x03, 0x0100);
    LTV350QV_WriteReg(0x04, 0x3000);
    LTV350QV_WriteReg(0x05, 0x4003);
    LTV350QV_WriteReg(0x06, 0x000A);
    LTV350QV_WriteReg(0x07, 0x0021);
    LTV350QV_WriteReg(0x08, 0x0C00);
    LTV350QV_WriteReg(0x10, 0x0103);
    LTV350QV_WriteReg(0x11, 0x0301);
    LTV350QV_WriteReg(0x12, 0x1F0F);
    LTV350QV_WriteReg(0x13, 0x1F0F);
    LTV350QV_WriteReg(0x14, 0x0707);
    LTV350QV_WriteReg(0x15, 0x0307);
    LTV350QV_WriteReg(0x16, 0x0707);
    LTV350QV_WriteReg(0x17, 0x0000);
    LTV350QV_WriteReg(0x18, 0x0004);
    LTV350QV_WriteReg(0x19, 0x0000);
    DelayMs(20);
    LTV350QV_WriteReg(0x09, 0x4A55);
    LTV350QV_WriteReg(0x05, 0x5003);
}


void LTV350QV_SetFrameBuffer(void *frameBuffer)
{
    LCD_SetFrameBuffer(LPC_LCD, frameBuffer);
}


static void LTV350QV_WriteReg(uint8_t reg, uint16_t value)
{
    uint8_t buffer[3];
    SSP_DATA_SETUP_Type transfer;

    transfer.tx_data = buffer;
    transfer.rx_data = NULL;
    transfer.length  = 3;

    // Index data
    buffer[0] = 0x74;
    buffer[1] = 0x00;
    buffer[2] = reg & 0x7F;

    GPIO_ClearValue(0, 1 << 6);
    SSP_ReadWrite(LPC_SSP0, &transfer, SSP_TRANSFER_POLLING);
    GPIO_SetValue(0, 1 << 6);
    DelayMs(1);

    transfer.tx_data = buffer;
    transfer.rx_data = NULL;
    transfer.length  = 3;

    // Instruction data
    buffer[0] = 0x76;
    buffer[1] = value >> 8;
    buffer[2] = value;

    GPIO_ClearValue(0, 1 << 6);
    SSP_ReadWrite(LPC_SSP0, &transfer, SSP_TRANSFER_POLLING);
    GPIO_SetValue(0, 1 << 6);
    DelayMs(1);
}
