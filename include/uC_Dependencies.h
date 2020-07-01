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


#if(STM32f103 == BOOTLOADER_TARGET)

#include "stm32f10x.h"

#define FlashBase		0x08000000
#define FlashSize		32000	// In bytes
#define FlashPageSize	1000
#define AppBase 		0x08000000
#define NumberOfPages	(FlashSize/FlashPageSize)

#endif

#endif
