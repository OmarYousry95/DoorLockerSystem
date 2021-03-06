/*******************************************************************************************
 * [FILE NAME]:		uart.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	20 Jan 2020
 * [DESCRIPTION]:	This header file contains static configurations and function prototypes
 * 					for UART Module in AVR ATMEGA-16 Micro-controller
 *******************************************************************************************/
#ifndef UART_H_
#define UART_H_

/******************************************************************
 * 				Common Header Files Inclusion					  *
 ******************************************************************/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/******************************************************************
 * 				  		    MACROS					      		  *
 ******************************************************************/
/*Macro to calculate UBBR register value according to a given Baud Rate*/
#define BAUD_RATE(BAUD)	(((F_CPU/(8*BAUD))-1))
/*Macros for defining both modes of UART module*/
#define UART_INTERRUPT	1
#define UART_POLLING	0
/*Macro definition for UART module mode either it's operating with polling/interrupt*/
#define UART_MODE		UART_POLLING

/******************************************************************
 * 						Global Variables					 	  *
 ******************************************************************/
#if(UART_MODE == UART_INTERRUPT)
/*Extern for (g_RxData) Global variable to hold byte to be used in
 *receiveByte API in case of receiving with interrupts*/
extern volatile uint8 g_RxData;
#endif
/******************************************************************
 * 				    User-defined Data Types					      *
 ******************************************************************/
/*[ENUM Name]		: UART_Parity
 *[ENUM Description]: This enum contains values for various parity types
 *					  either it's disabled, even or odd*/
typedef enum{
	NO_PARITY,EVEN_PARITY=2,ODD_PARITY
}UART_Parity;

/*[ENUM Name]		: UART_StopBit
 *[ENUM Description]: This enum contains 2 values for stop bits either 1 or 2*/
typedef enum{
	ONE_STOP_BIT,TWO_STOP_BITS
}UART_StopBit;

/*[ENUM Name]		: UART_CharacterSize
 *[ENUM Description]: This enum contains values of allowed data frame from 5-9 bits*/
typedef enum{
	FIVE_BITS,SIX_BITS,SEVEN_BITS,EIGHT_BITS
}UART_CharacterSize;

/*[Structure Name]		 : UART_ConfigType
 *[Structure Description]: This structure contains configuration parameters for UART
 *						   module: 1. Desired baud rate
 * 								   2. Desired parity type (even/odd/disabled)
 * 								   3. Desired number of stop bits (1/2)
 * 								   4. Desired data frame i.e.Number of data bits (5/6/7/8/9)*/
typedef struct{
	/*32-bit unsigned variable to hold supported baud rate values from 100-115200 bps*/
	uint32 baudRate;
	/*enum from UART_Parity type to hold desired parity type*/
	UART_Parity parity;
	/*enum from UART_StopBit type to hold desired number of stop bits*/
	UART_StopBit stopBit;
	/*enum from UART_CharacterSize type to hold desired data frame number of bits*/
	UART_CharacterSize characterSize;
}UART_ConfigType;

/******************************************************************
 * 				  Public Functions Prototypes					  *
 ******************************************************************/
/*********************************************************************************
 * [Function Name]	: UART_init
 * [Description]	: This function initialises UART Module:
 * 						1. Enables UART Module
 * 						2. Sets baud rate according to desired rate
 * 						3. Determines frame shape:
 * 						   a. Parity type (even/odd/disabled)
 * 						   b. Number of stop bit(s) (1/2)
 * 						   c. Number of data bits (5/6/7/8)
 * [Arguments]		: const UART_ConfigType * Config_Ptr
 * 						This is a pointer to structure shall be passed by address
 * 						having all the data to initialise UART module with
 * [Return]			: void
 ***********************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr);

/*********************************************************************************
 * [Function Name]	: UART_sendByte
 * [Description]	: This function sends a byte of data by UART protocol using polling
 * [Arguments]		: const uint16 data
 * 						This is a uint16 variable having the desired byte to be sent
 * 						uint16 to be able to hold 9-bits frame in case of 9-bits
 * 						const, so as to not be modified by this function
 * [Return]			: void
 ***********************************************************************************/
void UART_sendByte(const uint8 data);


#if(UART_MODE == UART_POLLING)
/*********************************************************************************
 * [Function Name]	: UART_receiveByte
 * [Description]	: This function receives a byte of data by UART protocol in case
 * 					  of polling
 * [Arguments]		: No input arguments
 * [Return]			: uint16 to hold value of received byte
 * 					  it's of 16-bit size to be able to hold 9-bits data in case of 9-bit
 * 					  data frame
 ***********************************************************************************/
uint8 UART_receiveByte(void);
#endif


/*********************************************************************************
 * [Function Name]	: UART_sendString
 * [Description]	: This function sends a string of data by UART protocol using polling
 * [Arguments]		: const uint8 * str
 * 						This is a pointer to first element of string to be sent
 * 						const, so as to not be modified by this function
 * [Return]			: void
 ***********************************************************************************/
void UART_sendString(const uint8 * str);

#if(UART_MODE == UART_POLLING)
/*********************************************************************************
 * [Function Name]	: UART_receiveString
 * [Description]	: This function receives a string of data by UART protocol using
 * 					  polling
 * [Arguments]		: uint8 * str
 * 						This is a pointer to empty array to receive the string in
 * [Return]			: void
 ***********************************************************************************/
void UART_receiveString(uint8 * str);
#endif

#if(UART_MODE == UART_INTERRUPT)
/*********************************************************************************
 * [Function Name]	: UART_setCallBack
 * [Description]	: This function sets the callback function of UART module when
 * 					  receiving is done
 * [Arguments]		: void(*a_ptr)(void)
 * 						This is a pointer to a function that takes void and returns void
 * 						to hold the address of function to be called when receiving data is
 * 						done
 * [Return]			: void
 ***********************************************************************************/
void UART_setCallBack (void(*a_ptr)(void));
#endif

#endif /* UART_H_ */
