#ifndef __UC_DEPENDENCIES_H
#define __UC_DEPENDENCIES_H

/* Module Version 1.0.0 */
#define UC_DEPENDENCIES_SW_MAJOR_VERSION           (1U)
#define UC_DEPENDENCIES_SW_MINOR_VERSION           (0U)
#define UC_DEPENDENCIES_SW_PATCH_VERSION           (0U)

#include "Platform_Types.h"

/* uC operating frequency */
#ifndef Freq_CPU
#define Freq_CPU 8000000UL //	8MHz Clock frequency
#endif


/*-------------------------------- Targets ----------------------------*/
#define STM32f103		1

/*-------------------------------- Word Sizes ----------------------------*/
#define  WORD_SIZE_08                          (8U)   /*  8-bit word size (1 Bytes).*/
#define  WORD_SIZE_16                          (16U)  /* 16-bit word size (2 Bytes).*/
#define  WORD_SIZE_32                          (32U)  /* 32-bit word size (4 Bytes).*/
#define  WORD_SIZE_64                          (64U)  /* 64-bit word size (8 Bytes).*/

/*----------------------- Stack Growth Direction ------------------------*/
#define  STACK_GROWTH_NONE                      (0U)
#define  STACK_GROWTH_HIGH_TO_LOW               (1U)
#define  STACK_GROWTH_LOW_TO_HIGH               (2U)

/*---------------------------- Endianness -------------------------------*/
#define  ENDIAN_TYPE_BIG                        (1U)   /* Big-endian order, Store most significant byte in the lowest memory address.     */
#define  ENDIAN_TYPE_LITTLE                     (2U)   /* Little-endian order, Store most significant byte in the highest memory address. */

/*----------------------- File format types ------------------------*/
#define Intel_HEX			0
#define Motorola_S_Records	1
#define Raw_Binary			2


/*----------------------- Operating uC ------------------------*/
#define BOOTLOADER_TARGET STM32f103


#if(STM32f103 == BOOTLOADER_TARGET)

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

typedef struct{
	uint32 SCB_CPUID;
	uint32 SCB_ICSR;
	uint32 SCB_VTOR;
	uint32 SCB_AIRCR;
	uint32 SCB_SCR;
	uint32 SCB_CCR;
	uint32 SCB_SHPR1;
	uint32 SCB_SHPR2;
	uint32 SCB_SHPR3;
	uint32 SCB_SHCRS;
	uint32 SCB_CFSR;
	uint32 SCB_HFSR;
	uint32 SCB_MMAR;
	uint32 SCB_BFAR;
}SCB_registers;

#define pSCB	((SCB_registers*) 0xE000ED00)

/*----------------------- Hex file format ------------------------*/
#define BOOTLOADER_FILE_FORMAT					Intel_HEX

/*----------------------- Data word sizes in bits ------------------------*/
#define CONFIG_DATA_SIZE_BITS                   (WORD_SIZE_32)              /*  STM32f103 data bus is 32-bit wide.                            */

/*--------------------- Address word sizes in bits -----------------------*/
#define CONFIG_ADDR_SIZE_BITS                   (WORD_SIZE_32)              /*  STM32f103 address bus is 32-bit wide.                          */

/*----------------------- Stack Growth Direction -------------------------*/
#define CONFIG_STACK_GROWTH                     (STACK_GROWTH_HIGH_TO_LOW)  /*  ARM stack grows from high to low address.                           */

/*----------------------- Data word memory order -------------------------*/
#define CONFIG_ENDIAN_TYPE                      (ENDIAN_TYPE_LITTLE)        /*  ARM is a little endian processor.                                   */

#define FlashStartAddress				(0x08000000)
#define FlashEndAddress					(0x0800FFFF)
#define FlashSize						(0x10000)	// In bytes
#define FlashPageSize					(0x400)
#define FlashNumberOfPages				(FlashSize/FlashPageSize)


#define AppStartAddress					(0x08000000)
#define AppEndAddress					(0x0800BFFF)
#define AppSize							(0xC000)	// In bytes
#define AppNumberOfPages				(AppSize/FlashPageSize)
#define AppIndex						((AppStartAddress - FlashStartAddress)/FlashPageSize)


#define BootloaderImageStartAddress		(0x0800C000)
#define BootloaderImageEndAddress		(0x0800FFFF)
#define BootloaderImageSize				(0x4000)	// In bytes
#define BootloaderImageNumberOfPages	(BootloaderImageSize/FlashPageSize)
#define BootloaderImageIndex			((BootloaderImageStartAddress - FlashStartAddress)/FlashPageSize)


#define BootloaderFlagStartAddress		(0x0800BC00)
#define BootloaderFlagEndAddress		(0x0800BFFF)
#define BootloaderFlagSize				(0x400)	// In bytes
#define BootloaderFlagNumberOfPages		(BootloaderFlagSize/FlashPageSize)
#define BootloaderFlagIndex				((BootloaderFlagStartAddress - FlashStartAddress)/FlashPageSize)


#define Bootloader_StartFlagAddress		(0x1FFFF804)
#define Bootloader_NotCompleted 		0xAA
#define Bootloader_Completed 			0xFF


#define JumpToBootloader_Flag		(0x1FFFF806)

#define SystemReset()	pSCB -> SCB_AIRCR = ((0x05FAUL << 16) | 4)

#endif

#endif
