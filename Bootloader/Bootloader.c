


#include "Bootloader.h"

__attribute__((section(".boot_code")))
static void Bootloader_EditData(uint8* pData, uint8* ByteCount);

__attribute__((section(".boot_code")))
static uint8 string_length(uint8* str);

__attribute__((section(".boot_code")))
static uint8 ASCII_To_HEX(uint8 ASCII);

__attribute__((section(".boot_code")))
static Std_ReturnType String_To_IHex(uint8* ASCII_Frame, void* Frame);

#if(STD_ON == BOOTLOADER_CHECKSUM)

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_Checksum(void* Frame);

#endif

Bootloader_Info Temp_FlashInfo;

static Bootloader_SizeOfData SizeOfDataTobeWritten = BOOTLOADER_FlashProgrammedDataSize;

Bootloader_Version BootloaderVersion;

pFunction Bootloader_JumpToBootloader = stub;

uint32 APP_Counter;

boolean ELAFirstTime_Flag = TRUE;


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

	if(Mem_FlashErase(&pErase) != E_OK)
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_Init_API, ERROR_FlashErase);
		return State;
	}

	Bootloader_ChangeWriteDataSize(FLASH_WRITE_DATA_SIZE_WORD);

	if(Bootloader_FlashWrite(BootloaderFlagStartAddress, SIZEOF_FlashInfo, (Bootloader_Info*)(&Temp_FlashInfo)) != E_OK)
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_Init_API, ERROR_FlashWrite);
		return State;
	}

	BootloaderVersion.major = BOOTLOADER_SW_MAJOR_VERSION;
	BootloaderVersion.minor = BOOTLOADER_SW_MINOR_VERSION;
	BootloaderVersion.patch = BOOTLOADER_SW_PATCH_VERSION;

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_ReceiveFrame(void* Frame)
{
	Std_ReturnType State = E_NOT_OK;

	/* StringFrame string to hold the ASCII value of the incoming IHex frame */
	uint8 StringFrame[MaxFrameLength];

	uint8 DataSize = 0;

	uint8 count = 0;

#if(IntelHex_Type == BOOTLOADER_FrameType)

	COMReceive((uint16)9, (void*)StringFrame);

	DataSize = (ASCII_To_HEX(StringFrame[1]) << 4) | ASCII_To_HEX(StringFrame[2]);

	COMReceive((uint16)((DataSize << 1) + 2), (void*)&(StringFrame[9]));

	COMReceive((uint16)1, (void*)&(StringFrame[11 + (DataSize << 1)]));

	if(StringFrame[11 + (DataSize << 1)] != '\r' || StringFrame[11 + (DataSize << 1)] != '\n')
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_ReceiveFrame_API, ERROR_Frame);
		return State;
	}
	StringFrame[11 + (DataSize << 1)] = '\0';

	while(StringFrame[count] != '\0')
	{
		*((uint8*)Frame + count) = StringFrame[count];
		count++;
	}
	*((uint8*)Frame + count) = '\0';

#elif(Motorola_S_Record_Type == BOOTLOADER_FrameType)

#elif(RawBinary_Type == BOOTLOADER_FrameType)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_ParseFrame(void* ASCII_Frame, void* Frame)
{
	Std_ReturnType State = E_NOT_OK;

#if(IntelHex_Type == BOOTLOADER_FrameType)

	IHex_Frame Frame_IHex;

	/* Convert ASCII frame to Hex frame */
	if(E_OK != String_To_IHex((uint8*) ASCII_Frame, (void*) Frame))
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_ParseFrame_API, ERROR_Frame);
		return State;
	}

	Frame_IHex = *((IHex_Frame*) Frame);

	if(IHEX_DATA == Frame_IHex.record_type)
	{
		APP_Counter = (APP_Counter & 0xFFFF0000) | (Frame_IHex.address);
	}
	else if (IHEX_ELA == Frame_IHex.record_type)
	{
		if(TRUE == ELAFirstTime_Flag)
		{
			Temp_FlashInfo.AppAddress = (Temp_FlashInfo.AppAddress & 0xFF) | (Frame_IHex.address << 16);
			ELAFirstTime_Flag = FALSE;
		}
		APP_Counter = (APP_Counter & 0xFF) | (Frame_IHex.address << 16);
	}else{}

#elif(Motorola_S_Record_Type == BOOTLOADER_FrameType)

#elif(RawBinary_Type == BOOTLOADER_FrameType)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

	State = E_OK;
	return State;
}

Std_ReturnType Bootloader_End(void)
{
	Std_ReturnType State = E_NOT_OK;
	Bootloader_EraseType pErase;

	Bootloader_UnlockFlash();

	ELAFirstTime_Flag = TRUE;

	Temp_FlashInfo.BootSuccesfull_Flag = TRUE;
	Temp_FlashInfo.Boot_Flag = FALSE;
	Temp_FlashInfo.NotFirstTime_Flag = FALSE;

	pErase.EraseType = ERASE_PAGE;
	pErase.StartPage = BootloaderFlagIndex;

	if(Mem_FlashErase(&pErase) != E_OK)
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_End_API, ERROR_FlashErase);
		return State;
	}

	Bootloader_ChangeWriteDataSize(FLASH_WRITE_DATA_SIZE_WORD);

	if(Bootloader_FlashWrite(BootloaderFlagStartAddress, SIZEOF_FlashInfo, (Bootloader_Info*)(&Temp_FlashInfo)) != E_OK)
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_End_API, ERROR_FlashWrite);
		return State;
	}

	Bootloader_LockFlash();

	//Bootloader_JumpToApp();

	State = E_OK;
	return State;
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


	if(Mem_FlashWrite(Address, Byte_Count, pData, SizeOfDataTobeWritten) != E_OK)
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_FlashWrite_API, ERROR_FlashWrite);
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

#if(STD_ON == BOOTLOADER_CHECKSUM)
Std_ReturnType Bootloader_Checksum(void* Frame)
{
	Std_ReturnType State = E_NOT_OK;
	uint8 ui8count;
	uint8 ui8CheckSumValue = 0;

#if(IntelHex_Type == BOOTLOADER_FrameType)

	IHex_Frame IHexFrame = *((IHex_Frame*) Frame);

	/* Calculating value of check sum */
	ui8CheckSumValue = IHexFrame.byte_count + IHexFrame.record_type + (IHexFrame.address >> 8) + (IHexFrame.address & 0xFF);

	for(ui8count = 0; ui8count < IHexFrame.byte_count; ui8count++)
	{
		/* Calculating value of check sum */
		ui8CheckSumValue += IHexFrame.data[ui8count];
	}

	/*A record's checksum byte is the two's complement of the least significant byte (LSB) of the sum of all decoded byte values
	  in the record preceding the checksum. It is computed by summing the decoded byte values and extracting the LSB of the sum
	  (i.e., the data checksum), and then calculating the two's complement of the LSB */

	ui8CheckSumValue = (~ui8CheckSumValue);
	ui8CheckSumValue++;

	if(ui8CheckSumValue != IHexFrame.checksum){
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_Checksum_API, ERROR_Checksum);
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
#endif

Std_ReturnType Bootloader_WriteFrame(void* Frame)
{
	Std_ReturnType State = E_NOT_OK;
	IHex_Frame Frame_IHex = *((IHex_Frame*)Frame);

	Bootloader_EditData(Frame_IHex.data, &(Frame_IHex.byte_count));

	if(E_OK != Bootloader_FlashWrite(APP_Counter, Frame_IHex.byte_count, Frame_IHex.data))
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_WriteFrame_API, ERROR_FlashWrite);
		return State;
	}

	State = E_OK;
	return State;
}

void Bootloader_Main(void)
{
	IHex_Frame Frame;

	/* StringFrame string to hold the ASCII value of the incoming IHex frame */
	uint8 StringFrame[MaxFrameLength] = ":1020000000000000000000000000000000000000D0\r\n";
	APP_Counter = 0x08000000;

	Bootloader_Init();

	do
	{
		/*if(Bootloader_ReceiveFrame((void *)&StringFrame) != E_OK)
		{
			Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_Main_API, ERROR_FrameReceive);
		}*/

		if(Bootloader_ParseFrame((void *)&StringFrame, (void *)&Frame) != E_OK)
		{
			Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_Main_API, ERROR_FrameParse);
		}

#if(STD_ON == BOOTLOADER_CHECKSUM)
		if(Bootloader_Checksum((void *)&Frame) != E_OK)
		{
			Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_Main_API, ERROR_Checksum);
		}
#endif

		if(Bootloader_WriteFrame((void *)&Frame) != E_OK)
		{
			Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_Main_API, ERROR_FlashWrite);
		}

	}while(Frame.record_type != IHEX_EOF);

	if(Bootloader_End() != E_OK)
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, Bootloader_Main_API, ERROR_BootloaderTerminate);
	}

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
		else
		{
			Bootloader_JumpToBootloader = (pFunction)Bootloader_Main;
		}
		retVal = BOOT_MODE;
	}

	return retVal;
}

static Std_ReturnType String_To_IHex(uint8* ASCII_Frame, void* Frame)
{
	Std_ReturnType State = E_NOT_OK;

#if(IntelHex_Type == BOOTLOADER_FrameType)

	/* Intel Hex frame */
	IHex_Frame IHexFrame;

	/* Counters */
	uint8 ui8count = 0;

	uint8 ASCII_Frame_Length = string_length(ASCII_Frame);

	/* Byte count is two hex digits ( 1 & 2 ) */
	IHexFrame.byte_count = (ASCII_To_HEX(ASCII_Frame[1]) << 4) | ASCII_To_HEX(ASCII_Frame[2]);

	/* First Hex digit must be colon */
	if(ASCII_Frame[0] != ':')
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, String_To_IHex_API, ERROR_Frame);
		return State;
	}

	/* Numbers of characters in frame must be byte count + 11 (data length) */
	else if(ASCII_Frame_Length != ((IHexFrame.byte_count << 1) + 11 + 2))
	{
		Det_ReportError(BOOTLOADER_MODULE_ID, String_To_IHex_API, ERROR_Frame);
		return State;
	}

	else{}

	/* Address is four digits ( 3 & 4 & 5 & 6 ) */
	IHexFrame.address = (ASCII_To_HEX(ASCII_Frame[3]) << 12) | (ASCII_To_HEX(ASCII_Frame[4]) << 8) | (ASCII_To_HEX(ASCII_Frame[5]) << 4) | ASCII_To_HEX(ASCII_Frame[6]);

	/* Record is two digits ( 7 & 8 ) */
	IHexFrame.record_type = (ASCII_To_HEX(ASCII_Frame[7]) << 4) | ASCII_To_HEX(ASCII_Frame[8]);

	/* Checksum is the last two digits  */
	IHexFrame.checksum = (ASCII_To_HEX(ASCII_Frame[(IHexFrame.byte_count << 1) + 9]) << 4) | ASCII_To_HEX(ASCII_Frame[(IHexFrame.byte_count << 1) + 10]);

	for(ui8count = 0; ui8count < IHexFrame.byte_count; ui8count++)
	{
		IHexFrame.data[ui8count] = (ASCII_To_HEX(ASCII_Frame[ui8count*2 + 9]) << 4) | ASCII_To_HEX(ASCII_Frame[ui8count*2 + 10]);
	}

	*((IHex_Frame*) Frame) = IHexFrame;

#elif(Motorola_S_Record_Type == BOOTLOADER_FrameType)

#elif(RawBinary_Type == BOOTLOADER_FrameType)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif

	State = E_OK;
	return State;
}


static uint8 ASCII_To_HEX(uint8 ASCII)
{
	/* Function to convert ASCII letter to Hex digit */

	if((ASCII >= '0') && (ASCII <= '9'))
	{
		return (ASCII - '0');
	}
	else if((ASCII >= 'A') && (ASCII <= 'F'))
	{
		return (ASCII - 'A' + 10);
	}
	else if((ASCII >= 'a') && (ASCII <= 'f'))
	{
		return (ASCII - 'a' + 10);
	}
	else
	{
		return 255;
	}
}

static uint8 string_length(uint8* str)
{
	/* Function to get the length of string */

	uint8 count;
	/* count the string start from element 0 until the element before the NULL terminator */
	for(count = 0; str[count] != '\0'; ++count);
	return count;
}
