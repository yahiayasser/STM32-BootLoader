


#include "Bootloader.h"

Std_ReturnType Bootloader_Init(void)
{
	Std_ReturnType State = E_NOT_OK;
	Bootloader_HW_Init();

	Bootloader_FlashUnlock();
	Bootloader_FlashClrFlags();
	Bootloader_FlashLock();

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_Erase(uint8 PageNo)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_FlashUnlock();
	if(Bootloader_ErasePage(PageNo) == EraseComplete)
	{
		State = E_OK;
	}
	Bootloader_FlashLock();

	return State;
}
