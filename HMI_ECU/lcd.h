/*******************************************************************************************
 * [FILE NAME]:		lcd.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	29 Dec 2019
 * [DESCRIPTION]:	This header file contains the protoypes of functions of LCD driver
 * 					either it is 2-line or 4-line for 4-bit mode or 8-bit mode
 *******************************************************************************************/

#ifndef LCD_H_
#define LCD_H_

/******************************************************************
 * 				Common Header Files Inclusion					  *
 ******************************************************************/
#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"


/******************************************************************
 * 				    Static Configurations					      *
 ******************************************************************/
/* LCD Data bits mode configuration */
#define DATA_BITS_MODE 8

/* Use higher 4 bits in the data port */
#if (DATA_BITS_MODE == 4)
#define UPPER_PORT_PINS
#endif

/*LCD Hardware Pins*/
#define LCD_DATA_DIR	DDRC
#define	LCD_DATA_PORT	PORTC
#define LCD_CTRL_DIR	DDRD
#define LCD_CTRL_PORT	PORTD
#define RS				PD4
#define RW				PD5
#define E				PD6

/*LCD commands*/
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define TWO_LINE_LCD_Four_BIT_MODE 	0x28
#define CURSOR_OFF 					0x0C
#define CURSOR_ON 					0x0E
#define CLEAR_COMMAND 				0x01
#define SET_CURSOR_LOCATION 		0x80

/******************************************************************
 * 				  Public Functions Prototypes					  *
 ******************************************************************/
/*[Function Name] : LCD_init
 *[Description]	  : This function initialises LCD (set direction for data and control ports
 *					and clears screen
 *[Arguments]     : void
 *[Return]        : void*/
void LCD_init(void);

/*[Function Name] : LCD_sendCommand
 *[Description]	  : This function sends a command over data port for LCD
 *[Arguments]     : uint8 command
 *						This uint8 variable holds a certain command to do
 *[Return]        : void*/
void LCD_sendCommand (uint8 command);

/*[Function Name] : LCD_displayCharacter
 *[Description]	  : This function displays a character on LCD screen
 *[Arguments]     : uint8 data
 *						This uint8 variable holds a character to be displayed on LCD
 *[Return]        : void*/
void LCD_displayCharacter (uint8 data);

/*[Function Name] : LCD_displayString
 *[Description]	  : This function displays a string on LCD screen
 *[Arguments]     : const char * data
 *						This pointer to char holds the first char in string to be displayed on LCD
 *[Return]        : void*/
void LCD_displayString (const char * data);

/*[Function Name] : LCD_clearScreen
 *[Description]	  : This function clears LCD screen and gets cursor on first place in first row
 *[Arguments]     : void
 *[Return]        : void*/
void LCD_clearScreen (void);

/*[Function Name] : LCD_displayStringRowColumn
 *[Description]	  : This function displays string on certain place (row&column) on LCD
 *[Arguments]     : uint8 row
 *						This uint8 variable holds row place of beginning of string
 *					uint8 col
 *						This uint8 variable holds column place of beginning of string
 *					const char * data
 *						This pointer to char holds the first char in string to be displayed on LCD
 *[Return]        : void*/
void LCD_displayStringRowColumn (uint8 row, uint8 col, const char * data);

/*[Function Name] : LCD_goToRowColumn
 *[Description]	  : This function moves LCD's cursor to a certain place (row&column)
 *[Arguments]     : uint8 row
 *						This uint8 variable holds row place cursor will move to
 *					uint8 col
 *						This uint8 variable holds column place cursor will move to
 *[Return]        : void*/
void LCD_goToRowColumn (uint8 row,uint8 col);

/*[Function Name] : LCD_intgerToString
 *[Description]	  : This function displays an integer on string by converting the number to character
 *[Arguments]     : int data
 *						This int variable holds integer to be converted to character
 *[Return]        : void*/
void LCD_intgerToString (int data);

#endif /* LCD_H_ */
