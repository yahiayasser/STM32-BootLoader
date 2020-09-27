/*
 * Abstraction.h
 *
 *  Created on: 26 Feb 2020
 *      Author: Yahia Yasser
 */

#ifndef ABSTRACTION_H_
#define ABSTRACTION_H_


#include "Mem_ABSTRACTION.h"

#define FLASH_OK	WriteComplete

#define Bootloader_UnlockFlash()	UnlockFlash()
#define Bootloader_LockFlash()		LockFlash()

#define Bootloader_HW_Init()	{ \
	RCC_HSICmd(ENABLE);\
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){};\
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);\
	FLASH_SetLatency(FLASH_Latency_0);\
}

#define SetNewVectorTable()		NVIC_SetVectorTable(NVIC_VectTab_FLASH, Application_Add);

#define BOOT_MAIN	Bootloader_Main


#endif

