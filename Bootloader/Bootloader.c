


#include "Bootloader.h"


__attribute__((section(".boot_code")))
static void Bootloader_UnlockFlash(void);

__attribute__((section(".boot_code")))
static void Bootloader_LockFlash(void);

static boolean FlashIsAlreadyUnocked_Flag = FALSE;
static uint8 FlashDontLock_Count = 0;

static Bootloader_FlashAddress Application_Add = AppStartAddress;
static Bootloader_SizeOfData SizeOfDataTobeWritten = BOOTLOADER_FlashProgrammedDataSize;

extern USART_InitTypeDef BootloaderUART;

pFunction Jump_To_Application;

Bootloader_Version BootloaderVersion;

static void Bootloader_UnlockFlash(void)
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

static void Bootloader_LockFlash(void)
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

Std_ReturnType Bootloader_Init(void)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_HW_Init();

	Bootloader_UnlockFlash();
	FlashClrFlags();
	Bootloader_LockFlash();

	BootloaderVersion.major = BOOTLOADER_SW_MAJOR_VERSION;
	BootloaderVersion.minor = BOOTLOADER_SW_MINOR_VERSION;
	BootloaderVersion.patch = BOOTLOADER_SW_PATCH_VERSION;

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_FlashErase(Bootloader_EraseType* pEraseType)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_UnlockFlash();

	if(ERASE_PAGE == pEraseType -> EraseType)
	{
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
			if(EraseComplete != PageErase(Page + count))
			{
				return State;
			}
		}
	}
	else if(ERASE_FLASH == pEraseType -> EraseType)
	{
		if(EraseComplete != FlashErase())
		{
			return State;
		}
	}
	else{
		return State;
	}

	Bootloader_LockFlash();

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_Start(void)
{
	Std_ReturnType State = E_NOT_OK;
	Application_Add = AppStartAddress;

	Bootloader_UnlockFlash();
	if(WriteComplete != Bootloader_SetStartFlag())
	{
		return State;
	}

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_End(void)
{
	Std_ReturnType State = E_NOT_OK;

	if(Bootloader_NotCompleted != Bootloader_GetStartFlag())
	{
		return State;
	}

	Bootloader_ClearStartFlag();

	Bootloader_LockFlash();
	State = E_OK;
	return State;
}

void Bootloader_ChangeWriteDataSize(Bootloader_SizeOfData size)
{
	SizeOfDataTobeWritten = size;
}

Std_ReturnType Bootloader_FlashWrite(uint64 Data)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_UnlockFlash();

	if(FLASH_WRITE_DATA_SIZE_HALFWORD == SizeOfDataTobeWritten)
	{
		if(WriteComplete != FLASH_WriteHalfWord(Application_Add, Data))
		{
			return State;
		}
		else{
			Application_Add += 2;
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_WORD == SizeOfDataTobeWritten)
	{
		if(WriteComplete != FLASH_WriteWord(Application_Add, Data))
		{
			return State;
		}
		else{
			Application_Add += 4;
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_DOUBLEWORD == SizeOfDataTobeWritten)
	{
		if(WriteComplete != FLASH_WriteWord(Application_Add, Data))
		{
			return State;
		}
		else{
			Application_Add += 4;
			if(WriteComplete != FLASH_WriteWord(Application_Add, (uint32)(Data >> 32)))
			{
				return State;
			}
			else{
				Application_Add += 4;
			}
		}
	}
	else{
		return State;
	}

	Bootloader_LockFlash();

	State = E_OK;
	return State;
}

void Bootloader_GetVersion(Bootloader_Version* version)
{
	*version = BootloaderVersion;
	return;
}

void Bootloader_JumpToApp(void)
{
	Disable_Interrupts();
	SystemReset();
	while(1);
}

