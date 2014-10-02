/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Main code for "Buttons Leds Simple" example.
*
* Copyryght by Diolan Ltd. All rights reserved.
*
*******************************************************************************/
#include <string.h>

#include "LPC43xx.h"
#include "lpc4350_db1.h"
#include "lpc43xx_cgu.h"
#include "lpc4350_db1_emc.h"

#define extern
#include "easyweb/easyweb.h"
#include "easyweb/emac.h"
#include "easyweb/tcpip.h"

#include "webpage.h"

volatile uint32_t TimeTick  = 0;
// SysTick interrupt happens every 10 ms
void SysTick_Handler (void)
{
    TimeTick++;
    if (TimeTick >= 20)
    {
        TimeTick = 0;
        TCPClockHandler();
    }
}


// This function implements a very simple dynamic HTTP-server.
// It waits until connected, then sends a HTTP-header and the
// HTML-code stored in memory. Before sending, it replaces
// some special strings with dynamic values.
// NOTE: For strings crossing page boundaries, replacing will
// not work. In this case, simply add some extra lines
// (e.g. CR and LFs) to the HTML-code.

void HTTPServer(void)
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


int main(void)
{
    SystemInit();

    // External flash note:
    // This demo can be larger than 16k (bootloader initializes only A[13..0])
    // So initialize memory before clock setup
    MemoryPinInit();
    EMCFlashInit();

    CGU_Init();
    // Enable 32 kHz & 1 kHz on osc32k
    CGU_EnableEntity(CGU_CLKSRC_32KHZ_OSC, ENABLE);			

    CGU_EnableEntity(CGU_CLKSRC_ENET_RX_CLK, ENABLE);
    CGU_EnableEntity(CGU_CLKSRC_ENET_TX_CLK, ENABLE);

    CGU_EntityConnect(CGU_CLKSRC_ENET_TX_CLK, CGU_BASE_PHY_TX);
    CGU_EntityConnect(CGU_CLKSRC_ENET_RX_CLK, CGU_BASE_PHY_RX);

    /* Generate interrupt every 10 ms */
    SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/100);


    TCPLowLevelInit();

    // clear HTTP-server's flag register
    HTTPStatus = 0;
    // set port we want to listen to
    TCPLocalPort = TCP_PORT_HTTP;

    // repeat forever
    while (1)
    {
        // listen for incoming TCP-connection
        if (!(SocketStatus & SOCK_ACTIVE)) TCPPassiveOpen();
        // handle network and easyWEB-stack
        DoNetworkStuff();
        // events
        HTTPServer();
    }

}
