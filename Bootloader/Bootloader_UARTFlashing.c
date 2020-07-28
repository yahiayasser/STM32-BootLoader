/*
 * Bootloader_UARTFlashing.c
 *
 *  Created on: 15 Jul 2020
 *      Author: Yahia
 */


#include "Bootloader.h"

#define MaxFrameLength	42
#define PageSizeB		32

Bootloader_EraseType pEraseType;

Std_ReturnType Bootloader(Bootloader_SignalType signal)
{
	Std_ReturnType State = E_NOT_OK;

	switch(signal)
	{
	case REMOVE_APP:
		pEraseType.EraseType = ERASE_SECTOR;
		pEraseType.StartPage = (AppStartAddress - FlashStartAddress)/FlashPageSize;
		pEraseType.PageNo = (FlashStartAddress + AppSize - AppStartAddress)/FlashPageSize;
		if(E_NOT_OK != Bootloader_FlashErase(&pEraseType))
		{
			return State;
		}
		break;

	case REMOVE_BOOTLOADER:
		pEraseType.EraseType = ERASE_SECTOR;
		pEraseType.StartPage = (BOOTLOADER_START_ADD - FlashStartAddress)/FlashPageSize;
		pEraseType.PageNo = (AppStartAddress - FlashStartAddress)/FlashPageSize;
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

#if(Intel_HEX == BOOTLOADER_FILE_FORMAT)

	/* IHex_Frame structure to hold the Hex value of the incoming IHex frame */
	IHex_Frame IHexFrame;

	/* StringFrame string to hold the ASCII value of the incoming IHex frame */
	uint8 StringFrame[MaxFrameLength];

	/* Counter */
	uint8 ui8Count = 0;

	/* DataBuffer array to hold data that will be written in the flash */
	uint8 DataBuffer[PageSizeB];

	/* Counter of DataBuffer array */
	uint8 DataBufferCounter;

	/* NextAddress is a pointer to the next page that will be written.
	 * Used to check if the writing process on the flash is continuous or not.
	 */
	uint32 NextAddress = NULL_Address;

	/* PageAddress holds the address value of page that will be written */
	uint32 PageAddress = 0x0;

	/* FileType specify whether the file is Application or Boot-loader */
	uint8 FileType;

#endif

	State = E_OK;
	return State;
}

