#ifndef _MEM_DATATYPES_H_
#define _MEM_DATATYPES_H_

/* Module Version 1.0.0 */
#define MEM_DATATYPES_SW_MAJOR_VERSION           (1U)
#define MEM_DATATYPES_SW_MINOR_VERSION           (0U)
#define MEM_DATATYPES_SW_PATCH_VERSION           (0U)

typedef uint8 FLASH_SizeOfData;
#define FLASH_WRITE_DATA_SIZE_HALFWORD    ((FLASH_SizeOfData)0x10)  /*!< Program a half-word (16-bit) at a specified address   */
#define FLASH_WRITE_DATA_SIZE_WORD        ((FLASH_SizeOfData)0x20)  /*!< Program a word (32-bit) at a specified address        */
#define FLASH_WRITE_DATA_SIZE_DOUBLEWORD  ((FLASH_SizeOfData)0x40)  /*!< Program a double word (64-bit) at a specified address */


typedef enum{
	ERASE_PAGE,
	ERASE_SECTOR,
	ERASE_FLASH
}Flash_EraseSize;

typedef struct
{
	Flash_EraseSize EraseType;
	uint8 StartPage;
	uint8 PageNo;
}FLASH_EraseType;

typedef struct
{
	uint8 major;
	uint8 minor;
	uint8 patch;
}MemIf_Version;



#endif
