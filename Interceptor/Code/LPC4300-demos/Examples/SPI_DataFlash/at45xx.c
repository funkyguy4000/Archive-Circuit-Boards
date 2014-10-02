/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: AT45 data flash driver.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/


#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_ssp.h"


#define AT45_SSP        LPC_SSP0
#define AT45_FREQ       12000000UL
#define AT45_PAGESIZE   264

// SCU config
#define AT45_SCK_CONF   0xF, 3, MD_PLN_FAST, FUNC0
#define AT45_MISO_CONF  0xF, 2, MD_PLN_FAST, FUNC2
#define AT45_MOSI_CONF  0xF, 3, MD_PLN_FAST, FUNC2
#define AT45_SSEL_CONF  0xF, 1, MD_PLN_FAST, FUNC4  // SSEL=GPIO7[16]

// HW SSEL toggles every byte/word,
// so for AT45 flash it must be controlled manually by GPIO
#define SSEL_HIGH()     GPIO_SetValue(7, 1 << 16)
#define SSEL_LOW()      GPIO_ClearValue(7, 1 << 16)


static void AT45_WaitReady(void);


void AT45_Init(void)
{
    SSP_CFG_Type sspCfg;

    // Configure pins
    scu_pinmux(AT45_SCK_CONF);
    scu_pinmux(AT45_MISO_CONF);
    scu_pinmux(AT45_MOSI_CONF);
    scu_pinmux(AT45_SSEL_CONF);     // SSEL = GPIO7[16]
    GPIO_SetDir(7, 1 << 16, 1);
    SSEL_HIGH();

    // Configure SSP
    sspCfg.Databit = SSP_DATABIT_8;
    sspCfg.CPHA = SSP_CPHA_FIRST;
    sspCfg.CPOL = SSP_CPOL_HI;
    sspCfg.Mode = SSP_MASTER_MODE;
    sspCfg.FrameFormat = SSP_FRAME_SPI;
    sspCfg.ClockRate = AT45_FREQ;

    SSP_Init(AT45_SSP, &sspCfg);
    SSP_Cmd(AT45_SSP, ENABLE);
}


uint32_t AT45_GetID(void)
{
    uint8_t cmd[5], ret[5];
    SSP_DATA_SETUP_Type transfer;

    cmd[0] = 0x9F;  // other bytes - don't care

    transfer.tx_data = cmd;
    transfer.rx_data = ret;
    transfer.length = 5;

    SSEL_LOW();
    SSP_ReadWrite(AT45_SSP, &transfer, SSP_TRANSFER_POLLING);
    SSEL_HIGH();

    return (ret[1] << 24) | (ret[2] << 16) | (ret[3] << 8) | (ret[4]);
}


uint8_t AT45_GetStatus(void)
{
    uint8_t cmd[2], ret[2];
    SSP_DATA_SETUP_Type transfer;

    cmd[0] = 0xD7;

    transfer.tx_data = cmd;
    transfer.rx_data = ret;
    transfer.length = 2;

    SSEL_LOW();
    SSP_ReadWrite(AT45_SSP, &transfer, SSP_TRANSFER_POLLING);
    SSEL_HIGH();

    return ret[1];
}


void AT45_ErasePage(uint32_t address)
{
    uint8_t cmd[4];
    SSP_DATA_SETUP_Type transfer;

    uint32_t page = address / AT45_PAGESIZE;

    cmd[0] = 0x81;
#if AT45_PAGESIZE == 256
    cmd[1] = (uint8_t)(page >> 8);
    cmd[2] = (uint8_t)(page);
#else
    cmd[1] = (uint8_t)(page >> 7);
    cmd[2] = (uint8_t)(page << 1);
#endif

    transfer.tx_data = cmd;
    transfer.rx_data = NULL;
    transfer.length = 4;

    SSEL_LOW();
    SSP_ReadWrite(AT45_SSP, &transfer, SSP_TRANSFER_POLLING);
    SSEL_HIGH();

    AT45_WaitReady();
}


void AT45_WritePage(uint32_t address, uint8_t *buffer, uint16_t count)
{
    uint8_t cmd[4];
    SSP_DATA_SETUP_Type transfer;

    uint32_t page = address / AT45_PAGESIZE;
    uint32_t byte = address % AT45_PAGESIZE;

    cmd[0] = 0x82;
#if AT45_PAGESIZE == 256
    cmd[1] = (uint8_t)(page >> 8);
    cmd[2] = (uint8_t)(page);
    cmd[3] = (uint8_t)(byte);
#else
    cmd[1] = (uint8_t)(page >> 7);
    cmd[2] = (uint8_t)((page << 1) | (byte >> 8));
    cmd[3] = (uint8_t)(byte);
#endif

    transfer.tx_data = cmd;
    transfer.rx_data = NULL;
    transfer.length = 4;

    SSEL_LOW();
    SSP_ReadWrite(AT45_SSP, &transfer, SSP_TRANSFER_POLLING);

    transfer.tx_data = buffer;
    transfer.rx_data = NULL;
    transfer.length = count;

    SSP_ReadWrite(AT45_SSP, &transfer, SSP_TRANSFER_POLLING);
    SSEL_HIGH();

    AT45_WaitReady();
}


void AT45_ReadPage(uint32_t address, uint8_t *buffer, uint16_t count)
{
    uint8_t cmd[8];
    SSP_DATA_SETUP_Type transfer;

    uint32_t page = address / AT45_PAGESIZE;
    uint32_t byte = address % AT45_PAGESIZE;

    cmd[0] = 0xE8;
#if AT45_PAGESIZE == 256
    cmd[1] = (uint8_t)(page >> 8);
    cmd[2] = (uint8_t)(page);
    cmd[3] = (uint8_t)(byte);
#else
    cmd[1] = (uint8_t)(page >> 7);
    cmd[2] = (uint8_t)((page << 1) | (byte >> 8));
    cmd[3] = (uint8_t)(byte);
#endif

    transfer.tx_data = cmd;
    transfer.rx_data = NULL;
    transfer.length = 8;

    SSEL_LOW();
    SSP_ReadWrite(AT45_SSP, &transfer, SSP_TRANSFER_POLLING);

    transfer.tx_data = NULL;
    transfer.rx_data = buffer;
    transfer.length = count;

    SSP_ReadWrite(AT45_SSP, &transfer, SSP_TRANSFER_POLLING);
    SSEL_HIGH();
}


static void AT45_WaitReady(void)
{
    while ((AT45_GetStatus() & 0x80) == 0);
}
