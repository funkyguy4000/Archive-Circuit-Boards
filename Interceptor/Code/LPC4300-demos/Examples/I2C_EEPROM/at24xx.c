/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: AT24 EEPROM driver.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_i2c.h"
#include <string.h>


#define AT24_I2C        LPC_I2C0
#define AT24_FREQ       400000UL
#define AT24_PAGESIZE   256


static uint8_t tempbuf[AT24_PAGESIZE + 2];  // max 2 bytes for address


void AT24_Init(void)
{
    I2C_Init(AT24_I2C, AT24_FREQ);
    I2C_Cmd(AT24_I2C, ENABLE);
}


void AT24_Write(uint8_t sla, uint32_t address, uint8_t addressLength, uint8_t *buffer, uint16_t count)
{
    I2C_M_SETUP_Type setup;
    Status status;

    // Prepare address
    if (addressLength == 2)
    {
        tempbuf[0] = (uint8_t)(address >> 8);   // msb first
        tempbuf[1] = (uint8_t)(address);
    }
    else if (addressLength == 1)
    {
        tempbuf[0] = (uint8_t)(address);
    }
    else
    {
        addressLength = 0;
    }

    memcpy(tempbuf + addressLength, buffer, count);

    setup.sl_addr7bit = sla;
    setup.tx_data     = tempbuf;
    setup.tx_length   = addressLength + count;
    setup.rx_data     = 0;
    setup.rx_length   = 0;
    setup.retransmissions_max = 10;

    status = I2C_MasterTransferData(AT24_I2C, &setup, I2C_TRANSFER_POLLING);
    if (status != SUCCESS)
    {
        while (1);      // catch the error
    }
}


void AT24_Read(uint8_t sla, uint32_t address, uint8_t addressLength, uint8_t *buffer, uint16_t count)
{
    I2C_M_SETUP_Type setup;
    Status status;

    // Prepare address
    if (addressLength == 2)
    {
        tempbuf[0] = (uint8_t)(address >> 8);   // msb first
        tempbuf[1] = (uint8_t)(address);
    }
    else if (addressLength == 1)
    {
        tempbuf[0] = (uint8_t)(address);
    }
    else
    {
        addressLength = 0;
    }

    setup.sl_addr7bit = sla;
    setup.tx_data     = tempbuf;
    setup.tx_length   = addressLength;
    setup.rx_data     = buffer;
    setup.rx_length   = count;
    setup.retransmissions_max = 10;

    status = I2C_MasterTransferData(AT24_I2C, &setup, I2C_TRANSFER_POLLING);
    if (status != SUCCESS)
    {
        while (1);      // catch the error
    }
}
