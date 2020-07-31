


#include "Bootloader.h"


__attribute__((section(".boot_code")))
static void Bootloader_UnlockFlash(void);

__attribute__((section(".boot_code")))
static void Bootloader_LockFlash(void);

__attribute__((section(".boot_code")))
static void Bootloader_EditData(uint8* pData, uint8* ByteCount);

static boolean FlashIsAlreadyUnocked_Flag = FALSE;
static uint8 FlashDontLock_Count = 0;

static Bootloader_FlashAddress Application_Add = AppStartAddress;
static Bootloader_SizeOfData SizeOfDataTobeWritten = BOOTLOADER_FlashProgrammedDataSize;

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
		Application_Add |= Frame_IHex.address;

		Bootloader_EditData(Frame_IHex.data, &(Frame_IHex.byte_count));
		Bootloader_FlashWrite(Frame_IHex.byte_count, Frame_IHex.data);
	}
	else if (IHEX_ELA == Frame_IHex.record_type)
	{
		Application_Add = (Application_Add & 0xFF) | (Frame_IHex.address << 16);
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

Std_ReturnType Bootloader_FlashWrite(uint8 Byte_Count, void* pData)
{
	Std_ReturnType State = E_NOT_OK;

	Bootloader_UnlockFlash();

#if(IntelHex_Type == BOOTLOADER_FrameType)

	uint16* pIHexData = (uint16*) pData;

	if(FLASH_WRITE_DATA_SIZE_HALFWORD == SizeOfDataTobeWritten)
	{
		for(uint8 count = 0; count < (Byte_Count >> 1); count++)
		{
			if(WriteComplete != FLASH_WriteHalfWord(Application_Add, pIHexData[count]))
			{
				return State;
			}else
			{
				Application_Add += 2;
			}
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_WORD == SizeOfDataTobeWritten)
	{
		for(uint8 count = 0; count < (Byte_Count >> 2); count++)
		{
			if(WriteComplete != FLASH_WriteHalfWord(Application_Add, pIHexData[count]))
			{
				return State;
			}else
			{
				Application_Add += 4;
			}
		}
	}
	else if(FLASH_WRITE_DATA_SIZE_DOUBLEWORD == SizeOfDataTobeWritten)
	{
		for(uint8 count = 0; count < (Byte_Count >> 3); count++)
		{
			if(WriteComplete != FLASH_WriteHalfWord(Application_Add, pIHexData[count]))
			{
				return State;
			}else
			{
				Application_Add += 4;
				if(WriteComplete != FLASH_WriteHalfWord(Application_Add, pIHexData[count]))
				{
					return State;
				}else
				{
					Application_Add += 4;
				}
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
	Bootloader_Start();
}

