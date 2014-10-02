/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Driver for TFP410 transmitter.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_lcd.h"
#include "lpc43xx_i2c.h"
#include "lpc43xx_gpio.h"


// Flash is too slow for 32 bit mode
// 32bit mode used if framebuffer is located in external SDRAM
//#define USE_16BIT_MODE

//#define LCD_PINCONFIG   (MD_PLN | MD_EHS | MD_EZI)

// TFP410 register addresses
#define CTL_1_MODE      0x08
#define CTL_2_MODE      0x09
#define CTL_3_MODE      0x0A
#define CFG             0x0B
#define DE_DLY          0x32
#define DE_CTL          0x33
#define DE_TOP          0x34


// LCD configuration
static const LCD_CFG_Type lcdCfg = {
	1024,                       /* Pixels per line */
	768,                        /* Lines per panel */
	{5,5-3},                    /* porch setting for horizontal */
	{5,5-3},                    /* porch setting for vertical */
	3,                          /* HSYNC pulse width in clocks */
	3,                          /* VSYNC pulse width in clocks */
	0,                          /* AC bias frequency in clocks */
	LCD_SIGNAL_ACTIVE_LOW,      /* HSYNC polarity */
	LCD_SIGNAL_ACTIVE_LOW,      /* VSYNC polarity */
	LCD_CLK_FALLING,            /* Panel Clock Edge Polarity */
	LCD_SIGNAL_ACTIVE_HIGH,     /* Output Enable polarity */
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


extern void DelayMs(uint32_t delay);
static void TFP410_WriteReg(uint8_t reg, uint8_t value);


void TFP410_Init(void *frameBuffer)
{
//    uint32_t dotclk, pcd;

    // Enable clocks
    // The best way to do this is to set divider in LPC_LCD->POL register.
    // But we don't want to edit NXP drivers, so pass LCD clock thrue divider E.
    CGU_SetDIV(CGU_CLKSRC_IDIVE, CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 18000000UL);
    CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_CLKSRC_IDIVE);
    CGU_EntityConnect(CGU_CLKSRC_IDIVE, CGU_BASE_LCD);

//    CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_LCD);

    // ISEL/RSTn
    scu_pinmux(0x07, 7, LCD_PINCONFIG, FUNC0); 	/* GPIO3[15] @ P7.7 */
    GPIO_SetDir(3, 1 << 15, 1);
    GPIO_SetValue(3, 1 << 15);

	// pin set up for LCD
//    scu_pinmux(0x07, 7, LCD_PINCONFIG, FUNC3); 	/* LCD_PWR @ P7.7 */
    scu_pinmux(0x04, 7, LCD_PINCONFIG, FUNC0);	/* LCD_DCLK @ P4.7 */
    scu_pinmux(0x04, 6, LCD_PINCONFIG, FUNC2); 	/* LCD_ENAB_M @ P4.6 */
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

   // Configure LCD controller
	LCD_Init(LPC_LCD, (LCD_CFG_Type*)&lcdCfg);

    // Set clock divider (does not included in lpc43xx_lcd.c)
//    dotclk = 60
//        * (lcdCfg.screen_height + lcdCfg.vertical_porch.front + lcdCfg.vertical_porch.back + lcdCfg.VSync_pulse_width)
//        * (lcdCfg.screen_width + lcdCfg.horizontal_porch.front + lcdCfg.horizontal_porch.back + lcdCfg.HSync_pulse_width);
//	pcd = CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / dotclk;
//    LPC_LCD->POL |= ((pcd>>5)<<27) | (pcd&0x1F);

	LCD_SetFrameBuffer(LPC_LCD, frameBuffer);
	LCD_Power(LPC_LCD, ENABLE);

    I2C_Init(LPC_I2C1, 400000);
    I2C_Cmd(LPC_I2C1, ENABLE);


    // Reset I2C in TFP410
    GPIO_ClearValue(3, 1 << 15);
    DelayMs(1);
    GPIO_SetValue(3, 1 << 15);
    DelayMs(1);


    // Configure TFP401
    TFP410_WriteReg(DE_CTL, 0x00);      // DE enabled
    TFP410_WriteReg(CTL_3_MODE, 0x80);
    TFP410_WriteReg(CTL_2_MODE, 0x00);
    TFP410_WriteReg(CTL_1_MODE, 0x3F);  // power on
}


void TFP410_SetFrameBuffer(void *frameBuffer)
{
    LCD_SetFrameBuffer(LPC_LCD, frameBuffer);
}


static void TFP410_WriteReg(uint8_t reg, uint8_t value)
{
    I2C_M_SETUP_Type transfer;
    Status status;
    uint8_t buffer[2];// = { reg, value };

	buffer[0] = reg;
	buffer[1] = value;

    transfer.sl_addr7bit = 0x38;
    transfer.tx_data = buffer;
    transfer.tx_length = 2;
    transfer.rx_data = 0;
    transfer.rx_length = 0;
    transfer.retransmissions_max = 10;
    status = I2C_MasterTransferData(LPC_I2C1, &transfer, I2C_TRANSFER_POLLING);

    if (status == ERROR) {
        while (1);
    }
}
