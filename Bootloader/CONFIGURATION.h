/*
 * Bootloader_cfg.h
 *
 *  Created on: 20 Feb 2020
 *      Author: Yahia
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/* Module Version 1.0.0 */
#define CONFIGURATION_SW_MAJOR_VERSION           (1U)
#define CONFIGURATION_SW_MINOR_VERSION           (0U)
#define CONFIGURATION_SW_PATCH_VERSION           (0U)

typedef uint8 Bootloader_SizeOfData;
#define FLASH_WRITE_DATA_SIZE_HALFWORD    ((Bootloader_SizeOfData)0x01U)  /*!< Program a half-word (16-bit) at a specified address   */
#define FLASH_WRITE_DATA_SIZE_WORD        ((Bootloader_SizeOfData)0x02U)  /*!< Program a word (32-bit) at a specified address        */
#define FLASH_WRITE_DATA_SIZE_DOUBLEWORD  ((Bootloader_SizeOfData)0x03U)  /*!< Program a double word (64-bit) at a specified address */


#define Bootloader_UART	(0x0U)
#define Bootloader_CAN	(0x1U)
#define Bootloader_USB	(0x2U)


#define BOOTLOADER_FlashProgrammedDataSize	FLASH_WRITE_DATA_SIZE_WORD

#define BOOTLOADER_CommProtocol		Bootloader_UART

/*
	Enable	: STD_ON
	Disable	: STD_OFF
 */
#define BOOTLOADER_CHECKSUM	STD_ON

/*
	Enable	: STD_ON
	Disable	: STD_OFF
 */
#define BOOTLOADER_ReadWhileWriting	STD_OFF

/*
	NoProtection	: No restrictions for SPM or LPM accessing the Application
					  section.
	SPMProtection	: SPM is not allowed to write to the Application section.
	FullProtection	: SPM is not allowed to write to the Application section, and
					  LPM executing from the Boot Loader section is not
					  allowed to read from the Application section. If interrupt
  	  	  	  	  	  vectors are placed in the Boot Loader section, interrupts
  	  	  	  	  	  are disabled while executing from the Application section.
	LPMProtection	: LPM executing from the Boot Loader section is not
  	  	  	  	  	  allowed to read from the Application section. If interrupt
  	  	  	  	  	  vectors are placed in the Boot Loader section, interrupts
  	  	  	  	  	  are disabled while executing from the Application section.
 */
#define BOOTLOADER_AppProtection			NoProtection

/*
	NoProtection	: No restrictions for SPM or LPM accessing the Boot Loader
					  section.
	SPMProtection	: SPM is not allowed to write to the Boot Loader section.
	FullProtection	: SPM is not allowed to write to the Boot Loader section,
					  and LPM executing from the Application section is not
  	  	  	  	  	  allowed to read from the Boot Loader section. If interrupt
  	  	  	  	  	  vectors are placed in the Application section, interrupts
  	  	  	  	  	  are disabled while executing from the Boot Loader section.
	LPMProtection	: LPM executing from the Application section is not allowed
  	  	  	  	  	  to read from the Boot Loader section. If interrupt vectors
  	  	  	  	  	  are placed in the Application section, interrupts are
  	  	  	  	  	  disabled while executing from the Boot Loader section.
 */
#define BOOTLOADER_BootloaderProtection		NoProtection

/*
	Enable	: STD_ON
	Disable	: STD_OFF
 */
#define BOOTLOADER_BootAfterReset	STD_ON

/*
	Enable	: STD_ON
	Disable	: STD_OFF
 */
#define BOOTLOADER_InterruptMode	STD_OFF

/*
	ResetAddress			: Address $0002
	ResetAddressPlusOffset	: Boot Reset Address + $0002
 */
#define BOOTLOADER_BootResetAddress	ResetAddress

/*
	WORDS_256	: 256 word / 512 byte
	WORDS_512	: 512 word / 1024 byte
	WORDS_1024	: 1024 word / 2048 byte
	WORDS_2048	: 2048 word / 4096 byte
 */
#define BOOTLOADER_BootSizeConfiguration	WORDS_2048

#endif /* BOOTLOADER_CFG_H_ */
