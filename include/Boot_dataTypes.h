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

typedef uint8 JumpMode;
#define APP_MODE 					(JumpMode)0
#define BOOT_MODE 					(JumpMode)1

typedef FLASH_EraseType Bootloader_EraseType;
typedef FLASH_SizeOfData Bootloader_SizeOfData;


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
	ERROR_BootloaderTerminate
}Bootloader_ErrorType;

typedef struct
{
	pFunction Main;
	FlashAddress AppAddress;
	FlashAddress BootloaderAddress;
	uint32 ApplicationSize;
	boolean Boot_Flag;
	boolean BootSuccesfull_Flag;
	boolean NotFirstTime_Flag;
	uint8 stub;
}Bootloader_Info;
#define SIZEOF_FlashInfo	20

typedef struct
{
	uint8 major;
	uint8 minor;
	uint8 patch;
}Bootloader_Version;

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




#endif
