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

#define BOOTLOADER_START_ADD	(0x08000000)

/* Maximum size of data in IHex frame
 * Default:	32*/
#define IHexDataMaxLength	32

/* IHex_Frame is a structure representing Intel Hex file format */
typedef struct{
	/* Sequence of n bytes of data, represented by 2n hex digits */
	uint8 data[IHexDataMaxLength];

	/* Four hex digits, representing the 16-bit beginning memory address offset of the data */
	uint16 address;

	/* Two hex digits, indicating the number of bytes (hex digit pairs) in the data field */
	uint8 byte_count;

	/* Two hex digits, 00 to 05, defining the meaning of the data field */
	IHex_RecordTypes record_type;

	/* Two hex digits, a computed value that can be used to verify the record has no errors */
	uint8 checksum;

	/* Flag to check whether the frame is valid or not (not intel hex standard) */
	boolean valid;
}IHex_Frame;

Std_ReturnType Bootloader_Init(void);
Std_ReturnType Bootloader_FlashErase(Bootloader_EraseType* pEraseType);
Std_ReturnType Bootloader_FlashWrite(uint64 Data);
Std_ReturnType Bootloader_Start(void);
Std_ReturnType Bootloader_End(void);
void Bootloader_GetVersion(Bootloader_Version* version);
void Bootloader_ChangeWriteDataSize(Bootloader_SizeOfData size);


#endif /* BOOTLOADER_INTERFACE_H_ */
