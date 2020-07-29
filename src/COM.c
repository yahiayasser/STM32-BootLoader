/*
 * COM.c
 *
 *  Created on: Jul 29, 2020
 *      Author: Yahia
 */


#include "COM_Config.h"
#include "Bootloader.h"


void COMInit(void)
{
#if(Bootloader_UART == BOOTLOADER_CommProtocol)
	Bootloader_UARTInit();
#elif(Bootloader_CAN == BOOTLOADER_CommProtocol)

#elif(Bootloader_USB == BOOTLOADER_CommProtocol)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif
}

void COMSend(uint16 SizeOfData, void* pData)
{

#if(Bootloader_UART == BOOTLOADER_CommProtocol)

	uint8* ptr = (uint8*) pData;
	uint16 Count = 0;

	for(; Count < SizeOfData; Count++)
	{
		USART_SendData(USART1, (uint16)ptr[Count]);
	}

#elif(Bootloader_CAN == BOOTLOADER_CommProtocol)

#elif(Bootloader_USB == BOOTLOADER_CommProtocol)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

}

void COMReceive(uint16 SizeOfData, void* pData)
{

#if(Bootloader_UART == BOOTLOADER_CommProtocol)

	uint8* ptr = (uint8*) pData;
	uint16 Count = 0;

	for(; Count < SizeOfData; Count++)
	{
		ptr[Count] = (uint8)USART_ReceiveData(USART1);
	}

#elif(Bootloader_CAN == BOOTLOADER_CommProtocol)

#elif(Bootloader_USB == BOOTLOADER_CommProtocol)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

}


#if(Bootloader_UART == BOOTLOADER_CommProtocol)

extern USART_InitTypeDef BootloaderUART;
pFunction UART_Handler = stub;

void Bootloader_UARTInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

#if(UART_1 == Bootloader_UART_ID)

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	/* GPIOA PIN9 alternative function Tx */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* GPIOA PIN9 alternative function Rx */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);

    /* Configure USART1 */
    USART_Init(USART1, &BootloaderUART);

#if(Bootloader_Interrupt == BOOTLOADER_Handle)
    /* Enable RXNE interrupt */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 global interrupt */
    NVIC_EnableIRQ(USART1_IRQn);
#endif

#elif(UART_2 == Bootloader_UART_ID)

#elif(UART_3 == Bootloader_UART_ID)

#elif(UART_4 == Bootloader_UART_ID)

#elif(UART_5 == Bootloader_UART_ID)

#else
#error "Invalid value of Bootloader_UART_ID"
#endif
}

void USART1_IRQHandler(void)
{
	UART_Handler();
}


#elif(Bootloader_CAN == BOOTLOADER_CommProtocol)

#elif(Bootloader_USB == BOOTLOADER_CommProtocol)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif



void COM_InterruptSetter(pFunction ptr)
{
#if(Bootloader_UART == BOOTLOADER_CommProtocol)
	UART_Handler = ptr;

#elif(Bootloader_CAN == BOOTLOADER_CommProtocol)

#elif(Bootloader_USB == BOOTLOADER_CommProtocol)
#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif
}



