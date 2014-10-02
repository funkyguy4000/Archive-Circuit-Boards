/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: HID functions.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include <string.h>
#include "usbd/usbd_hid.h"
#include "usbd/usbd_rom_api.h"
#include "usbd/usbd_hiduser.h"
#include "config.h"


uint8_t *HID_MouseReport;

extern uint32_t copy_hid_descriptors(uint8_t** pDesc, uint32_t* mem_base, uint32_t* mem_size);


// Set configuration callback
ErrorCode_t USB_Configure_Event(USBD_HANDLE_T hUsb)
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
ErrorCode_t HID_EpIn_Hdlr(USBD_HANDLE_T hUsb, void* data, uint32_t event)
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
ErrorCode_t HID_GetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t* plength)
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
ErrorCode_t HID_SetReport( USBD_HANDLE_T hHid, USB_SETUP_PACKET* pSetup, uint8_t** pBuffer, uint16_t length)
{
    return (LPC_OK);
}


// Configures HID
ErrorCode_t usb_hid_init(USBD_HANDLE_T hUsb, USB_INTERFACE_DESCRIPTOR* pIntfDesc, uint32_t* mem_base, uint32_t* mem_size)
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
