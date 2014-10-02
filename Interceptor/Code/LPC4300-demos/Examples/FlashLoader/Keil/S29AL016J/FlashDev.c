/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2006                         */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Device Description for S29AL016J (16-bit Bus)         */
/*                                                                     */
/***********************************************************************/

#include "..\FlashOS.H"        // FlashOS Structures

#ifdef TOP
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "S29AL016J_TOP-Flash",      // Device Name 
   EXT16BIT,                   // Device Type
   0x0000000,                  // Device Start Address
   0x200000,                   // Device Size in Bytes (2MB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   900,                        // Erase Sector Timeout 900 mSec
// Specify Size and Address of Sectors
   0x10000,     0x000000,      // Sector Size 64kB (31 Sectors)
   0x08000,     0x1F0000,      // Sector Size 32kB (1 Sector)
   0x02000,     0x1F8000,      // Sector Size 8kB (2 Sectors)
   0x04000,     0x1FC000,      // Sector Size 16kB (1 Sector)

   SECTOR_END
};
#endif


#ifdef BOTTOM
struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   "S29AL016J_BOT-Flash",      // Device Name 
   EXT16BIT,                   // Device Type
   0x0000000,                  // Device Start Address
   0x200000,                   // Device Size in Bytes (2MB)
   1024,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   100,                        // Program Page Timeout 100 mSec
   900,                        // Erase Sector Timeout 900 mSec
// Specify Size and Address of Sectors
   0x04000,     0x000000,      // Sector Size 16kB (1 Sector)
   0x02000,     0x004000,      // Sector Size 8kB (2 Sectors)   
   0x08000,     0x008000,      // Sector Size 32kB (1 Sector)
   0x10000,     0x010000,      // Sector Size 64kB (31 Sectors)

   SECTOR_END
};
#endif



