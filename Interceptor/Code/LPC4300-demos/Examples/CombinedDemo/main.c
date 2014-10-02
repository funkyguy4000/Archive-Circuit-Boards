/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "Combined demo" example.
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



#define extern
#include "easyweb/easyweb.h"
#include "easyweb/emac.h"
#include "easyweb/tcpip.h"

#include "webpage.h"

static volatile uint32_t TimeTick;
static void HTTPServer(void);

static volatile uint8_t blinkOn;


// SysTick interrupt happens every 10 ms
void SysTick_Handler (void)
{
    TimeTick++;
    if (TimeTick >= 20)
    {
        TimeTick = 0;
        TCPClockHandler();

        LPC_GPIO_PORT->NOT[D2_GPIO_PORT] = D2_GPIO_MASK;

        if (blinkOn)
        {
            LPC_GPIO_PORT->NOT[D3_GPIO_PORT] = D3_GPIO_MASK;
            LPC_GPIO_PORT->NOT[D4_GPIO_PORT] = D4_GPIO_MASK;
            LPC_GPIO_PORT->NOT[D5_GPIO_PORT] = D5_GPIO_MASK;
            LPC_GPIO_PORT->NOT[D6_GPIO_PORT] = D6_GPIO_MASK;
        }
    }
}


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

    blinkOn = 0;

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

    CGU_EnableEntity(CGU_CLKSRC_ENET_RX_CLK, ENABLE);
    CGU_EnableEntity(CGU_CLKSRC_ENET_TX_CLK, ENABLE);
    CGU_EntityConnect(CGU_CLKSRC_ENET_TX_CLK, CGU_BASE_PHY_TX);
    CGU_EntityConnect(CGU_CLKSRC_ENET_RX_CLK, CGU_BASE_PHY_RX);


    // Configure buttons
    scu_pinmux(S1_SCU_PORT, S1_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S2_SCU_PORT, S2_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S3_SCU_PORT, S3_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    scu_pinmux(S4_SCU_PORT, S4_SCU_PIN, MD_BUK | MD_EZI, FUNC4);
    GPIO_SetDir(S1_GPIO_PORT, S1_GPIO_MASK, 0);
    GPIO_SetDir(S2_GPIO_PORT, S2_GPIO_MASK, 0);
    GPIO_SetDir(S3_GPIO_PORT, S3_GPIO_MASK, 0);
    GPIO_SetDir(S4_GPIO_PORT, S4_GPIO_MASK, 0);
    // Configure LEDs
    scu_pinmux(D2_SCU_PORT, D2_SCU_PIN, MD_BUK, FUNC0);
    scu_pinmux(D3_SCU_PORT, D3_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D4_SCU_PORT, D4_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D5_SCU_PORT, D5_SCU_PIN, MD_BUK, FUNC4);
    scu_pinmux(D6_SCU_PORT, D6_SCU_PIN, MD_BUK, FUNC4);
    GPIO_SetDir(D2_GPIO_PORT, D2_GPIO_MASK, 1);
    GPIO_SetDir(D3_GPIO_PORT, D3_GPIO_MASK, 1);
    GPIO_SetDir(D4_GPIO_PORT, D4_GPIO_MASK, 1);
    GPIO_SetDir(D5_GPIO_PORT, D5_GPIO_MASK, 1);
    GPIO_SetDir(D6_GPIO_PORT, D6_GPIO_MASK, 1);
    GPIO_ClearValue(D2_GPIO_PORT, D2_GPIO_MASK);
    GPIO_ClearValue(D3_GPIO_PORT, D3_GPIO_MASK);
    GPIO_ClearValue(D4_GPIO_PORT, D4_GPIO_MASK);
    GPIO_ClearValue(D5_GPIO_PORT, D5_GPIO_MASK);
    GPIO_ClearValue(D6_GPIO_PORT, D6_GPIO_MASK);


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
            GPIO_SetValue(D4_GPIO_PORT, D4_GPIO_MASK);  // for testing
            while (1);  // catch error
        }
    }
    else
    {
        GPIO_SetValue(D3_GPIO_PORT, D3_GPIO_MASK);      // for testing
        while (1);  // catch error
    }


    // Generate interrupt every 10 ms
    SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE) / 100);


    // Configure Ethernet
    GPIO_SetValue(D6_GPIO_PORT, D6_GPIO_MASK);          // for testing
    TCPLowLevelInit();
    // clear HTTP-server's flag register
    HTTPStatus = 0;
    // set port we want to listen to
    TCPLocalPort = TCP_PORT_HTTP;


    GPIO_ClearValue(D6_GPIO_PORT, D6_GPIO_MASK);
    blinkOn = 1;


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


        // listen for incoming TCP-connection
        if (!(SocketStatus & SOCK_ACTIVE)) TCPPassiveOpen();
        // handle network and easyWEB-stack
        DoNetworkStuff();
        // events
        HTTPServer();
    }
}


// This function implements a very simple dynamic HTTP-server.
// It waits until connected, then sends a HTTP-header and the
// HTML-code stored in memory. Before sending, it replaces
// some special strings with dynamic values.
// NOTE: For strings crossing page boundaries, replacing will
// not work. In this case, simply add some extra lines
// (e.g. CR and LFs) to the HTML-code.

static void HTTPServer(void)
{
    if (SocketStatus & SOCK_CONNECTED)             // check if somebody has connected to our TCP
    {
        if (SocketStatus & SOCK_DATA_AVAILABLE)      // check if remote TCP sent data
        {
            TCPReleaseRxBuffer();                      // and throw it away
        }

        if (SocketStatus & SOCK_TX_BUF_RELEASED)     // check if buffer is free for TX
        {
            if (!(HTTPStatus & HTTP_SEND_PAGE))        // init byte-counter and pointer to webside
            {                                          // if called the 1st time
                HTTPBytesToSend = sizeof(WebPage) - 1;   // get HTML length, ignore trailing zero
                PWebPage = (unsigned char *)WebPage;     // pointer to HTML-code
            }

            if (HTTPBytesToSend > MAX_TCP_TX_DATA_SIZE)     // transmit a segment of MAX_SIZE
            {
                if (!(HTTPStatus & HTTP_SEND_PAGE))           // 1st time, include HTTP-header
                {
                    memcpy(TCP_TX_BUF, GetResponse, sizeof(GetResponse) - 1);
                    memcpy(TCP_TX_BUF + sizeof(GetResponse) - 1, PWebPage, MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1);
                    HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
                    PWebPage += MAX_TCP_TX_DATA_SIZE - sizeof(GetResponse) + 1;
                }
                else
                {
                    memcpy(TCP_TX_BUF, PWebPage, MAX_TCP_TX_DATA_SIZE);
                    HTTPBytesToSend -= MAX_TCP_TX_DATA_SIZE;
                    PWebPage += MAX_TCP_TX_DATA_SIZE;
                }

                TCPTxDataCount = MAX_TCP_TX_DATA_SIZE;   // bytes to xfer
                TCPTransmitTxBuffer();                   // xfer buffer
            }
            else if (HTTPBytesToSend)                  // transmit leftover bytes
            {
                memcpy(TCP_TX_BUF, PWebPage, HTTPBytesToSend);
                TCPTxDataCount = HTTPBytesToSend;        // bytes to xfer
                TCPTransmitTxBuffer();                   // send last segment
                TCPClose();                              // and close connection
                HTTPBytesToSend = 0;                     // all data sent
            }

            HTTPStatus |= HTTP_SEND_PAGE;              // ok, 1st loop executed
        }
    }
    else
        HTTPStatus &= ~HTTP_SEND_PAGE;               // reset help-flag if not connected
}
