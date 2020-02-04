/*******************************************************************************************
 * [FILE NAME]:		i2c.c
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	30 Jan 2020
 * [DESCRIPTION]:	This c file contains implementation of functions for I2C (TWI) Module
 * 					in AVR ATMEGA-16 Micro-controller
 *******************************************************************************************/

#include "i2c.h"


/******************************************************************
 * 				  Public Functions Definitions					  *
 ******************************************************************/
/*******************************************************************************************
 *[Function Name]	: TWI_init
 *[Description]		: This function initialises the I2C module with required specs as follows:
 *						1. Inserts slave address of microcontroller.
 *						   This address will be used when the microcontroller acts as a slave
 *						2. Inserts prescaler value either it's 1, 4, 16 or 64
 *						3. Calculates the value of TWBR register according to input SCL frequency
 *						   and prescalar value
 *						4. Enables I2C module
 *[Arguments]		: const I2C_ConfigType * Config_Ptr
 *						- Pointer to structure of I2C_ConfigType type having all the parameters
 *						  to initialise I2C module as desired
 *						- It's const so as function don't change anything of it
 *[Return]			: void
 *******************************************************************************************/
void TWI_init(const I2C_ConfigType * Config_Ptr)
{
	/*Insert controller's slave address in Most Significant 7 bits of TWAR register
	 *if any device wants to address it as a slave*/
	TWAR=(TWAR&0x01)|((Config_Ptr->address)<<1);
	/*Disable General Call Recognition*/
	CLEAR_BIT(TWAR,TWGCE);
	/*Configures prescaler as required*/
	switch(Config_Ptr->prescaler)
	{
		/*4^0 = 1*/
		case 1:
			CLEAR_BIT(TWSR,TWPS0);
			CLEAR_BIT(TWSR,TWPS1);
			break;
		/*4^1 = 4*/
		case 4:
			SET_BIT(TWSR,TWPS0);
			CLEAR_BIT(TWSR,TWPS1);
			break;
		/*4^2 = 16*/
		case 16:
			CLEAR_BIT(TWSR,TWPS0);
			SET_BIT(TWSR,TWPS1);
			break;
		/*4^3 = 64*/
		case 64:
			SET_BIT(TWSR,TWPS0);
			SET_BIT(TWSR,TWPS1);
			break;
	}
	/*Calculate TWBR register value through required SCL clock and prescaler value*/
	TWBR=TWBR_VAL(Config_Ptr->SCL_Freq,Config_Ptr->prescaler);
	/*Enable I2C module by setting TWEN bit in TWCR register*/
	SET_BIT(TWCR,TWEN);
}

/*******************************************************************************************
 *[Function Name]	: TWI_start
 *[Description]		: This function sends a start bit on SDA bus to start and own the bus
 *[Arguments]		: void
 *[Return]			: void
 *******************************************************************************************/
void TWI_start(void)
{
	/*1. Clear TWINT flag by setting it before sending
	 *2. Keep enabling TWI module i.e. TWEN=1
	 *3. Send start bit i.e. TWSTA=1*/
	TWCR=(1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	/*Busy wait until TWINT flag is set which means start bit is sent successfully*/
	while(IS_BIT_CLEAR(TWCR,TWINT));
}

/******************************************************************************************
 *[Function Name]	: TWI_stop
 *[Description]		: This function sends a stop bit on SDA bus to indicate stop of sending
 *[Arguments]		: void
 *[Return]			: void
 ******************************************************************************************/
void TWI_stop(void)
{
	/*1. Clear TWINT flag by setting it before sending
	 *2. Keep enabling TWI module i.e. TWEN=1
	 *3. Send stop bit i.e. TWSTO=1*/
	TWCR=(1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

/******************************************************************************************
 *[Function Name]	: TWI_write
 *[Description]		: This function sends a byte of data on SDA bus either it's byte/address
 *[Arguments]		: const uint8 data
 *						- This is a variable having the byte of data to be sent
 *						- It's const so as dunction don't corrupt it
 *[Return]			: void
 ******************************************************************************************/
void TWI_write(const uint8 data)
{
	/*Put data byte in TWDR register to be written*/
	TWDR=data;

	/*1. Clear TWINT flag by setting it before sending
	 *2. Keep enabling TWI module i.e. TWEN=1*/
	TWCR=(1<<TWINT) | (1<<TWEN);

	/*Busy wait until TWINT flag is set which means data byte is sent successfully*/
	while(IS_BIT_CLEAR(TWCR,TWINT));
}

/******************************************************************************************
 *[Function Name]	: TWI_readWithACK
 *[Description]		: This function reads a byte of data on SDA bus either it's byte/address
 *					  with sending ACK signal after it.
 *[Arguments]		: void
 *[Return]			: uint8
 *						returns the received byte of data
 ******************************************************************************************/
uint8 TWI_readWithACK(void)
{
	/*1. Clear TWINT flag by setting it before sending
	 *2. Keep enabling TWI module i.e. TWEN=1
	 *3. Sends ACK bit i.e. TWEA=1*/
	TWCR=(1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	/*Busy wait until TWINT flag is set which means data byte is received successfully*/
	while(IS_BIT_CLEAR(TWCR,TWINT));
	return TWDR;
}

/******************************************************************************************
 *[Function Name]	: TWI_readWithNACK
 *[Description]		: This function reads a byte of data on SDA bus either it's byte/address
 *					  without sending ACK signal after it (NACK).
 *[Arguments]		: void
 *[Return]			: uint8
 *						returns the received byte of data
 ******************************************************************************************/
uint8 TWI_readWithNACK(void)
{
	/*1. Clear TWINT flag by setting it before sending
	 *2. Keep enabling TWI module i.e. TWEN=1
	 *3. Doesn't send ACK bit i.e. TWEA=0*/
	TWCR=(1<<TWINT) | (1<<TWEN);
	/*Busy wait until TWINT flag is set which means data byte is received successfully*/
	while(IS_BIT_CLEAR(TWCR,TWINT));
	return TWDR;
}

/******************************************************************************************
 *[Function Name]	: TWI_getStatus
 *[Description]		: This function gets the status of I2C module
 *[Arguments]		: void
 *[Return]			: uint8
 *						returns the status from status register 5 MSBs
 ******************************************************************************************/
uint8 TWI_getStatus(void)
{
	uint8 status;
	status=TWSR&0xF8;
	return status;
}
