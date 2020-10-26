/*
 * MemIf.c
 *
 *  Created on: Aug 25, 2020
 *      Author: Yahia
 */

#include "MemIf.h"


static boolean FlashIsAlreadyUnocked_Flag = FALSE;
static uint8 FlashDontLock_Count = 0;


void UnlockFlash(void)
{
	if(False == FlashIsAlreadyUnocked_Flag)
	{

		FlashUnlock();
		FlashIsAlreadyUnocked_Flag = TRUE;
	}
	else
	{
		FlashDontLock_Count++;
	}
}

void LockFlash(void)
{
	if(FlashDontLock_Count)
	{
		FlashDontLock_Count--;
	}
	else
	{
		FlashLock();
		FlashIsAlreadyUnocked_Flag = FALSE;
	}
}

void Mem_init(void)
{
	UnlockFlash();
	FlashClrFlags();
	LockFlash();
}

Std_ReturnType Mem_FlashErase(FLASH_EraseType* pEraseType)
{
	Std_ReturnType State = E_NOT_OK;

	if(ERASE_PAGE == pEraseType -> EraseType)
	{
		UnlockFlash();
		if(EraseComplete != PageErase(pEraseType -> StartPage))
		{
			return State;
		}
	}
	else if(ERASE_SECTOR == pEraseType -> EraseType)
	{
		uint8 Page = pEraseType -> StartPage;
		uint8 Iterations = pEraseType -> PageNo;
		for(uint8 count = 0; count < Iterations; count++)
		{
			UnlockFlash();
			if(EraseComplete != PageErase(Page + count))
			{
				return State;
			}
		}
	}
	else if(ERASE_FLASH == pEraseType -> EraseType)
	{
		UnlockFlash();
		if(EraseComplete != FlashErase())
		{
			return State;
		}
	}
	else{
		return State;
	}

	LockFlash();

	State = E_OK;
	return State;
}

Std_ReturnType Mem_FlashWrite(uint32 Address, uint16 Byte_Count, void* pData)
{
	Std_ReturnType State = E_NOT_OK;

	UnlockFlash();

#if(FLASH_WRITE_DATA_SIZE_HALFWORD == FLASH_FLASH_DATA_WRITE_SIZE)

		uint16* pIHexData = (uint16*) pData;
		Byte_Count = Byte_Count >> 1;
		for(uint16 count = 0; count < Byte_Count; count++)
		{
			if(WriteComplete != FLASH_WriteHalfWord(Address, pIHexData[count]))
			{
				return State;
			}
			Address += 2;
		}

#elif(FLASH_WRITE_DATA_SIZE_WORD == FLASH_FLASH_DATA_WRITE_SIZE)

		uint32* pIHexData = (uint32*) pData;
		Byte_Count = Byte_Count >> 2;
		for(uint16 count = 0; count < Byte_Count; count++)
		{
			if(WriteComplete != FLASH_WriteWord(Address, pIHexData[count]))
			{
				return State;
			}
			Address += 4;
		}

#elif(FLASH_WRITE_DATA_SIZE_DOUBLEWORD == FLASH_FLASH_DATA_WRITE_SIZE)

		uint32* pIHexData = (uint32*) pData;
		Byte_Count = Byte_Count >> 3;
		for(uint16 count = 0; count < Byte_Count; count++)
		{
			if(WriteComplete != FLASH_WriteWord(Address, pIHexData[count]))
			{
				return State;
			}else
			{
				Address += 4;
				if(WriteComplete != FLASH_WriteWord(Address, pIHexData[count]))
				{
					return State;
				}
				Address += 4;
			}
		}

#else

		//error

#endif

	LockFlash();

	State = E_OK;
	return State;
}
