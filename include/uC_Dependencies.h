#ifndef __UC_DEPENDENCIES_H
#define __UC_DEPENDENCIES_H

/* Module Version 1.0.0 */
#define UC_DEPENDENCIES_SW_MAJOR_VERSION           (1U)
#define UC_DEPENDENCIES_SW_MINOR_VERSION           (0U)
#define UC_DEPENDENCIES_SW_PATCH_VERSION           (0U)

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

/*----------------------- Hex file format ------------------------*/
#define BOOTLOADER_FILE_FORMAT	Intel_HEX

/*----------------------- Data word sizes in bits ------------------------*/
#define CONFIG_DATA_SIZE_BITS                   (WORD_SIZE_32)              /*  STM32f103 data bus is 32-bit wide.                            */

/*--------------------- Address word sizes in bits -----------------------*/
#define CONFIG_ADDR_SIZE_BITS                   (WORD_SIZE_32)              /*  STM32f103 address bus is 32-bit wide.                          */

/*----------------------- Stack Growth Direction -------------------------*/
#define CONFIG_STACK_GROWTH                     (STACK_GROWTH_HIGH_TO_LOW)  /*  ARM stack grows from high to low address.                           */

/*----------------------- Data word memory order -------------------------*/
#define CONFIG_ENDIAN_TYPE                      (ENDIAN_TYPE_LITTLE)        /*  ARM is a little endian processor.                                   */

#define FlashBase		(0x08000000)
#define FlashSize		(0x8000)	// In bytes
#define FlashPageSize	(0x400)
#define AppBase 		0x08002000
#define NumberOfPages	(FlashSize/FlashPageSize)
#define Bootloader_StartFlagAddress		(0x1FFFF804)
#define FlagValue 		0xAA
#define ClearFlagValue 	0xFF

#endif

#endif
