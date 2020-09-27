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
#define MaxFrameLength			46

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_Init(void);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_FlashErase(Bootloader_EraseType* pEraseType);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_FlashWrite(uint32 Address, uint8 Byte_Count, void* pData);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_End(void);

__attribute__((section(".boot_code")))
void Bootloader_GetVersion(Bootloader_Version* version);

__attribute__((section(".boot_code")))
void Bootloader_ChangeWriteDataSize(Bootloader_SizeOfData size);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_ReceiveFrame(void* Frame);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_ParseFrame(void* ASCII_Frame, void* Frame);

__attribute__((section(".boot_code")))
void Bootloader_JumpToApp(void);

__attribute__((section(".boot_code")))
void Bootloader_Main(void);

__attribute__((section(".boot_code")))
Std_ReturnType Bootloader_WriteFrame(void* Frame);


#endif /* BOOTLOADER_INTERFACE_H_ */
