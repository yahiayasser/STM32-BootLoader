#ifndef _BOOTLOADER_DATATYPES_H_
#define _BOOTLOADER_DATATYPES_H_

/* Module Version 1.0.0 */
#define BOOTLOADER_DATATYPES_SW_MAJOR_VERSION           (1U)
#define BOOTLOADER_DATATYPES_SW_MINOR_VERSION           (0U)
#define BOOTLOADER_DATATYPES_SW_PATCH_VERSION           (0U)


#include "Mem_dataTypes.h"
/* Checking between MEM and MEM_DataTypes Modules */
#if ((BOOTLOADER_DATATYPES_SW_MAJOR_VERSION != MEM_DATATYPES_SW_MAJOR_VERSION)\
 ||  (BOOTLOADER_DATATYPES_SW_MINOR_VERSION != MEM_DATATYPES_SW_MINOR_VERSION)\
 ||  (BOOTLOADER_DATATYPES_SW_PATCH_VERSION != MEM_DATATYPES_SW_PATCH_VERSION))
  #error "The SW version of Mem_dataTypes.h does not match the expected version"
#endif

typedef uint8 BufferStatus;
#define BufferStatus_Complete		(BufferStatus)0
#define BufferStatus_NotComplete	(BufferStatus)1
#define BufferStatus_Failed			(BufferStatus)2

typedef FLASH_EraseType Bootloader_EraseType;


/* Maximum size of data in IHex frame
 * Default:	16*/
#define IHexDataMaxLength	0x10

typedef enum{
    IHEX_DATA,	// Data
    IHEX_EOF,	// End Of File
    IHEX_ESA,	// Extended Segment Address
    IHEX_SSA,	// Start Segment Address
    IHEX_ELA,	// Extended Linear Address
    IHEX_SLA	// Start Linear Address
}IHex_RecordTypes;

typedef enum{
	REMOVE_APP,
	REMOVE_BOOTLOADER,
	REMOVE_BOTH_APP_BOOTLOADER,
	UPDATE_APP,
	UPDATE_BOOTLOADER,
	INSTALL_APP,
	INSTALL_BOOTLOADER
}Bootloader_SignalType;

typedef enum{
	ERROR_FlashErase,
	ERROR_FlashWrite,
	ERROR_Frame,
	ERROR_Checksum,
	ERROR_FrameParse,
	ERROR_FrameReceive,
	ERROR_FrameHandle,
	ERROR_BootloaderTerminate
}Bootloader_ErrorType;

typedef struct
{
	uint8 major;
	uint8 minor;
	uint8 patch;
}Bootloader_Version;

struct Bootloader_InitInfo
{
	pFunction Main;
	FlashAddress AppAddress;
	FlashAddress BootloaderAddress;
	uint32 ApplicationSize;
	struct Bootloader_InitInfo* pInitStruct;
	Bootloader_Version BootloaderVersion;
	boolean Boot_Flag;
	boolean BootSuccesfull_Flag;
	boolean NotFirstTime_Flag;
	uint8 NumberOfPagesFilledByPrevProgram;
};
typedef struct Bootloader_InitInfo Bootloader_InitTypeDef;
#define SIZEOF_FlashInfo	27

/* IHex_Frame is a structure representing Intel Hex file format */
typedef struct{
	/* Sequence of n bytes of data, represented by 2n hex digits */
	uint8 data[IHexDataMaxLength];

	/* Four hex digits, representing the 16-bit beginning memory address offset of the data */
	uint16 address;

	/* Two hex digits, indicating the number of bytes (hex digit pairs) in the data field */
	uint8 byte_count;

	/* Two hex digits, a computed value that can be used to verify the record has no errors */
	uint8 checksum;

	/* Two hex digits, 00 to 05, defining the meaning of the data field */
	IHex_RecordTypes record_type;
}IHex_Frame;

typedef struct
{
	uint8 BUFFER[FlashPageSize];
#if(IntelHex_Type == BOOTLOADER_FrameType)
	IHex_Frame BUFFER_CurrentFrame;
#elif(Motorola_S_Record_Type == BOOTLOADER_FrameType)

#elif(RawBinary_Type == BOOTLOADER_FrameType)

#else
#error "Invalid value of BOOTLOADER_CommProtocol"
#endif
	uint32 BUFFER_BaseAdd;
	uint16 BUFFER_StartAdd;
	uint16 BUFFER_NextAdd;
	uint16 BUFFER_Counter;
	uint8 BUFFER_CurrentPageIndex;
	BufferStatus BUFFER_Status;
}Frame_Buffer;




#endif
