


#include "Bootloader.h"

__attribute__((section(".boot_code")))
static void Bootloader_EditData(uint8* pData, uint8* ByteCount);

Bootloader_Info Temp_FlashInfo;

static Bootloader_SizeOfData SizeOfDataTobeWritten = BOOTLOADER_FlashProgrammedDataSize;

Bootloader_Version BootloaderVersion;

pFunction Bootloader_JumpToBootloader = stub;


Std_ReturnType Bootloader_Init(void)
{
	Std_ReturnType State = E_NOT_OK;
	Bootloader_EraseType pErase;
	Bootloader_Info* pFlashInfo = (Bootloader_Info*)BootloaderFlagStartAddress;

	Bootloader_HW_Init();

	Mem_init();

	Temp_FlashInfo = *pFlashInfo;

	if(Temp_FlashInfo.NotFirstTime_Flag != TRUE)
	{
		Temp_FlashInfo.AppAddress = AppStartAddress;
		Temp_FlashInfo.ApplicationSize = 0x00;
		Temp_FlashInfo.BootloaderAddress = BootloaderImageStartAddress;
		Temp_FlashInfo.NotFirstTime_Flag = TRUE;
	}
	Temp_FlashInfo.Boot_Flag = TRUE;
	Temp_FlashInfo.BootSuccesfull_Flag = FALSE;
	Temp_FlashInfo.Main = BOOT_MAIN;

	pErase.EraseType = ERASE_PAGE;
	pErase.StartPage = BootloaderFlagIndex;

	Mem_FlashErase(&pErase);
	Bootloader_ChangeWriteDataSize(FLASH_WRITE_DATA_SIZE_WORD);
	Bootloader_Write(BootloaderFlagStartAddress, SIZEOF_FlashInfo, (Bootloader_Info*)(&Temp_FlashInfo));

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
		Bootloader_Write(Temp_FlashInfo.AppAddress, Frame_IHex.byte_count, Frame_IHex.data);
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

void Bootloader_End(void)
{
	Bootloader_EraseType pErase;

	UnlockFlash();

	Temp_FlashInfo.BootSuccesfull_Flag = TRUE;
	Temp_FlashInfo.Boot_Flag = FALSE;
	Temp_FlashInfo.NotFirstTime_Flag = FALSE;

	pErase.EraseType = ERASE_PAGE;
	pErase.StartPage = BootloaderFlagIndex;

	Mem_FlashErase(&pErase);
	Bootloader_ChangeWriteDataSize(FLASH_WRITE_DATA_SIZE_WORD);
	Bootloader_Write(BootloaderFlagStartAddress, SIZEOF_FlashInfo, (Bootloader_Info*)(&Temp_FlashInfo));

	LockFlash();

	//Bootloader_JumpToApp();
}

void Bootloader_ChangeWriteDataSize(Bootloader_SizeOfData size)
{
	SizeOfDataTobeWritten = size;
}

Std_ReturnType Bootloader_Write(uint32 Address, uint8 Byte_Count, void* pData)
{
	Std_ReturnType State = E_NOT_OK;

	UnlockFlash();

#if(IntelHex_Type == BOOTLOADER_FrameType)


	if(Mem_FlashWrite(Address, Byte_Count, pData, SizeOfDataTobeWritten) != E_OK)
	{
		return State;
	}

#elif(Motorola_S_Record_Type == BOOTLOADER_FrameType)

#elif(RawBinary_Type == BOOTLOADER_FrameType)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

	LockFlash();

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
	while (1);
}

JumpMode BranchingCode(void)
{
	JumpMode retVal = APP_MODE;
	Bootloader_Info* pInfo = (Bootloader_Info*)BootloaderFlagStartAddress;
	Bootloader_Info BootInfo = *(Bootloader_Info*)pInfo;
	BootInfo.Boot_Flag = TRUE;
	if(BootInfo.Boot_Flag == TRUE)
	{
		if(BootInfo.NotFirstTime_Flag == TRUE)
		{
			Bootloader_JumpToBootloader = BootInfo.Main;
		}
		Bootloader_JumpToBootloader = (pFunction)Bootloader_Main;
		retVal = BOOT_MODE;
	}
	return retVal;
}

