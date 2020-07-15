/*
 * Bootloader.h
 *
 *  Created on: 29 Jun 2020
 *      Author: Yahia
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

/* Module Version 1.0.0 */
#define BOOTLOADER_SW_MAJOR_VERSION           (1U)
#define BOOTLOADER_SW_MINOR_VERSION           (0U)
#define BOOTLOADER_SW_PATCH_VERSION           (0U)


#include "uC_Dependencies.h"
/* Checking between Bootloader and UC_DEPENDENCIES Modules */
#if ((BOOTLOADER_SW_MAJOR_VERSION != UC_DEPENDENCIES_SW_MAJOR_VERSION)\
 ||  (BOOTLOADER_SW_MINOR_VERSION != UC_DEPENDENCIES_SW_MINOR_VERSION)\
 ||  (BOOTLOADER_SW_PATCH_VERSION != UC_DEPENDENCIES_SW_PATCH_VERSION))
  #error "The SW version of uC_Dependencies.h does not match the expected version"
#endif


#include "Platform_Types.h"
#include "ABSTRACTION.h"


#include "API.h"
/* Checking between Bootloader and Bootloader_interface Modules */
#if ((BOOTLOADER_SW_MAJOR_VERSION != API_SW_MAJOR_VERSION)\
 ||  (BOOTLOADER_SW_MINOR_VERSION != API_SW_MINOR_VERSION)\
 ||  (BOOTLOADER_SW_PATCH_VERSION != API_SW_PATCH_VERSION))
  #error "The SW version of Bootloader_interface.h does not match the expected version"
#endif


#include "CONFIGURATION.h"
/* Checking between Bootloader and CONFIGURATION Modules */
#if ((BOOTLOADER_SW_MAJOR_VERSION != CONFIGURATION_SW_MAJOR_VERSION)\
 ||  (BOOTLOADER_SW_MINOR_VERSION != CONFIGURATION_SW_MINOR_VERSION)\
 ||  (BOOTLOADER_SW_PATCH_VERSION != CONFIGURATION_SW_PATCH_VERSION))
  #error "The SW version of CONFIGURATION.h does not match the expected version"
#endif

typedef struct
{
	uint8 major;
	uint8 minor;
	uint8 patch;
}Bootloader_Version;

typedef uint32 Bootloader_FlashAddress;


#endif /* BOOTLOADER_H_ */
