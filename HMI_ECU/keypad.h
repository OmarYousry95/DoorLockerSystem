/*******************************************************************************************
 * [FILE NAME]:		keypad.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	29 Dec 2019
 * [DESCRIPTION]:	This header file contains the protoypes of functions of keypad driver
 * 					either it is 4x3 or 4x4
 *******************************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/******************************************************************
 * 				Common Header Files Inclusion					  *
 ******************************************************************/
#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"


/******************************************************************
 * 				    Static Configurations					      *
 ******************************************************************/
/*Macro to define the number of columns of keypad*/
#define N_COL					4
/*Macro to define the number of rows of keypad*/
#define N_ROW					4
/*Macro to define input port the keypad connected to*/
#define KEYPAD_PORT_IN 			PINA
/*Macro to define output port the keypad connected to*/
#define KEYPAD_PORT_OUT			PORTA
/*Macro to define direction port the keypad connected to*/
#define KEYPAD_PORT_DIRECTION 	DDRA


/******************************************************************
 * 				  Public Functions Prototypes					  *
 ******************************************************************/
/*[Function Name] : KEYPAD_getPressedKey
 *[Description]	  : This function gets the value of pressed key on keypad
 *[Arguments]     : void
 *[Return]        : uint8
 *					This function returns a uint8 variable holding data of
 *					pressed key*/
uint8 KEYPAD_getPressedKey (void);

#endif /* KEYPAD_H_ */
