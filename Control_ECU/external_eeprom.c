/*******************************************************************************************
 * [FILE NAME]:		external_eeprom.c
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	30 Jan 2020
 * [DESCRIPTION]:	This c file contains implementation of functions for external EEPROM
 * 					24C16 (11-bit address) which communicates with I2C
 *******************************************************************************************/

#include "external_eeprom.h"
#include "i2c.h"

/******************************************************************
 * 				  Public Functions Definitions					  *
 ******************************************************************/
void EEPROM_init(void)
{
	/*Structure of I2C_ConfigType type to initialise I2C module:
	 * 1. Prescaler = 1 (4^0=1)
	 * 2. Bit rate = 400 kbps
	 * 3. (Slave address = 1) not used*/
	I2C_ConfigType I2C_Config_EEPROM={1,1,400000};
	/*Call TWI_init function to initialise I2C with configuration parameters*/
	TWI_init(&I2C_Config_EEPROM);
}


uint8 EEPROM_writeByte(uint16 u16addr, const uint8 u8data)
{
/*******************************************************************
 * STA | Slave Add | W | ACK | Memory Loc | ACK | Data | ACK | STO *
 *******************************************************************/
	/*Send start bit to begin frame*/
	TWI_start();
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of start bit, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_START)
		return EEPROM_ERROR;
	/*Write the slave address of EEPROM (1010 + 3 bits from memory location (A10 A9 A8) + Write*/
	TWI_write(SLAVE_ADDRESS_W(u16addr));
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of slave address + ACK from slave, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_MT_SLA_W_ACK)
		return EEPROM_ERROR;
	/*Write the rest of memory location address from A7 --> A0*/
	TWI_write((uint8)(u16addr));
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of data + ACK from slave, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_MT_DATA_ACK)
		return EEPROM_ERROR;
	/*Write data inside memory location*/
	TWI_write(u8data);
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of data + ACK from slave, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_MT_DATA_ACK)
		return EEPROM_ERROR;
	/*Send stop bit*/
	TWI_stop();
	/*Return success indicating successful transmission of whole frame*/
	return EEPROM_SUCCESS;
}


uint8 EEPROM_readByte(uint16 u16addr, uint8 * u8data)
{
/************************************************************************************************
 * STA | Slave Add | W | ACK | Memory Loc | ACK | Sr | Slave Add. | R | ACK | Data | NACK | STO *
 ************************************************************************************************/

	/*Send start bit to begin frame*/
	TWI_start();
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of start bit, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_START)
		return EEPROM_ERROR;

	/*Write the slave address of EEPROM (1010 + 3 bits from memory location (A10 A9 A8) + Write*/
	TWI_write(SLAVE_ADDRESS_W(u16addr));
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of slave address + ACK from slave, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_MT_SLA_W_ACK)
		return EEPROM_ERROR;

	/*Write the rest of memory location address from A7 --> A0*/
	TWI_write((uint8)(u16addr));
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of data + ACK from slave, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_MT_DATA_ACK)
		return EEPROM_ERROR;

	/*Send repeated start bit to change write operation to read from same slave*/
	TWI_start();
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of start bit, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_REP_START)
		return EEPROM_ERROR;

	/*Write the slave address of EEPROM (1010 + 3 bits from memory location (A10 A9 A8) + Read*/
	TWI_write(SLAVE_ADDRESS_R(u16addr));
	/*Check if status register doesn't hold the corresponding number for successful transmission
	 *of slave address + ACK from slave, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_MT_SLA_R_ACK)
		return EEPROM_ERROR;

	/*Read a byte of data from memory and save it in u8data variable*/
	*u8data=TWI_readWithNACK();
	/*Check if status register doesn't hold the corresponding number for successful reception
	 *of a byte of data + NACK from slave, return error, else continue sending the frame*/
	if(TWI_getStatus()!=TWI_MR_DATA_NACK)
		return EEPROM_ERROR;

	/*Send stop bit*/
	TWI_stop();

	/*Return success indicating successful transmission of whole frame*/
	return EEPROM_SUCCESS;
}
