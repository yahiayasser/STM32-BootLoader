


#include "Bootloader.h"


static void Bootloader_UnlockFlash(void);
static void Bootloader_LockFlash(void);
static Std_ReturnType Bootloader_PageErase(uint8 PageNo);
static void Bootloader_ChangeWriteDataSize(Bootloader_SizeOfData size);

static boolean FlashIsAlreadyUnocked_Flag = FALSE;
static uint8 FlashDontLock_Count = 0;

static Bootloader_FlashAddress FlashAdd = AppBase;
static Bootloader_SizeOfData SizeOfDataTobeWritten = BOOTLOADER_FlashProgrammedDataSize;

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

static Std_ReturnType Bootloader_PageErase(uint8 PageNo)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_UnlockFlash();
	if(PageErase(PageNo) == EraseComplete)
	{
		State = E_OK;
	}
	Bootloader_LockFlash();

	return State;
}

Std_ReturnType Bootloader_FlashErase(void)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_UnlockFlash();
	if(EraseComplete != FlashErase())
	{
		return State;
	}
	Bootloader_LockFlash();

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_Start(void)
{
	Std_ReturnType State = E_NOT_OK;
	FlashAdd = AppBase;

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

	if(FlagValue != Bootloader_GetStartFlag())
	{
		return State;
	}

	Bootloader_ClearStartFlag();

	Bootloader_LockFlash();
	State = E_OK;
	return State;
}

static void Bootloader_ChangeWriteDataSize(Bootloader_SizeOfData size)
{
	SizeOfDataTobeWritten = size;
}

Std_ReturnType Bootloader_FlashWrite(uint64 Data)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_UnlockFlash();

	if(FLASH_WRITE_DATA_SIZE_HALFWORD == SizeOfDataTobeWritten)
	{
		if(WriteComplete != FLASH_ProgramHalfWord(FlashAdd, Data))
		{
			return State;
		}
		else{
			FlashAdd += 2;
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_WORD == SizeOfDataTobeWritten)
	{
		if(WriteComplete != FLASH_ProgramWord(FlashAdd, Data))
		{
			return State;
		}
		else{
			FlashAdd += 4;
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_DOUBLEWORD == SizeOfDataTobeWritten)
	{
		if(WriteComplete != FLASH_ProgramWord(FlashAdd, Data))
		{
			return State;
		}
		else{
			FlashAdd += 4;
			if(WriteComplete != FLASH_ProgramWord(FlashAdd, (uint32)(Data >> 32)))
			{
				return State;
			}
			else{
				FlashAdd += 4;
			}
		}
	}else{}

	Bootloader_LockFlash();

	State = E_OK;
	return State;
}

void Bootloader_GetVersion(Bootloader_Version* version)
{
	*version = BootloaderVersion;
	return;
}

