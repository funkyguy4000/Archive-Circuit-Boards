/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Easyweb task code for "FreeRTOS" example.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#include <string.h>
#include "LPC43xx.h"

// Kernel includes.
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "webpage.h"
#include "easyweb/easyweb.h"
#include "easyweb/emac.h"
#define extern
#include "easyweb/tcpip.h"
#undef extern


// Forward declarations
static void vTimerCallback(xTimerHandle xTimer);
static void HTTPServer(void);

// Variables
static xTimerHandle tcpTimer;


void vEasywebTask(void *pvParameters)
{
    (void)pvParameters;

    // Initialize
    TCPLowLevelInit();

    // Create timer to call TCPClockHandler() every 210 ms
    tcpTimer = xTimerCreate("TCP_TIMER", 210 / portTICK_RATE_MS, pdTRUE, NULL, vTimerCallback);
    if (tcpTimer != NULL)
    {
        xTimerStart(tcpTimer, 0);
    }

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


static void vTimerCallback(xTimerHandle xTimer)
{
    TCPClockHandler();
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
