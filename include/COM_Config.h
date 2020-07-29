/*
 * UART_Config.h
 *
 *  Created on: 29 Jun 2020
 *      Author: Yahia
 */

#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_


#include "Platform_Types.h"
#include "uC_Dependencies.h"


#if(STM32f103 == BOOTLOADER_TARGET)

#if(Bootloader_UART == BOOTLOADER_CommProtocol)
/*************** Select the USART or the UART peripheral *********************/
#ifndef	UART_1
#define UART_1	(1U)
#endif
#ifndef	UART_2
#define UART_2	(2U)
#endif
#ifndef	UART_3
#define UART_3	(3U)
#endif
#ifndef	UART_4
#define UART_4	(4U)
#endif
#ifndef	UART_5
#define UART_5	(5U)
#endif
#define Bootloader_UART_ID	UART_1
/*****************************************************************************/

/*************** Select the USART or the UART Baud rate  *********************/
#define	UART_1200		(1200U)
#define	UART_2400		(2400U)
#define	UART_4800		(4800U)
#define	UART_9600		(9600U)
#define	UART_19200		(19200U)
#define	UART_38400		(38400U)
#define	UART_57600		(57600U)
#define	UART_115200		(115200U)

#define Bootloader_UART_Rate	UART_115200
/*****************************************************************************/

void Bootloader_UARTInit(void);

#endif

void COM_InterruptSetter(pFunction ptr);

#endif

#endif
