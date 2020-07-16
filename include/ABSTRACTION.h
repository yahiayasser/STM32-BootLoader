/*
 * Abstraction.h
 *
 *  Created on: 26 Feb 2020
 *      Author: Yahia Yasser
 */

#ifndef ABSTRACTION_H_
#define ABSTRACTION_H_

#define EraseComplete	FLASH_COMPLETE
#define WriteComplete	FLASH_COMPLETE

#define Bootloader_HW_Init()	{ \
	RCC_HSICmd(ENABLE);\
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){};\
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);\
	FLASH_SetLatency(FLASH_Latency_0);\
}

#define FlashLock()	FLASH_Lock()
#define FlashUnlock()	FLASH_Unlock()

#define FlashClrFlags()	{\
	FLASH_ClearFlag(FLASH_FLAG_PGERR);\
	FLASH_ClearFlag(FLASH_FLAG_WRPRTERR);\
	FLASH_ClearFlag(FLASH_FLAG_EOP);\
}

#define PageErase(PageNo)	FLASH_ErasePage((PageNo * FlashPageSize) + FlashBase)

#define FlashErase()	FLASH_EraseAllPages()

#define FLASH_WriteHalfWord(Add, data)	FLASH_ProgramHalfWord(Add, (uint16)data)
#define FLASH_WriteWord(Add, data)		FLASH_ProgramWord(Add, (uint32)data)

#define Bootloader_SetStartFlag()	FLASH_ProgramOptionByteData((uint32)Bootloader_StartFlagAddress, FlagValue)
#define Bootloader_GetStartFlag()	((uint8)((FLASH_GetUserOptionByte() >> 8) && 0x000000FF))
#define Bootloader_ClearStartFlag()	FLASH_ProgramOptionByteData((uint32)Bootloader_StartFlagAddress, ClearFlagValue)


#endif

