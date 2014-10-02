;/*****************************************************************************
; * @file:    startup_LPC18xx_43xx_m0a_IAR.s
; * @purpose: CMSIS Cortex-M3/M4 Core Device Startup File
; *           for the NXP LPC18xx/43xx Device Series
; * @version: V0.01
; * @date:    20. September 2011
; *----------------------------------------------------------------------------
; *
; * Copyright (C) 2010 ARM Limited. All rights reserved.
; *
; * ARM Limited (ARM) is supplying this software for use with Cortex-Mx
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA


__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     0
        DCD     0
        DCD     0
__vector_table_0x1c
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     0
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
        DCD		DAC_IRQHandler	 			; 16 D/A Converter
	DCD		M3_M4_IRQHandler			; 17 M3/M4
	DCD		DMA_IRQHandler				; 18 General Purpose DMA
	DCD		EZH_IRQHandler				; 19 EZH/EDM
	DCD		SGPIO_INPUT_IRQHandler		; 20 SGPIO INPUT - only in LPC43xx
	DCD		SGPIO_MATCH_IRQHandler		; 21 SGPIO MATCH - only in LPC43xx
	DCD		SGPIO_SHIFT_IRQHandler		; 22 SGPIO SHIFT - only in LPC43xx
	DCD		SGPIO_POS_IRQHandler			; 23 SGPIO POS
	DCD		USB0_IRQHandler				; 24 USB0
	DCD		USB1_IRQHandler				; 25 USB1
	DCD		SCT_IRQHandler				; 26 State Configurable Timer
	DCD		RIT_IRQHandler				; 27 Repetitive Interrupt Timer
	DCD		GINT0_IRQHandler			; 28 GINT0
	DCD		TIMER1_IRQHandler			; 29 Timer1
	DCD		TIMER2_IRQHandler			; 30 Timer2
	DCD		GPIO5_IRQHandler			; 31 GPIO5
	DCD		MCPWM_IRQHandler			; 32 Motor Control PWM
	DCD		ADC0_IRQHandler				; 33 A/D Converter 0
	DCD		I2C0_IRQHandler				; 34 I2C0
	DCD		I2C1_IRQHandler				; 35 I2C1
	DCD		SPI_IRQHandler				; 36 SPI
	DCD		ADC1_IRQHandler				; 37 A/D Converter 1
	DCD		SSP0_SSP1_IRQHandler			; 38 SSP0 and SSP1 or'ed
	DCD		EVRT_IRQHandler				; 39 Event Router
	DCD		UART0_IRQHandler			; 40 UART0
	DCD		UART1_IRQHandler			; 41 UART1
	DCD		UART2_CAN1_IRQHandler		; 42 UART2 and C_CAN1 or'ed
	DCD		UART3_IRQHandler			; 43 UART3
	DCD		I2S0_I2S1_QEI_IRQHandler		; 44 I2S0, I2s1 and QEI or'ed
	DCD		CAN0_IRQHandler				; 45 C_CAN0
	DCD		SPIFI_VADC_IRQHandler		; 46 SPIFI and VADC or'ed
	DCD		M0s_IRQHandler				; 47 M0s
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size 	EQU 	__Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK DAC_IRQHandler
        SECTION .text:CODE:REORDER(1)
DAC_IRQHandler
        B DAC_IRQHandler

        PUBWEAK M3_M4_IRQHandler
        SECTION .text:CODE:REORDER(1)
M3_M4_IRQHandler
        B M3_M4_IRQHandler

        PUBWEAK DMA_IRQHandler
        SECTION .text:CODE:REORDER(1)
DMA_IRQHandler
        B DMA_IRQHandler

	PUBWEAK EZH_IRQHandler				; 19 EZH/EDM
	SECTION .text:CODE:REORDER(1)
EZH_IRQHandler
	B EZH_IRQHandler
	
	PUBWEAK SGPIO_INPUT_IRQHandler		; 20 SGPIO INPUT - only in LPC43xx
	SECTION .text:CODE:REORDER(1)
SGPIO_INPUT_IRQHandler
	B SGPIO_INPUT_IRQHandler
	
	PUBWEAK SGPIO_MATCH_IRQHandler		; 21 SGPIO MATCH - only in LPC43xx
	SECTION .text:CODE:REORDER(1)
SGPIO_MATCH_IRQHandler
	B SGPIO_MATCH_IRQHandler
	
	PUBWEAK SGPIO_SHIFT_IRQHandler		; 22 SGPIO SHIFT - only in LPC43xx
	SECTION .text:CODE:REORDER(1)
SGPIO_SHIFT_IRQHandler
	B SGPIO_SHIFT_IRQHandler
	
	PUBWEAK SGPIO_POS_IRQHandler			; 23 SGPIO POS
	SECTION .text:CODE:REORDER(1)
SGPIO_POS_IRQHandler
	B SGPIO_POS_IRQHandler
	
	PUBWEAK USB0_IRQHandler				; 24 USB0
	SECTION .text:CODE:REORDER(1)
USB0_IRQHandler
	B USB0_IRQHandler
	
	PUBWEAK USB1_IRQHandler				; 25 USB1
	SECTION .text:CODE:REORDER(1)
USB1_IRQHandler
	B USB1_IRQHandler
	
	PUBWEAK SCT_IRQHandler				; 26 State Configurable Timer
	SECTION .text:CODE:REORDER(1)
SCT_IRQHandler
	B SCT_IRQHandler
	
	PUBWEAK RIT_IRQHandler				; 27 Repetitive Interrupt Timer
	SECTION .text:CODE:REORDER(1)
RIT_IRQHandler
	B RIT_IRQHandler
	
	PUBWEAK GINT0_IRQHandler			; 28 GINT0
	SECTION .text:CODE:REORDER(1)
GINT0_IRQHandler
	B GINT0_IRQHandler
	
	PUBWEAK TIMER1_IRQHandler			; 29 Timer1
	SECTION .text:CODE:REORDER(1)
TIMER1_IRQHandler
	B TIMER1_IRQHandler
	
	PUBWEAK TIMER2_IRQHandler			; 30 Timer2
	SECTION .text:CODE:REORDER(1)
TIMER2_IRQHandler
	B TIMER2_IRQHandler
	
	PUBWEAK GPIO5_IRQHandler			; 31 GPIO5
	SECTION .text:CODE:REORDER(1)
GPIO5_IRQHandler
	B GPIO5_IRQHandler
	
	PUBWEAK MCPWM_IRQHandler			; 32 Motor Control PWM
	SECTION .text:CODE:REORDER(1)
MCPWM_IRQHandler
    B MCPWM_IRQHandler

	PUBWEAK ADC0_IRQHandler				; 33 A/D Converter 0
	SECTION .text:CODE:REORDER(1)
ADC0_IRQHandler
	B ADC0_IRQHandler
	
	PUBWEAK I2C0_IRQHandler				; 34 I2C0
	SECTION .text:CODE:REORDER(1)
I2C0_IRQHandler
	B I2C0_IRQHandler
	
	PUBWEAK I2C1_IRQHandler				; 35 I2C1
	SECTION .text:CODE:REORDER(1)
I2C1_IRQHandler
	B I2C1_IRQHandler
	
	PUBWEAK SPI_IRQHandler				; 36 SPI
	SECTION .text:CODE:REORDER(1)
SPI_IRQHandler
	B SPI_IRQHandler
	
	PUBWEAK ADC1_IRQHandler				; 37 A/D Converter 1
	SECTION .text:CODE:REORDER(1)
ADC1_IRQHandler
	B ADC1_IRQHandler
	
	PUBWEAK SSP0_SSP1_IRQHandler			; 38 SSP0 and SSP1 or'ed
	SECTION .text:CODE:REORDER(1)
SSP0_SSP1_IRQHandler
	B SSP0_SSP1_IRQHandler
	
	PUBWEAK EVRT_IRQHandler				; 39 Event Router
	SECTION .text:CODE:REORDER(1)
EVRT_IRQHandler
	B EVRT_IRQHandler
	
	PUBWEAK UART0_IRQHandler			; 40 UART0
	SECTION .text:CODE:REORDER(1)
UART0_IRQHandler
	B UART0_IRQHandler
	
	PUBWEAK UART1_IRQHandler			; 41 UART1
	SECTION .text:CODE:REORDER(1)
UART1_IRQHandler
	B UART1_IRQHandler
	
	PUBWEAK UART2_CAN1_IRQHandler		; 42 UART2 and C_CAN1 or'ed
	SECTION .text:CODE:REORDER(1)
UART2_CAN1_IRQHandler
	B UART2_CAN1_IRQHandler
	
	PUBWEAK UART3_IRQHandler			; 43 UART3
	SECTION .text:CODE:REORDER(1)
UART3_IRQHandler
	B UART3_IRQHandler
	
	PUBWEAK I2S0_I2S1_QEI_IRQHandler		; 44 I2S0, I2s1 and QEI or'ed
	SECTION .text:CODE:REORDER(1)
I2S0_I2S1_QEI_IRQHandler
	B I2S0_I2S1_QEI_IRQHandler
	
	PUBWEAK CAN0_IRQHandler				; 45 C_CAN0
	SECTION .text:CODE:REORDER(1)
CAN0_IRQHandler
	B CAN0_IRQHandler
	
	PUBWEAK SPIFI_VADC_IRQHandler		; 46 SPIFI and VADC or'ed
	SECTION .text:CODE:REORDER(1)
SPIFI_VADC_IRQHandler
    B SPIFI_VADC_IRQHandler

	PUBWEAK M0s_IRQHandler				; 47 M0s
	SECTION .text:CODE:REORDER(1)
M0s_IRQHandler
	B M0s_IRQHandler
	


        END
