#ifndef _BOOTLOADER_DATATYPES_H_
#define _BOOTLOADER_DATATYPES_H_

/* Module Version 1.0.0 */
#define BOOTLOADER_DATATYPES_SW_MAJOR_VERSION           (1U)
#define BOOTLOADER_DATATYPES_SW_MINOR_VERSION           (0U)
#define BOOTLOADER_DATATYPES_SW_PATCH_VERSION           (0U)


typedef uint8 Std_ReturnType;
#define E_OK 					(Std_ReturnType)0
#define E_NOT_OK 				(Std_ReturnType)1

typedef uint8 Bootloader_SizeOfData;
#define FLASH_WRITE_DATA_SIZE_HALFWORD    ((Bootloader_SizeOfData)0x10)  /*!< Program a half-word (16-bit) at a specified address   */
#define FLASH_WRITE_DATA_SIZE_WORD        ((Bootloader_SizeOfData)0x20)  /*!< Program a word (32-bit) at a specified address        */
#define FLASH_WRITE_DATA_SIZE_DOUBLEWORD  ((Bootloader_SizeOfData)0x40)  /*!< Program a double word (64-bit) at a specified address */

typedef uint8 JumpMode;
#define APP_MODE 					(JumpMode)0
#define BOOT_MODE 					(JumpMode)1


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
	ERASE_PAGE,
	ERASE_SECTOR,
	ERASE_FLASH
}Flash_EraseType;

typedef enum{
	REMOVE_APP,
	REMOVE_BOOTLOADER,
	REMOVE_BOTH_APP_BOOTLOADER,
	UPDATE_APP,
	UPDATE_BOOTLOADER,
	INSTALL_APP,
	INSTALL_BOOTLOADER
}Bootloader_SignalType;

typedef struct
{
	Flash_EraseType EraseType;
	uint8 StartPage;
	uint8 PageNo;
}Bootloader_EraseType;

typedef struct
{
	FlashAddress AppAddress;
	FlashAddress BootloaderAddress;
	uint32 ApplicationSize;
	boolean Boot_Flag;
	boolean BootSuccesfull_Flag;
	boolean FirstTime_Flag;
	uint8 stub;
}Bootloader_Info;

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
