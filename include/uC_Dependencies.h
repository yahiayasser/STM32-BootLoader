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

#include "stm32f10x.h"

#endif
