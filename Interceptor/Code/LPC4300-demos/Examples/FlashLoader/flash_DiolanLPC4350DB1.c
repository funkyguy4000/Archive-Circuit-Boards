/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Memory-specific routines for Flash Loader.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include "flash_loader.h"


static uint32_t flashBase;

static uint32_t PrepareFlash(void);
static uint32_t PollData(uint32_t address, uint16_t value);
static uint32_t EraseSector(uint32_t sectorAddress);
static uint32_t WriteWord(uint32_t address, uint16_t value);


uint32_t FlashInit(void *base_of_flash, uint32_t image_size, uint32_t link_address, uint32_t flags)
{
    flashBase = (uint32_t)base_of_flash;
    PrepareFlash();

    return RESULT_OK;
}


uint32_t FlashWrite(void *block_start, uint32_t offset_into_block, uint32_t count, char const *buffer)
{
    uint32_t address = (uint32_t)block_start + offset_into_block;
    uint16_t *data   = (uint16_t *)buffer;

    do
    {
        uint32_t result = WriteWord(address, *data);
        if (result != RESULT_OK)
            return result;

        data++;
        address += sizeof(uint16_t);
        count   -= sizeof(uint16_t);
    }
    while (count >= sizeof(uint16_t));

    return RESULT_OK;
}


uint32_t FlashErase(void *block_start, uint32_t block_size)
{
     return EraseSector((uint32_t)block_start);
}


static uint32_t PrepareFlash(void)
{
    volatile int i;

    // Reset flash
    *((volatile uint16_t *)flashBase) = 0xF0;
    for (i = 0; i < 20; i++);

    // Dummy read
    i = *((volatile uint16_t *)flashBase);

    return RESULT_OK;
}


static uint32_t PollData(uint32_t address, uint16_t value)
{
    uint16_t temp;

    // Wait until valid data uppears or timeout expires
    do
    {
        temp = *((volatile uint16_t *)address);
        if ((temp & (1 << 7)) == (value & (1 << 7)))
            return RESULT_OK;
    } while ((temp & (1 << 5)) == 0);

    // Timeout expires - check for valid data one more time
    temp = *((volatile uint16_t *)address);
    if ((temp & (1 << 7)) == (value & (1 << 7)))
        return RESULT_OK;
    else
        return RESULT_ERROR;
}


static uint32_t EraseSector(uint32_t sectorAddress)
{
    // Perform sector erase sequence
    *((volatile uint16_t *)flashBase + 0x555) = 0xAA;
    *((volatile uint16_t *)flashBase + 0x2AA) = 0x55;
    *((volatile uint16_t *)flashBase + 0x555) = 0x80;
    *((volatile uint16_t *)flashBase + 0x555) = 0xAA;
    *((volatile uint16_t *)flashBase + 0x2AA) = 0x55;
    *((volatile uint16_t *)sectorAddress    ) = 0x30;

    // Poll for erase finish
    return PollData(sectorAddress, 0xFFFF);
}


static uint32_t WriteWord(uint32_t address, uint16_t value)
{
    // Perform write sequence
    *((volatile uint16_t *)flashBase + 0x555) = 0xAA;
    *((volatile uint16_t *)flashBase + 0x2AA) = 0x55;
    *((volatile uint16_t *)flashBase + 0x555) = 0xA0;
    *((volatile uint16_t *)address          ) = value;

    // Poll for write finish
    return PollData(address, value);
}
