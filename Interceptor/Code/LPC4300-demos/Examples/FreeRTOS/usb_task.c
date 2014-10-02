/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: USB task code for "FreeRTOS" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include <string.h>
#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_gpio.h"

#include "usbd/usbd.h"
#include "usbd/usbd_core.h"
#include "usbd/usbd_hw.h"
#include "usbd/usbd_rom_api.h"
#include "usbd/usbd_hid.h"

// Kernel includes.
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"


// USB configuration
#define USB_MAX_IF_NUM      8
#define USB_MAX_EP_NUM      6
#define USB_MAX_PACKET0     64
#define HID_EP_IN           0x82
#define HID_EP_OUT          0x02


static USBD_HANDLE_T hUsb;
static uint8_t *HID_MouseReport;


static ErrorCode_t USB_Configure_Event(USBD_HANDLE_T hUsb);
static ErrorCode_t usb_hid_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size);
static uint32_t copy_descriptors(USB_CORE_DESCS_T* pDesc, uint32_t mem_base);
static uint32_t copy_hid_descriptors(uint8_t** pDesc, uint32_t* mem_base, uint32_t* mem_size);


void vUSBTask(void *pvParameters)
{
    ErrorCode_t ret;
    USBD_API_INIT_PARAM_T usb_param;
    USB_CORE_DESCS_T desc;
    USB_INTERFACE_DESCRIPTOR* pIntfDesc;

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


void USB0_IRQHandler(void)
{
    USBD_API->hw->ISR(hUsb);
}


// Set configuration callback
static ErrorCode_t USB_Configure_Event(USBD_HANDLE_T hUsb)
{
    USB_CORE_CTRL_T* pCtrl = (USB_CORE_CTRL_T*)hUsb;

    // Check if USB is configured
    if (pCtrl->config_value)
    {
        USBD_API->hw->WriteEP(hUsb, HID_EP_IN, HID_MouseReport, 3);
    }

    return LPC_OK;
}


// In endpoint callback
static ErrorCode_t HID_EpIn_Hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
{
    USB_HID_CTRL_T* pHidCtrl = (USB_HID_CTRL_T*)data;

    switch (event)
    {
    case USB_EVT_IN:
        USBD_API->hw->WriteEP(hUsb, pHidCtrl->epin_adr, HID_MouseReport, 3);
        break;
    }

    return LPC_OK;
}


// Get report callback
static ErrorCode_t HID_GetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* plength)
{
    // ReportID = SetupPacket.wValue.WB.L;
    switch (pSetup->wValue.WB.H)
    {
    case HID_REPORT_INPUT:
        *pBuffer[0] = HID_MouseReport[0];
        *pBuffer[1] = HID_MouseReport[1];
        *pBuffer[2] = HID_MouseReport[2];
        *plength = 3;
        break;
    case HID_REPORT_OUTPUT:
    case HID_REPORT_FEATURE:
        return (ERR_USBD_STALL);          /* Not Supported */
    }

    return (LPC_OK);
}


// Get report callback
// Not used
static ErrorCode_t HID_SetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length)
{
    return (LPC_OK);
}


// Configures HID
static ErrorCode_t usb_hid_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
{
    USBD_HID_INIT_PARAM_T hid_param;
    USB_HID_REPORT_T reports_data[1];
    ErrorCode_t ret = LPC_OK;

    memset((void*)&hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));

    // HID paramas
    hid_param.max_reports = 1;
    // Init reports_data
    reports_data[0].len = copy_hid_descriptors(&reports_data[0].desc, mem_base, mem_size);
    reports_data[0].idle_time = 0;

    if ((pIntfDesc == 0) || (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE))
        return ERR_FAILED;

    hid_param.mem_base = *mem_base;
    hid_param.mem_size = *mem_base;
    hid_param.intf_desc = (uint8_t*)pIntfDesc;
    // User defined functions
    hid_param.HID_GetReport = HID_GetReport;
    hid_param.HID_SetReport = HID_SetReport;
    hid_param.HID_EpIn_Hdlr = HID_EpIn_Hdlr;
    hid_param.report_data  = reports_data;

    ret = USBD_API->hid->init(hUsb, &hid_param);
    // allocate USB accessable memory space for report data */
    HID_MouseReport = (uint8_t*)hid_param.mem_base;
    HID_MouseReport[0] = 0; // buttons
    HID_MouseReport[1] = 0; // x
    HID_MouseReport[2] = 0; // y
    hid_param.mem_base += 4;
    hid_param.mem_size += 4;
    // update memory variables
    *mem_base = hid_param.mem_base;
    *mem_size = hid_param.mem_size;

    return ret;
}


const uint8_t HID_ReportDescriptor[] = {
/*
USAGE_PAGE (Generic Desktop)	05 01
USAGE (Mouse)	09 02
COLLECTION (Application)	A1 01
  USAGE (Pointer)	09 01
  COLLECTION (Physical)	A1 00
    USAGE_PAGE (Button)	05 09
    USAGE_MINIMUM (Button 1)	19 01
    USAGE_MAXIMUM (Button 3)	29 03
    LOGICAL_MINIMUM (0)	15 00
    LOGICAL_MAXIMUM (1)	25 01
    REPORT_COUNT (3)	95 03
    REPORT_SIZE (1)	75 01
    INPUT (Data,Var,Abs)	81 02
    REPORT_COUNT (1)	95 01
    REPORT_SIZE (5)	75 05
    INPUT (Cnst,Var,Abs)	81 03
    USAGE_PAGE (Generic Desktop)	05 01
    USAGE (X)	09 30
    USAGE (Y)	09 31
    LOGICAL_MINIMUM (-127)	15 81
    LOGICAL_MAXIMUM (127)	25 7F
    REPORT_SIZE (8)	75 08
    REPORT_COUNT (2)	95 02
    INPUT (Data,Var,Rel)	81 06
  END_COLLECTION	C0
END_COLLECTION	C0
*/
    0x05, 0x01,
    0x09, 0x02,
    0xA1, 0x01,
    0x09, 0x01,
    0xA1, 0x00,
    0x05, 0x09,
    0x19, 0x01,
    0x29, 0x03,
    0x15, 0x00,
    0x25, 0x01,
    0x95, 0x03,
    0x75, 0x01,
    0x81, 0x02,
    0x95, 0x01,
    0x75, 0x05,
    0x81, 0x03,
    0x05, 0x01,
    0x09, 0x30,
    0x09, 0x31,
    0x15, 0x81,
    0x25, 0x7F,
    0x75, 0x08,
    0x95, 0x02,
    0x81, 0x06,
    0xC0,
    0xC0,
};

#define HID_ReportDescSize (sizeof(HID_ReportDescriptor))


/* USB Standard Device Descriptor */
uint8_t USB_DeviceDescriptor[] =
{
    USB_DEVICE_DESC_SIZE,              /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
    0x00,                              /* bDeviceClass */
    0x00,                              /* bDeviceSubClass */
    0x00,                              /* bDeviceProtocol */
    USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
    WBVAL(0xA257),                     /* idVendor */
    WBVAL(0x3000),                     /* idProduct */
    WBVAL(0x0100), /* 1.00 */          /* bcdDevice */
    0x01,                              /* iManufacturer */
    0x02,                              /* iProduct */
    0x03,                              /* iSerialNumber */
    0x01                               /* bNumConfigurations */
};

/* USB FSConfiguration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
uint8_t USB_FsConfigDescriptor[] = {
    /* Configuration 1 */
    USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
    WBVAL(                             /* wTotalLength */
          1*USB_CONFIGUARTION_DESC_SIZE +
          1*USB_INTERFACE_DESC_SIZE     +
          HID_DESC_SIZE                 +
          1*USB_ENDPOINT_DESC_SIZE
         ),
    0x01,                              /* bNumInterfaces */
    0x01,                              /* bConfigurationValue */
    0x00,                              /* iConfiguration */
    USB_CONFIG_SELF_POWERED,           /* bmAttributes */
    USB_CONFIG_POWER_MA(100),          /* bMaxPower */

    /* Interface 0, Alternate Setting 0, HID Class */
    USB_INTERFACE_DESC_SIZE,           /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x00,                              /* bInterfaceNumber */
    0x00,                              /* bAlternateSetting */
    0x01,                              /* bNumEndpoints */
    USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
    HID_SUBCLASS_NONE,                 /* bInterfaceSubClass */
    HID_PROTOCOL_NONE,                 /* bInterfaceProtocol */
    0x04,                              /* iInterface */
    /* HID Class Descriptor */
    /* HID_DESC_OFFSET = 0x0012 */
    HID_DESC_SIZE,                     /* bLength */
    HID_HID_DESCRIPTOR_TYPE,           /* bDescriptorType */
    WBVAL(0x0100), /* 1.00 */          /* bcdHID */
    0x00,                              /* bCountryCode */
    0x01,                              /* bNumDescriptors */
    HID_REPORT_DESCRIPTOR_TYPE,        /* bDescriptorType */
    WBVAL(HID_ReportDescSize),       /* wDescriptorLength */
    /* Endpoint, HID Interrupt In */
    USB_ENDPOINT_DESC_SIZE,            /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
    HID_EP_IN,                /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
    WBVAL(0x0004),                     /* wMaxPacketSize */
    0x20,          /* 16ms */          /* bInterval */
    /* Terminator */
    0                                  /* bLength */
};

/* USB HSConfiguration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
uint8_t USB_HsConfigDescriptor[] = {
    /* Configuration 1 */
    USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
    WBVAL(                             /* wTotalLength */
          1*USB_CONFIGUARTION_DESC_SIZE +
          1*USB_INTERFACE_DESC_SIZE     +
          HID_DESC_SIZE                 +
          1*USB_ENDPOINT_DESC_SIZE
         ),
    0x01,                              /* bNumInterfaces */
    0x02,                              /* bConfigurationValue */
    0x00,                              /* iConfiguration */
    USB_CONFIG_SELF_POWERED /*|*/       /* bmAttributes */
        /*USB_CONFIG_REMOTE_WAKEUP*/,
    USB_CONFIG_POWER_MA(100),          /* bMaxPower */
    /* Interface 0, Alternate Setting 0, HID Class */
    USB_INTERFACE_DESC_SIZE,           /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x00,                              /* bInterfaceNumber */
    0x00,                              /* bAlternateSetting */
    0x01,                              /* bNumEndpoints */
    USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
    HID_SUBCLASS_NONE,                 /* bInterfaceSubClass */
    HID_PROTOCOL_NONE,                 /* bInterfaceProtocol */
    0x04,                              /* iInterface */
    /* HID Class Descriptor */
    /* HID_DESC_OFFSET = 0x0012 */
    HID_DESC_SIZE,                     /* bLength */
    HID_HID_DESCRIPTOR_TYPE,           /* bDescriptorType */
    WBVAL(0x0100), /* 1.00 */          /* bcdHID */
    0x00,                              /* bCountryCode */
    0x01,                              /* bNumDescriptors */
    HID_REPORT_DESCRIPTOR_TYPE,        /* bDescriptorType */
    WBVAL(HID_ReportDescSize),         /* wDescriptorLength */
    /* Endpoint, HID Interrupt In */
    USB_ENDPOINT_DESC_SIZE,            /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
    HID_EP_IN,                         /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
    WBVAL(0x0004),                     /* wMaxPacketSize */
    0x10,          /* 32ms */          /* bInterval */
    /* Terminator */
    0                                  /* bLength */
};

/* USB String Descriptor (optional) */
uint8_t USB_StringDescriptor[] =
{
    /* Index 0x00: LANGID Codes */
    0x04,                              /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    WBVAL(0x0409), /* US English */    /* wLANGID */
    /* Index 0x01: Manufacturer */
    (14*2 + 2),                        /* bLength (14 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'w', 0,
    'w', 0,
    'w', 0,
    '.', 0,
    'd', 0,
    'i', 0,
    'o', 0,
    'l', 0,
    'a', 0,
    'n', 0,
    '.', 0,
    'c', 0,
    'o', 0,
    'm', 0,
    /* Index 0x02: Product */
    (12*2 + 2),                        /* bLength (13 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'L', 0,
    'P', 0,
    'C', 0,
    '-', 0,
    '4', 0,
    '3', 0,
    '5', 0,
    '0', 0,
    '-', 0,
    'D', 0,
    'B', 0,
    '1', 0,
    /* Index 0x03: Serial Number */
    (16*2 + 2),                        /* bLength (16 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    '0', 0,
    '1', 0,
    '2', 0,
    '3', 0,
    '4', 0,
    '5', 0,
    '6', 0,
    '7', 0,
    '8', 0,
    '9', 0,
    'A', 0,
    'B', 0,
    'C', 0,
    'D', 0,
    'E', 0,
    'F', 0,
    /* Index 0x04: Interface 0, Alternate Setting 0 */
    (3*2 + 2),                        /* bLength (3 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,       /* bDescriptorType */
    'H', 0,
    'I', 0,
    'D', 0,
    /* Terminator */
    0                                  /* bLength */
};

/* USB Device Qualifier */
uint8_t USB_DeviceQualifier[] = {
    USB_DEVICE_QUALI_SIZE,              /* bLength */
    USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE, /* bDescriptorType */
    WBVAL(0x0200), /* 2.00 */          /* bcdUSB */
    0x00,                              /* bDeviceClass */
    0x00,                              /* bDeviceSubClass */
    0x00,                              /* bDeviceProtocol */
    USB_MAX_PACKET0,                   /* bMaxPacketSize0 */
    0x01,                              /* bNumOtherSpeedConfigurations */
    0x00                               /* bReserved */
};


static uint32_t copy_descriptors(USB_CORE_DESCS_T* pDesc, uint32_t mem_base)
{
    uint8_t* dest = (uint8_t*)mem_base;

    // Set all descriptors

    memcpy((void*)mem_base, USB_DeviceDescriptor, sizeof(USB_DeviceDescriptor));
    pDesc->device_desc = dest;
    dest += sizeof(USB_DeviceDescriptor);
    // Align to 4 byte boundary
    while ((uint32_t)dest & 0x03) dest++;

    memcpy(dest, USB_FsConfigDescriptor, sizeof(USB_FsConfigDescriptor));
    pDesc->full_speed_desc = dest;
    dest += sizeof(USB_FsConfigDescriptor);
    // Align to 4 byte boundary
    while ((uint32_t)dest & 0x03) dest++;

    memcpy(dest, USB_HsConfigDescriptor, sizeof(USB_HsConfigDescriptor));
    pDesc->high_speed_desc = dest;
    dest += sizeof(USB_HsConfigDescriptor);
    // Align to 4 byte boundary
    while ((uint32_t)dest & 0x03) dest++;

    memcpy(dest, USB_StringDescriptor, sizeof(USB_StringDescriptor));
    pDesc->string_desc = dest;
    dest += sizeof(USB_StringDescriptor);
    // Align to 4 byte boundary
    while ((uint32_t)dest & 0x03) dest++;

    memcpy(dest, USB_DeviceQualifier, sizeof(USB_DeviceQualifier));
    pDesc->device_qualifier = dest;
    dest += sizeof(USB_DeviceQualifier);
    // Align to 4 byte boundary
    while ((uint32_t)dest & 0x03) dest++;

    return (uint32_t)dest;
}


static uint32_t copy_hid_descriptors(uint8_t** pDesc, uint32_t* mem_base, uint32_t* mem_size)
{
    *pDesc = (uint8_t*)(*mem_base);
    memcpy(*pDesc, HID_ReportDescriptor, sizeof(HID_ReportDescriptor));
    *mem_base += sizeof(HID_ReportDescriptor);
    *mem_size -= sizeof(HID_ReportDescriptor);

    // align to 4 byte boundary
    while (*mem_base & 0x03)
    {
        *mem_base = *mem_base + 1;
        *mem_size = *mem_size - 1;
    }

    return  sizeof(HID_ReportDescriptor);
}
