


#include "Bootloader.h"


__attribute__((section(".boot_code")))
static void Bootloader_UnlockFlash(void);

__attribute__((section(".boot_code")))
static void Bootloader_LockFlash(void);

__attribute__((section(".boot_code")))
static void Bootloader_EditData(uint8* pData, uint8* ByteCount);

static boolean FlashIsAlreadyUnocked_Flag = FALSE;
static uint8 FlashDontLock_Count = 0;

__attribute__((section(".boot_flags")))
Bootloader_Info FlashInfo;

Bootloader_Info Temp_FlashInfo;

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
	Bootloader_EraseType pErase;
	Bootloader_Info* pFlashInfo = &FlashInfo;

	Bootloader_HW_Init();

	Bootloader_UnlockFlash();
	FlashClrFlags();
	Bootloader_LockFlash();

	Temp_FlashInfo = *pFlashInfo;

	if(Temp_FlashInfo.FirstTime_Flag != TRUE)
	{
		Temp_FlashInfo.AppAddress = AppStartAddress;
		Temp_FlashInfo.ApplicationSize = 0x00;
		Temp_FlashInfo.Boot_Flag = TRUE;
		Temp_FlashInfo.BootloaderAddress = BootloaderImageStartAddress;
	}

	Temp_FlashInfo.BootSuccesfull_Flag = FALSE;

	pErase.EraseType = ERASE_PAGE;
	pErase.StartPage = BootloaderFlagIndex;

	Bootloader_FlashErase(&pErase);
	Bootloader_ChangeWriteDataSize(FLASH_WRITE_DATA_SIZE_WORD);
	Bootloader_FlashWrite(BootloaderFlagStartAddress, 16, (Bootloader_Info*)(&Temp_FlashInfo));

	BootloaderVersion.major = BOOTLOADER_SW_MAJOR_VERSION;
	BootloaderVersion.minor = BOOTLOADER_SW_MINOR_VERSION;
	BootloaderVersion.patch = BOOTLOADER_SW_PATCH_VERSION;

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_ReceiveFrame(void* Frame)
{
	Std_ReturnType State = E_NOT_OK;

#if(IntelHex_Type == BOOTLOADER_FrameType)

	volatile uint8 Temp = 0;
	IHex_Frame Frame_IHex = *((IHex_Frame*) Frame);

	COMReceive((uint16)1, (void*)&Temp);

	if(Temp != ':')
	{
		return State;
	}

	COMReceive((uint16)1, 						(void*)&(Frame_IHex.byte_count));
	COMReceive((uint16)2, 						(void*)&(Frame_IHex.address));
	COMReceive((uint16)1, 						(void*)&(Frame_IHex.record_type));
	COMReceive((uint16)(Frame_IHex.byte_count), (void*)&(Frame_IHex.data));
	COMReceive((uint16)1,						(void*)&(Frame_IHex.checksum));

	COMReceive((uint16)1, (void*)&Temp);
	if(Temp != '\r' || Temp != '\n')
	{
		return State;
	}

#elif(Motorola_S_Record_Type == BOOTLOADER_FrameType)

#elif(RawBinary_Type == BOOTLOADER_FrameType)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_ParseFrame(void* Frame)
{
	Std_ReturnType State = E_NOT_OK;

#if(IntelHex_Type == BOOTLOADER_FrameType)

	IHex_Frame Frame_IHex = *((IHex_Frame*) Frame);

	if(IHEX_DATA == Frame_IHex.record_type)
	{
		Temp_FlashInfo.AppAddress |= Frame_IHex.address;

		Bootloader_EditData(Frame_IHex.data, &(Frame_IHex.byte_count));
		Bootloader_FlashWrite(Temp_FlashInfo.AppAddress, Frame_IHex.byte_count, Frame_IHex.data);
		Temp_FlashInfo.AppAddress += Frame_IHex.byte_count;
	}
	else if (IHEX_ELA == Frame_IHex.record_type)
	{
		Temp_FlashInfo.AppAddress = (Temp_FlashInfo.AppAddress & 0xFF) | (Frame_IHex.address << 16);
	}

#elif(Motorola_S_Record_Type == BOOTLOADER_FrameType)

#elif(RawBinary_Type == BOOTLOADER_FrameType)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_FlashErase(Bootloader_EraseType* pEraseType)
{
	Std_ReturnType State = E_NOT_OK;

	if(ERASE_PAGE == pEraseType -> EraseType)
	{
		Bootloader_UnlockFlash();
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
			Bootloader_UnlockFlash();
			if(EraseComplete != PageErase(Page + count))
			{
				return State;
			}
		}
	}
	else if(ERASE_FLASH == pEraseType -> EraseType)
	{
		Bootloader_UnlockFlash();
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

void Bootloader_End(void)
{
	Bootloader_EraseType pErase;

	Bootloader_UnlockFlash();

	Temp_FlashInfo.BootSuccesfull_Flag = TRUE;
	Temp_FlashInfo.Boot_Flag = FALSE;
	Temp_FlashInfo.FirstTime_Flag = FALSE;

	pErase.EraseType = ERASE_PAGE;
	pErase.StartPage = BootloaderFlagIndex;

	Bootloader_FlashErase(&pErase);
	Bootloader_ChangeWriteDataSize(FLASH_WRITE_DATA_SIZE_WORD);
	Bootloader_FlashWrite(BootloaderFlagStartAddress, 16, (Bootloader_Info*)(&Temp_FlashInfo));

	Bootloader_LockFlash();

	Bootloader_JumpToApp();
}

void Bootloader_ChangeWriteDataSize(Bootloader_SizeOfData size)
{
	SizeOfDataTobeWritten = size;
}

Std_ReturnType Bootloader_FlashWrite(uint32 Address, uint8 Byte_Count, void* pData)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_UnlockFlash();

#if(IntelHex_Type == BOOTLOADER_FrameType)


	if(FLASH_WRITE_DATA_SIZE_HALFWORD == SizeOfDataTobeWritten)
	{
		uint16* pIHexData = (uint16*) pData;
		Byte_Count = Byte_Count >> 1;
		for(uint8 count = 0; count < Byte_Count; count++)
		{
			if(WriteComplete != FLASH_WriteHalfWord(Address, pIHexData[count]))
			{
				return State;
			}
			Address += 2;
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_WORD == SizeOfDataTobeWritten)
	{
		uint32* pIHexData = (uint32*) pData;
		Byte_Count = Byte_Count >> 2;
		for(uint8 count = 0; count < Byte_Count; count++)
		{
			if(WriteComplete != FLASH_WriteWord(Address, pIHexData[count]))
			{
				return State;
			}
			Address += 4;
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_DOUBLEWORD == SizeOfDataTobeWritten)
	{
		uint32* pIHexData = (uint32*) pData;
		Byte_Count = Byte_Count >> 3;
		for(uint8 count = 0; count < Byte_Count; count++)
		{
			if(WriteComplete != FLASH_WriteWord(Address, pIHexData[count]))
			{
				return State;
			}else
			{
				Address += 4;
				Temp_FlashInfo.AppAddress += 4;
				if(WriteComplete != FLASH_WriteWord(Address, pIHexData[count]))
				{
					return State;
				}
				Address += 4;
			}
		}
	}
	else
	{
		return State;
	}

#elif(Motorola_S_Record_Type == BOOTLOADER_FrameType)

#elif(RawBinary_Type == BOOTLOADER_FrameType)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

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

static void Bootloader_EditData(uint8* pData, uint8* ByteCount)
{
	uint8 count = *ByteCount;
	if(FLASH_WRITE_DATA_SIZE_HALFWORD == SizeOfDataTobeWritten)
	{
		if((count%2) != 0)
		{
			pData[count] = 0xFF;
			count++;
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_WORD == SizeOfDataTobeWritten)
	{
		while((count%4) != 0)
		{
			pData[count] = 0xFF;
			count++;
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_DOUBLEWORD == SizeOfDataTobeWritten)
	{
		while((count%8) != 0)
		{
			pData[count] = 0xFF;
			count++;
		}
	}
	else
	{
		return;
	}

	*ByteCount = count;
}

void Bootloader_Main(void)
{
	Bootloader_Init();
	Bootloader_End();
}

JumpMode BranchingCode(void)
{
	JumpMode retVal = APP_MODE;
	Bootloader_Info* pInfo = &FlashInfo;

	Temp_FlashInfo = *pInfo;

	Temp_FlashInfo.Boot_Flag = TRUE;

	if(Temp_FlashInfo.Boot_Flag == TRUE)
	{
		retVal = BOOT_MODE;
	}
	return retVal;
}

