
#ifndef _MEMIF_H_
#define _MEMIF_H_

/* Module Version 1.0.0 */
#define MEMIF_SW_MAJOR_VERSION           (1U)
#define MEMIF_SW_MINOR_VERSION           (0U)
#define MEMIF_SW_PATCH_VERSION           (0U)


/* Bootloader Module Id */
#define MEMIF_MODULE_ID    (0x02)


#include "Platform_Types.h"
#include "Mem_dataTypes.h"
/* Checking between MEM and MEM_DataTypes Modules */
#if ((MEMIF_SW_MAJOR_VERSION != MEM_DATATYPES_SW_MAJOR_VERSION)\
 ||  (MEMIF_SW_MINOR_VERSION != MEM_DATATYPES_SW_MINOR_VERSION)\
 ||  (MEMIF_SW_PATCH_VERSION != MEM_DATATYPES_SW_PATCH_VERSION))
  #error "The SW version of Mem_dataTypes.h does not match the expected version"
#endif

#include "uC_Dependencies.h"
/* Checking between MEMIF and UC_DEPENDENCIES Modules */
#if ((MEMIF_SW_MAJOR_VERSION != UC_DEPENDENCIES_SW_MAJOR_VERSION)\
 ||  (MEMIF_SW_MINOR_VERSION != UC_DEPENDENCIES_SW_MINOR_VERSION)\
 ||  (MEMIF_SW_PATCH_VERSION != UC_DEPENDENCIES_SW_PATCH_VERSION))
  #error "The SW version of uC_Dependencies.h does not match the expected version"
#endif


#include "Mem_ABSTRACTION.h"
#include "Interrupts.h"


#include "Mem_API.h"
/* Checking between MEMIF and MEM_API Modules */
#if ((MEMIF_SW_MAJOR_VERSION != MEM_API_SW_MAJOR_VERSION)\
 ||  (MEMIF_SW_MINOR_VERSION != MEM_API_SW_MINOR_VERSION)\
 ||  (MEMIF_SW_PATCH_VERSION != MEM_API_SW_PATCH_VERSION))
  #error "The SW version of Mem_API.h does not match the expected version"
#endif


/* MemIf_ServiceId ENUM */
typedef uint8 MemIf_ServiceId;
#define MemIf_Init_API					((MemIf_ServiceId)0x01U)
#define MemIf_UnlockFlash_API			((MemIf_ServiceId)0x02U)
#define MemIf_LockFlash_API				((MemIf_ServiceId)0x03U)
#define MemIf_PageErase_API				((MemIf_ServiceId)0x04U)
#define MemIf_FlashErase_API			((MemIf_ServiceId)0x05U)
#define MemIf_Start_API					((MemIf_ServiceId)0x06U)
#define MemIf_End_API					((MemIf_ServiceId)0x07U)
#define MemIf_ChangeWriteDataSize_API	((MemIf_ServiceId)0x08U)
#define MemIf_FlashWrite_API			((MemIf_ServiceId)0x09U)
#define MemIf_GetVersion_API			((MemIf_ServiceId)0x0AU)
#define MemIf_Remove_Image_API			((MemIf_ServiceId)0x0BU)
#define MemIf_Update_Image_API			((MemIf_ServiceId)0x0CU)
#define MemIf_Install_Image_API			((MemIf_ServiceId)0x0DU)

#endif
