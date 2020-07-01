/*
 * Abstraction.h
 *
 *  Created on: 26 Feb 2020
 *      Author: Yahia Yasser
 */

#ifndef ABSTRACTION_H_
#define ABSTRACTION_H_

#define EraseComplete	FLASH_COMPLETE

#define Bootloader_HW_Init()	{ \
	RCC_HSICmd(ENABLE);\
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){};\
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);\
	FLASH_SetLatency(FLASH_Latency_0);\
}

#define Bootloader_FlashUnlock()	FLASH_Unlock()

#define Bootloader_FlashClrFlags()	{\
	FLASH_ClearFlag(FLASH_FLAG_PGERR);\
	FLASH_ClearFlag(FLASH_FLAG_WRPRTERR);\
	FLASH_ClearFlag(FLASH_FLAG_EOP);\
}

#define Bootloader_FlashLock()	FLASH_Lock()

#define Bootloader_ErasePage(PageNo)	FLASH_ErasePage((PageNo * FlashPageSize) + FlashBase)


#endif

