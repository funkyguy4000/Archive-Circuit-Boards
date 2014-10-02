/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: AT24 EEPROM driver.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#ifndef __AT24XX_H__
#define __AT24XX_H__

void AT24_Init(void);
void AT24_Write(uint8_t sla, uint32_t address, uint8_t addressLength, uint8_t *buffer, uint16_t count);
void AT24_Read(uint8_t sla, uint32_t address, uint8_t addressLength, uint8_t *buffer, uint16_t count);

#endif //__AT24XX_H__
