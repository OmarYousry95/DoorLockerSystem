/*******************************************************************************************
 * [FILE NAME]:		i2c.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	30 Jan 2020
 * [DESCRIPTION]:	This header file contains static configurations and function prototypes
 * 					for I2C (TWI) Module in AVR ATMEGA-16 Micro-controller
 *******************************************************************************************/

#ifndef I2C_H_
#define I2C_H_

/******************************************************************
 * 				Common Header Files Inclusion					  *
 ******************************************************************/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/******************************************************************
 * 				   			 Macros					      		  *
 ******************************************************************/
#define TWI_START			0x08 /*A START condition has been transmitted*/
#define TWI_REP_START		0x10 /*A repeated START condition has been transmitted*/
#define TWI_MT_SLA_W_ACK	0x18 /*Master Transmit (Slave Address + Write) + ACK from slave*/
#define TWI_MT_SLA_W_NACK	0x20 /*Master Transmit (Slave Address + Write) + NACK from slave*/
#define TWI_MT_SLA_R_ACK	0x40 /*Master Transmit (Slave Address + Read) + ACK from slave*/
#define TWI_MT_SLA_R_NACK	0x48 /*Master Transmit (Slave Address + Read) + NACK from slave*/
#define TWI_MT_DATA_ACK		0x28 /*Master Transmit Data + ACK from slave*/
#define TWI_MT_DATA_NACK	0x30 /*Master Transmit Data + NACK from slave*/
#define TWI_MR_DATA_ACK		0x50 /*Master Receive Data + Sends ACK to slave*/
#define TWI_MR_DATA_NACK	0x58 /*Master Receive Data + Don't Send ACK to slave (NACK)*/

/*Macro to calculate the TWBR register value given: SCL frequency and prescaler*/
#define TWBR_VAL(F_SCL,PRESCALER)	((((F_CPU)/(F_SCL))-16)/(2*(PRESCALER)))

#define Mbps	1000000		/*Mega is 10^6*/
#define kbps	1000		/*Kilo is 10^3*/

/******************************************************************
 * 				    User-defined Data Types					      *
 ******************************************************************/
typedef struct{
	uint8 address;
	uint8 prescaler;
	uint32 SCL_Freq;
}I2C_ConfigType;

/******************************************************************
 * 				  Public Functions Prototypes					  *
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
void TWI_init(const I2C_ConfigType * Config_Ptr);

/*******************************************************************************************
 *[Function Name]	: TWI_start
 *[Description]		: This function sends a start bit on SDA bus to start and own the bus
 *[Arguments]		: void
 *[Return]			: void
 *******************************************************************************************/
void TWI_start(void);

/******************************************************************************************
 *[Function Name]	: TWI_stop
 *[Description]		: This function sends a stop bit on SDA bus to indicate stop of sending
 *[Arguments]		: void
 *[Return]			: void
 ******************************************************************************************/
void TWI_stop(void);

/******************************************************************************************
 *[Function Name]	: TWI_write
 *[Description]		: This function sends a byte of data on SDA bus either it's byte/address
 *[Arguments]		: const uint8 data
 *						- This is a variable having the byte of data to be sent
 *						- It's const so as dunction don't corrupt it
 *[Return]			: void
 ******************************************************************************************/
void TWI_write(const uint8 data);

/******************************************************************************************
 *[Function Name]	: TWI_readWithACK
 *[Description]		: This function reads a byte of data on SDA bus either it's byte/address
 *					  with sending ACK signal after it.
 *[Arguments]		: void
 *[Return]			: uint8
 *						returns the received byte of data
 ******************************************************************************************/
uint8 TWI_readWithACK(void);

/******************************************************************************************
 *[Function Name]	: TWI_readWithNACK
 *[Description]		: This function reads a byte of data on SDA bus either it's byte/address
 *					  without sending ACK signal after it (NACK).
 *[Arguments]		: void
 *[Return]			: uint8
 *						returns the received byte of data
 ******************************************************************************************/
uint8 TWI_readWithNACK(void);

/******************************************************************************************
 *[Function Name]	: TWI_getStatus
 *[Description]		: This function gets the status of I2C module
 *[Arguments]		: void
 *[Return]			: uint8
 *						returns the status from status register 5 MSBs
 ******************************************************************************************/
uint8 TWI_getStatus(void);

#endif /* I2C_H_ */
