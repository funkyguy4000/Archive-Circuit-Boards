/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: AT45 data flash driver.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#ifndef __AT45XX_H__
#define __AT45XX_H__

void AT45_Init(void);
uint32_t AT45_GetID(void);
uint8_t AT45_GetStatus(void);
void AT45_ErasePage(uint32_t address);
void AT45_WritePage(uint32_t address, uint8_t *buffer, uint16_t count);
void AT45_ReadPage(uint32_t address, uint8_t *buffer, uint16_t count);

#endif //__AT45XX_H__
