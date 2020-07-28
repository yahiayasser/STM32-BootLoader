/*
 * PBcfg.c
 *
 *  Created on: 28 Jul 2020
 *      Author: Yahia
 */


#include "UART_Config.h"

USART_InitTypeDef BootloaderUART =
{
		Bootloader_UART_Rate,
		USART_WordLength_8b,
		USART_StopBits_1,
		USART_Parity_No,
		USART_Mode_Rx,
		USART_HardwareFlowControl_None
};
