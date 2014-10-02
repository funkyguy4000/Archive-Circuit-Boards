/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Inter-process communication protocol for "IPC_Blinking" demo project.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/


#ifndef __IPC_PROTOCOL_H__
#define __IPC_PROTOCOL_H__


// CM0 memory loactions
#define CM0_RAM_BASE        0x10080000
#define CM0_RAM_SIZE        (40 * 1024)


// Use 32k-region at 0x1008A000 address
// WARNING: LPC4350/30 only
//#define CMD_BASE_ADDR       0x1008A000
//#define MSG_BASE_ADDR       0x1008E000
#define CMD_BASE_ADDR       0x20000000
#define MSG_BASE_ADDR       0x20008000

// Command return status
#define STATUS_CMD_OK       0x00
#define STATUS_CMD_UNKNOWN  0x80

// Command codes
#define CMD_SET_LED         0x01

// Message codes
#define MSG_INIT_FINISH     0x81


#endif //__IPC_PROTOCOL_H__
