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

typedef uint8 Std_ReturnType;
#define E_OK 					(Std_ReturnType)0
#define E_NOT_OK 				(Std_ReturnType)1

#endif /* BOOTLOADER_INTERFACE_H_ */
