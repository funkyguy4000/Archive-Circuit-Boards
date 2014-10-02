/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "USB ROM HID mouse" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/


#include <string.h>

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_cgu.h"
#include "lpc4350_db1_emc.h"

#include "usbd/usbd_core.h"
#include "usbd/usbd_hw.h"
#include "usbd/usbd_rom_api.h"


static USBD_HANDLE_T hUsb;


// Extern stuff
extern uint8_t *HID_MouseReport;
uint32_t copy_descriptors(USB_CORE_DESCS_T* pDesc, uint32_t mem_base);
ErrorCode_t usb_hid_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size);
ErrorCode_t USB_Configure_Event(USBD_HANDLE_T hUsb);


void USB0_IRQHandler(void)
{
    USBD_API->hw->ISR(hUsb);
}


int main(void)
{
    ErrorCode_t ret;
    USBD_API_INIT_PARAM_T usb_param;
    USB_CORE_DESCS_T desc;
    USB_INTERFACE_DESCRIPTOR* pIntfDesc;

    // Configure clocks
	SystemInit();

    // External flash note:
    // This demo can be larger than 16k (bootloader initializes only A[13..0])
    // So initialize memory before clock setup
    MemoryPinInit();
    EMCFlashInit();

    CGU_Init();
    CGU_SetPLL0();
    CGU_EnableEntity(CGU_CLKSRC_PLL0, ENABLE);
    CGU_EntityConnect(CGU_CLKSRC_PLL0, CGU_BASE_USB0);

    // Initialize callback structures
    memset((void*)&usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));

    usb_param.usb_reg_base = LPC_USB0_BASE;
    usb_param.max_num_ep = 6;

    usb_param.mem_base = 0x20004000;
    usb_param.mem_size = 0x2000;
    usb_param.USB_Configure_Event = USB_Configure_Event;

    // For eagle/raptor the local SRAM is not accesable to USB
    // so copy the descriptors to USB accessable memory
    copy_descriptors(&desc, usb_param.mem_base + usb_param.mem_size);

    // Turn on the phy
    LPC_CREG->CREG0 &= ~(1<<5);

    // Initialize USB
    ret = USBD_API->hw->Init(&hUsb, &desc, &usb_param);
    if (ret == LPC_OK)
    {
        pIntfDesc = (USB_INTERFACE_DESCRIPTOR*)((uint32_t)desc.high_speed_desc + USB_CONFIGUARTION_DESC_SIZE);
        ret = usb_hid_init(hUsb, pIntfDesc, &usb_param.mem_base, &usb_param.mem_size);
        if (ret == LPC_OK)
        {
            NVIC_EnableIRQ(USB0_IRQn);          // enable interrupt
            USBD_API->hw->Connect(hUsb, 1);     // connect
        }
        else
        {
            while (1);  // catch error
        }
    }
    else
    {
        while (1);  // catch error
    }


    // Configure buttons
    scu_pinmux(S1_SCU_PORT, S1_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S2_SCU_PORT, S2_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S3_SCU_PORT, S3_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S4_SCU_PORT, S4_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    GPIO_SetDir(S1_GPIO_PORT, S1_GPIO_MASK, 0);
    GPIO_SetDir(S2_GPIO_PORT, S2_GPIO_MASK, 0);
    GPIO_SetDir(S3_GPIO_PORT, S3_GPIO_MASK, 0);
    GPIO_SetDir(S4_GPIO_PORT, S4_GPIO_MASK, 0);

    while (1)
    {
        uint8_t b = 0, x = 0, y = 0;

        if ((GPIO_ReadValue(S1_GPIO_PORT) & S1_GPIO_MASK) == 0) x = 1;
        if ((GPIO_ReadValue(S2_GPIO_PORT) & S2_GPIO_MASK) == 0) x = (uint8_t)-1;
        if ((GPIO_ReadValue(S3_GPIO_PORT) & S3_GPIO_MASK) == 0) y = 1;
        if ((GPIO_ReadValue(S4_GPIO_PORT) & S4_GPIO_MASK) == 0) y = (uint8_t)-1;

        HID_MouseReport[0] = b; // buttons
        HID_MouseReport[1] = x; // x
        HID_MouseReport[2] = y; // y
    }
}
