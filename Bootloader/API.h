/*
 * API.h
 *
 *  Created on: 20 Feb 2020
 *      Author: Yahia
 */

#ifndef _API_H_
#define _API_H_

/* Module Version 1.0.0 */
#define API_SW_MAJOR_VERSION           (1U)
#define API_SW_MINOR_VERSION           (0U)
#define API_SW_PATCH_VERSION           (0U)

#define STD_HIGH		0x01
#define STD_LOW			0x00

#define STD_ACTIVE		0x01
#define STD_IDLE		0x00

#define STD_ON			0x01
#define STD_OFF			0x00

#define BOOTLOADER_START_ADD	(0x08006000)

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_Init(void);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_FlashErase(Bootloader_EraseType* pEraseType);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_FlashWrite(uint8 Byte_Count, void* pData);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_Start(void);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_End(void);

__attribute__((section(".boot_code")))
void Bootloader_GetVersion(Bootloader_Version* version);

__attribute__((section(".boot_code")))
void Bootloader_ChangeWriteDataSize(Bootloader_SizeOfData size);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_ReceiveFrame(void* Frame);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_ParseFrame(void* Frame);

__attribute__((section(".boot_code")))
void Bootloader_JumpToApp(void);


#endif /* BOOTLOADER_INTERFACE_H_ */
