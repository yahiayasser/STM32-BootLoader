/*
 * Mem_ABSTRACTION.h
 *
 *  Created on: 26 Feb 2020
 *      Author: Yahia Yasser
 */

#ifndef MEM_ABSTRACTION_H_
#define MEM_ABSTRACTION_H_

#define EraseComplete	FLASH_COMPLETE
#define WriteComplete	FLASH_COMPLETE


#define FlashLock()		FLASH_Lock()
#define FlashUnlock()	{\
	FLASH_Unlock();\
}

#define FlashClrFlags()	{\
	FLASH_ClearFlag(FLASH_FLAG_PGERR);\
	FLASH_ClearFlag(FLASH_FLAG_WRPRTERR);\
	FLASH_ClearFlag(FLASH_FLAG_EOP);\
}

#define PageErase(PageNo)	FLASH_ErasePage((PageNo * FlashPageSize) | FlashStartAddress)

#define FlashErase()	FLASH_EraseAllPages()

#define FLASH_WriteHalfWord(Add, data)	FLASH_ProgramHalfWord(Add, (uint16)data)
#define FLASH_WriteWord(Add, data)		FLASH_ProgramWord(Add, (uint32)data)

#define SetNewVectorTable()		NVIC_SetVectorTable(NVIC_VectTab_FLASH, Application_Add);

#endif

