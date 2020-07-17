/*
 * Bootloader_UARTFlashing.c
 *
 *  Created on: 15 Jul 2020
 *      Author: Yahia
 */


#include "Bootloader.h"

Bootloader_EraseType pEraseType;

Std_ReturnType Bootloader(Bootloader_SignalType signal)
{
	Std_ReturnType State = E_NOT_OK;

	switch(signal)
	{
	case REMOVE_APP:
		pEraseType.EraseType = ERASE_SECTOR;
		pEraseType.StartPage = (AppBase - FlashBase)/FlashPageSize;
		pEraseType.PageNo = (FlashBase + FlashSize - AppBase)/FlashPageSize;
		if(E_NOT_OK != Bootloader_FlashErase(&pEraseType))
		{
			return State;
		}
		break;

	case REMOVE_BOOTLOADER:
		pEraseType.EraseType = ERASE_SECTOR;
		pEraseType.StartPage = (BOOTLOADER_START_ADD - FlashBase)/FlashPageSize;
		pEraseType.PageNo = (AppBase - FlashBase)/FlashPageSize;
		if(E_NOT_OK != Bootloader_FlashErase(&pEraseType))
		{
			return State;
		}
		break;

	case REMOVE_BOTH_APP_BOOTLOADER:
		pEraseType.EraseType = ERASE_FLASH;
		if(E_NOT_OK != Bootloader_FlashErase(&pEraseType))
		{
			return State;
		}
		break;

	case UPDATE_APP:
		break;
	case UPDATE_BOOTLOADER:
		break;
	case INSTALL_APP:
		break;
	case INSTALL_BOOTLOADER:
		break;
	default:
		return State;
		break;
	}

	State = E_OK;
	return State;
}

Std_ReturnType Remove_Image(void)
{
	Std_ReturnType State = E_NOT_OK;


	State = E_OK;
	return State;
}

Std_ReturnType Update_Image(void)
{
	Std_ReturnType State = E_NOT_OK;

	State = E_OK;
	return State;
}

Std_ReturnType Install_Image(void)
{
	Std_ReturnType State = E_NOT_OK;

	State = E_OK;
	return State;
}

