/*
 * DET.h
 *
 *  Created on: 20 Feb 2020
 *      Author: Yahia
 */

#ifndef _DET_H_
#define _DET_H_

#define DET_SW_MAJOR_VERSION			(1U)
#define DET_SW_MINOR_VERSION			(0U)
#define DET_SW_PATCH_VERSION			(0U)

void Det_ReportError(uint16 ModuleId, uint8 ApiId, uint8 ErrorId);

#endif
