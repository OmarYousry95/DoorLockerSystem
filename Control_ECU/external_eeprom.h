/*******************************************************************************************
 * [FILE NAME]:		external_eeprom.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	30 Jan 2020
 * [DESCRIPTION]:	This header file contains static configurations and function prototypes
 * 					for external EEPROM 24C16 (11-bit address) which communicates with I2C
 *******************************************************************************************/
#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

/******************************************************************
 * 				Common Header Files Inclusion					  *
 ******************************************************************/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/******************************************************************
 * 				   			 Macros					      		  *
 ******************************************************************/
#define SLAVE_ADDRESS_W(ADD)	(uint8)(((ADD&0x700)>>7)|0xA0)
#define SLAVE_ADDRESS_R(ADD)	(uint8)((((ADD&0x700)>>7)|0xA0)|0x01)

#define EEPROM_SUCCESS	1u
#define EEPROM_ERROR	0u

/******************************************************************
 * 				  Public Functions Prototypes					  *
 ******************************************************************/
void EEPROM_init(void);
uint8 EEPROM_writeByte(uint16 u16addr, const uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr, uint8 * u8data);

#endif /* EXTERNAL_EEPROM_H_ */
