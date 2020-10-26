/*
 * MEM_API.h
 *
 *  Created on: 20 Feb 2020
 *      Author: Yahia
 */

#ifndef _MEM_API_H_
#define _MEM_API_H_

/* Module Version 1.0.0 */
#define MEM_API_SW_MAJOR_VERSION           (1U)
#define MEM_API_SW_MINOR_VERSION           (0U)
#define MEM_API_SW_PATCH_VERSION           (0U)

#include "Mem_dataTypes.h"
/* Checking between MEM and MEM_DataTypes Modules */
#if ((MEM_API_SW_MAJOR_VERSION != MEM_DATATYPES_SW_MAJOR_VERSION)\
 ||  (MEM_API_SW_MINOR_VERSION != MEM_DATATYPES_SW_MINOR_VERSION)\
 ||  (MEM_API_SW_PATCH_VERSION != MEM_DATATYPES_SW_PATCH_VERSION))
  #error "The SW version of Mem_dataTypes.h does not match the expected version"
#endif

/*	FLASH_WRITE_DATA_SIZE_HALFWORD
 * 	FLASH_WRITE_DATA_SIZE_WORD
 * 	FLASH_WRITE_DATA_SIZE_DOUBLEWORD
 */
#define FLASH_FLASH_DATA_WRITE_SIZE	FLASH_WRITE_DATA_SIZE_WORD

#ifndef STD_HIGH
#define STD_HIGH		0x01
#endif
#ifndef STD_LOW
#define STD_LOW			0x00
#endif

#ifndef STD_ACTIVE
#define STD_ACTIVE		0x01
#endif
#ifndef STD_IDLE
#define STD_IDLE		0x00
#endif

#ifndef STD_ON
#define STD_ON			0x01
#endif
#ifndef STD_OFF
#define STD_OFF			0x00
#endif

#define BOOTLOADER_START_ADD	(0x08006000)

__attribute__((section(".boot_code")))
void UnlockFlash(void);

__attribute__((section(".boot_code")))
void LockFlash(void);

__attribute__((section(".boot_code")))
void Mem_init(void);

__attribute__((section(".boot_code")))
Std_ReturnType Mem_FlashErase(FLASH_EraseType* pEraseType);

__attribute__((section(".boot_code")))
Std_ReturnType Mem_FlashWrite(uint32 Address, uint16 Byte_Count, void* pData);


#endif /* BOOTLOADER_INTERFACE_H_ */
