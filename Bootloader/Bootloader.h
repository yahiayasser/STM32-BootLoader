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


/* Bootloader Module Id */
#define BOOTLOADER_MODULE_ID    (0x01)


#include "Platform_Types.h"


#include "uC_Dependencies.h"
/* Checking between Bootloader and UC_DEPENDENCIES Modules */
#if ((BOOTLOADER_SW_MAJOR_VERSION != UC_DEPENDENCIES_SW_MAJOR_VERSION)\
 ||  (BOOTLOADER_SW_MINOR_VERSION != UC_DEPENDENCIES_SW_MINOR_VERSION)\
 ||  (BOOTLOADER_SW_PATCH_VERSION != UC_DEPENDENCIES_SW_PATCH_VERSION))
  #error "The SW version of uC_Dependencies.h does not match the expected version"
#endif


#include "Mem_API.h"
/* Checking between BOOTLOADER and MEM_API Modules */
#if ((BOOTLOADER_SW_MAJOR_VERSION != MEM_API_SW_MAJOR_VERSION)\
 ||  (BOOTLOADER_SW_MINOR_VERSION != MEM_API_SW_MINOR_VERSION)\
 ||  (BOOTLOADER_SW_PATCH_VERSION != MEM_API_SW_PATCH_VERSION))
  #error "The SW version of Mem_API.h does not match the expected version"
#endif

#include "Boot_dataTypes.h"
/* Checking between Bootloader and Bootloader_DataTypes Modules */
#if ((BOOTLOADER_SW_MAJOR_VERSION != BOOTLOADER_DATATYPES_SW_MAJOR_VERSION)\
 ||  (BOOTLOADER_SW_MINOR_VERSION != BOOTLOADER_DATATYPES_SW_MINOR_VERSION)\
 ||  (BOOTLOADER_SW_PATCH_VERSION != BOOTLOADER_DATATYPES_SW_PATCH_VERSION))
  #error "The SW version of Bootloader_DataTypes.h does not match the expected version"
#endif


#include "ABSTRACTION.h"
#include "Interrupts.h"

#include "Com_Config.h"


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


/* Bootloader_ServiceId ENUM */
typedef uint8 Bootloader_ServiceId;
#define Bootloader_Init_API					((Bootloader_ServiceId)0x01U)
#define Bootloader_UnlockFlash_API			((Bootloader_ServiceId)0x02U)
#define Bootloader_LockFlash_API			((Bootloader_ServiceId)0x03U)
#define Bootloader_PageErase_API			((Bootloader_ServiceId)0x04U)
#define Bootloader_FlashErase_API			((Bootloader_ServiceId)0x05U)
#define Bootloader_Start_API				((Bootloader_ServiceId)0x06U)
#define Bootloader_End_API					((Bootloader_ServiceId)0x07U)
#define Bootloader_ChangeWriteDataSize_API	((Bootloader_ServiceId)0x08U)
#define Bootloader_FlashWrite_API			((Bootloader_ServiceId)0x09U)
#define Bootloader_GetVersion_API			((Bootloader_ServiceId)0x0AU)
#define Bootloader_Remove_Image_API			((Bootloader_ServiceId)0x0BU)
#define Bootloader_Update_Image_API			((Bootloader_ServiceId)0x0CU)
#define Bootloader_WriteFrame_API			((Bootloader_ServiceId)0x0DU)
#define Bootloader_ReceiveFrame_API			((Bootloader_ServiceId)0x0EU)
#define Bootloader_ParseFrame_API			((Bootloader_ServiceId)0x0FU)
#define Bootloader_JumpToApp_API			((Bootloader_ServiceId)0x10U)
#define Bootloader_EditData_API				((Bootloader_ServiceId)0x11U)
#define Bootloader_Checksum_API				((Bootloader_ServiceId)0x12U)
#define Bootloader_Main_API					((Bootloader_ServiceId)0x13U)
#define BranchingCode_API					((Bootloader_ServiceId)0x14U)
#define String_To_IHex_API					((Bootloader_ServiceId)0x15U)
#define ASCII_To_HEX_API					((Bootloader_ServiceId)0x16U)
#define string_length_API					((Bootloader_ServiceId)0x17U)


#include "DET.h"
/* Checking between Bootloader and DET Modules */
#if ((BOOTLOADER_SW_MAJOR_VERSION != DET_SW_MAJOR_VERSION)\
 ||  (BOOTLOADER_SW_MINOR_VERSION != DET_SW_MINOR_VERSION)\
 ||  (BOOTLOADER_SW_PATCH_VERSION != DET_SW_PATCH_VERSION))
  #error "The SW version of DET.h does not match the expected version"
#endif



JumpMode BranchingCode(void);

void stub(void);


#endif /* BOOTLOADER_H_ */
