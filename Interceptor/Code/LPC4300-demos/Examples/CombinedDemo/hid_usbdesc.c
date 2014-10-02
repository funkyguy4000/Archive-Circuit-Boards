/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: HID descriptors.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/


#include <string.h>
#include "usbd/usbd.h"
#include "usbd/usbd_core.h"
#include "usbd/usbd_hid.h"
#include "config.h"


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


uint32_t copy_descriptors(USB_CORE_DESCS_T* pDesc, uint32_t mem_base)
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


uint32_t copy_hid_descriptors(uint8_t** pDesc, uint32_t* mem_base, uint32_t* mem_size)
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


