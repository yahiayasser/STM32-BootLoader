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
#define FLASH_WRITE_DATA_SIZE_HALFWORD    ((Bootloader_SizeOfData)0x01U)  /*!< Program a half-word (16-bit) at a specified address   */
#define FLASH_WRITE_DATA_SIZE_WORD        ((Bootloader_SizeOfData)0x02U)  /*!< Program a word (32-bit) at a specified address        */
#define FLASH_WRITE_DATA_SIZE_DOUBLEWORD  ((Bootloader_SizeOfData)0x03U)  /*!< Program a double word (64-bit) at a specified address */

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
	uint8 major;
	uint8 minor;
	uint8 patch;
}Bootloader_Version;



#endif
