/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Driver for TSC2007 touchscreen controller.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_i2c.h"


// HW definitions
#define TSC_I2C         LPC_I2C1
#define TSC_SLA         0x48
#define TSC_XPATE_OHMS  100


#define TSC2007_MEASURE_TEMP0   (0x0 << 4)
#define TSC2007_MEASURE_AUX     (0x2 << 4)
#define TSC2007_MEASURE_TEMP1   (0x4 << 4)
#define TSC2007_ACTIVATE_XN     (0x8 << 4)
#define TSC2007_ACTIVATE_YN     (0x9 << 4)
#define TSC2007_ACTIVATE_YP_XN  (0xa << 4)
#define TSC2007_SETUP           (0xb << 4)
#define TSC2007_MEASURE_X       (0xc << 4)
#define TSC2007_MEASURE_Y       (0xd << 4)
#define TSC2007_MEASURE_Z1      (0xe << 4)
#define TSC2007_MEASURE_Z2      (0xf << 4)

#define TSC2007_POWER_OFF_IRQ_EN    (0x0 << 2)
#define TSC2007_ADC_ON_IRQ_DIS0     (0x1 << 2)
#define TSC2007_ADC_OFF_IRQ_EN      (0x2 << 2)
#define TSC2007_ADC_ON_IRQ_DIS1     (0x3 << 2)

#define TSC2007_12BIT           (0x0 << 1)
#define TSC2007_8BIT            (0x1 << 1)

#define	MAX_12BIT               ((1 << 12) - 1)

#define ADC_ON_12BIT    (TSC2007_12BIT | TSC2007_ADC_ON_IRQ_DIS0)

#define READ_Y      (ADC_ON_12BIT | TSC2007_MEASURE_Y)
#define READ_Z1     (ADC_ON_12BIT | TSC2007_MEASURE_Z1)
#define READ_Z2     (ADC_ON_12BIT | TSC2007_MEASURE_Z2)
#define READ_X      (ADC_ON_12BIT | TSC2007_MEASURE_X)
#define PWRDOWN     (TSC2007_12BIT | TSC2007_POWER_OFF_IRQ_EN)


typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t z1, z2;
} TSC2007_Values;


static int TSC2007_Transfer(uint8_t cmd);
static void TSC2007_ReadValues(TSC2007_Values *val);
static int TSC2007_CalcPressure(TSC2007_Values *val);


void TSC2007_Init(void)
{
    I2C_M_SETUP_Type transfer;
    uint8_t command;

    I2C_Init(TSC_I2C, 400000);
    I2C_Cmd(TSC_I2C, ENABLE);

    // Send 'setup' command
    command = 0xB0; // MAV filter enabled, Rirq=50kOhm
    transfer.sl_addr7bit = TSC_SLA;
    transfer.tx_data = &command;
    transfer.tx_length = 1;
    transfer.rx_data = 0;
    transfer.rx_length = 0;
    transfer.retransmissions_max = 1;   // no ACK on this command
    I2C_MasterTransferData(TSC_I2C, &transfer, I2C_TRANSFER_POLLING);
}


int TSC2007_CheckPressure(uint16_t *x, uint16_t *y)
{
    TSC2007_Values val;
    int pressure;

    // Get values
    TSC2007_ReadValues(&val);
    // Calculate pressure
    pressure = TSC2007_CalcPressure(&val);

    if (pressure > 0)
    {
        *x = val.x;
        *y = val.y;
    }

    return pressure;
}


static void TSC2007_ReadValues(TSC2007_Values *val)
{
    val->y = TSC2007_Transfer(READ_Y);
    val->x = TSC2007_Transfer(READ_X);
    val->z1 = TSC2007_Transfer(READ_Z1);
    val->z2 = TSC2007_Transfer(READ_Z2);
    TSC2007_Transfer(PWRDOWN);
}


static int TSC2007_CalcPressure(TSC2007_Values *val)
{
    uint16_t x, /*y,*/ z1, z2;
    uint32_t Rt;

    x = val->x;
//    y = val->y;
    z1 = val->z1;
    z2 = val->z2;

    if (x == MAX_12BIT)
        x = 0;

    if (x && z1)
    {
        Rt = z2;
        Rt -= z1;
        Rt *= x;
        Rt *= TSC_XPATE_OHMS;
        Rt /= z1;
        Rt = (Rt + 2047) >> 12;
    }
    else
        Rt = 0;

    if (Rt > MAX_12BIT)
        return -1;
    else
        return (int)Rt;
}


static int TSC2007_Transfer(uint8_t cmd)
{
    I2C_M_SETUP_Type transfer;
    Status ret;
    uint8_t rbuf[2];
    uint16_t val;

    transfer.sl_addr7bit = TSC_SLA;
    transfer.tx_data = &cmd;
    transfer.tx_length = 1;
    transfer.rx_data = 0;
    transfer.rx_length = 0;
    transfer.retransmissions_max = 10;
    ret = I2C_MasterTransferData(TSC_I2C, &transfer, I2C_TRANSFER_POLLING);
    if (ret != SUCCESS) return -1;

    transfer.sl_addr7bit = TSC_SLA;
    transfer.tx_data = 0;
    transfer.tx_length = 0;
    transfer.rx_data = rbuf;
    transfer.rx_length = 2;
    transfer.retransmissions_max = 10;
    ret = I2C_MasterTransferData(TSC_I2C, &transfer, I2C_TRANSFER_POLLING);
    if (ret != SUCCESS) return -1;

    val = (rbuf[1] >> 4) | (rbuf[0] << 4);

    return (int)val;
}

