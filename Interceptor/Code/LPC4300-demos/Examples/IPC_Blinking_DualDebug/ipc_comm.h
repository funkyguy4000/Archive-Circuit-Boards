/*******************************************************************************
*
* Project: LPC4350-DB1 Development Board Examples
*
* Description: Inter-process communication protocol.
*
* Copyright by Diolan Ltd. All rights reserved.
*
*******************************************************************************/

#ifndef __IPC_COMM_H
#define __IPC_COMM_H

#define CMD_M4      (*(volatile unsigned int *)0x20000000)
#define RSP_M0      (*(volatile unsigned int *)0x20008000)

// Commands
#define CMD_SET_LED2    0x01000002
#define CMD_SET_LED3    0x01000003
#define CMD_SET_LED4    0x01000004
#define CMD_SET_LED5    0x01000005
#define CMD_SET_LED6    0x01000006

// Responses
#define RSP_DONE        0x00000001
#define RSP_ERROR       0x80000000

#endif // __IPC_COMM_H
