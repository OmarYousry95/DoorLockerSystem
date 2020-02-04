/***********************************************************************************************
 * [FILE NAME]:		keypad.c
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	29 Dec 2019
 * [DESCRIPTION]:	This c file contains the functions of keypad driver either it is 4x3 or 4x4
 ***********************************************************************************************/

#include "keypad.h"

/******************************************************************
 * 				  Private Functions Prototypes					  *
 ******************************************************************/
#if (N_COL == 3)
/*[Function Name] : KEYPAD_4x3_adjustKeyNumber
 *[Description]	  : This function to map the switch number in the 4x3 keypad to its functional
 *					number on proteus 4x3 keypad
 *[Arguments]     : uint8 button_number
 *						This is the input number of pressed button in 4x3 keypad
 *[Return]        : uint8
 *						This function returns a uint8 variable holding functional key
 *						of input number*/
static uint8 KEYPAD_4x3_adjustKeyNumber (uint8 button_number);

#elif (N_COL == 4)
/*[Function Name] : KEYPAD_4x4_adjustKeyNumber
 *[Description]	  : This function to map the switch number in the 4x3 keypad to its functional
 *					number on proteus 4x4 keypad
 *[Arguments]     : uint8 button_number
 *						This is the input number of pressed button in 4x3 keypad
 *[Return]        : uint8
 *						This function returns a uint8 variable holding functional key
 *						of input number*/
static uint8 KEYPAD_4x4_adjustKeyNumber (uint8 button_number);
#endif

/******************************************************************
 * 				  	  Functions Definitions				 		  *
 ******************************************************************/
/*[Function Name] : KEYPAD_getPressedKey
 *[Description]	  : This function gets the value of pressed key on keypad
 *[Arguments]     : void
 *[Return]        : uint8
 *					This function returns a uint8 variable holding data of
 *					pressed key*/
uint8 KEYPAD_getPressedKey (void)
{
	uint8 col,row;
	while(1)
	{
		for(col=0;col<4;col++)
		{
			/*Each loop, set a column to be output at a time
			 *and clear the first 4 pins to be input pins for the rows*/
			KEYPAD_PORT_DIRECTION = (0b00010000<<col);
			/*Each loop, column output is 0 at a time
			 *and set the first 4 pins to use internal pull-up resistor for each row*/
			KEYPAD_PORT_OUT = ~(0b00010000<<col);
			for(row=0;row<N_ROW;row++)
			{
				if(IS_BIT_CLEAR(KEYPAD_PORT_IN,row))
				{
					#if (N_COL == 3)
						return KEYPAD_4x3_adjustKeyNumber((row*N_COL)+col+1);
					#elif (N_COL == 4)
						return KEYPAD_4x4_adjustKeyNumber((row*N_COL)+col+1);
					#endif
				}
			}
		}
	}
}

#if (N_COL == 3)
/*[Function Name] : KEYPAD_4x3_adjustKeyNumber
 *[Description]	  : This function to map the switch number in the 4x3 keypad to its functional
 *					number on proteus 4x3 keypad
 *[Arguments]     : uint8 button_number
 *						This is the input number of pressed button in 4x3 keypad
 *[Return]        : uint8
 *						This function returns a uint8 variable holding functional key
 *						of input number*/
static uint8 KEYPAD_4x3_adjustKeyNumber (uint8 button_number)
{
	switch (button_number)
	{
		case 10:return '*'; /*ASCII of '*'*/
				break;
		case 11:return 0;
				break;
		case 12:return '#'; /*ASCII of '#'*/
				break;
		default: return button_number;
	}
}

#elif(N_COL == 4)
/*[Function Name] : KEYPAD_4x4_adjustKeyNumber
 *[Description]	  : This function to map the switch number in the 4x3 keypad to its functional
 *					number on proteus 4x4 keypad
 *[Arguments]     : uint8 button_number
 *						This is the input number of pressed button in 4x3 keypad
 *[Return]        : uint8
 *						This function returns a uint8 variable holding functional key
 *						of input number*/
static uint8 KEYPAD_4x4_adjustKeyNumber (uint8 button_number)
{
	switch (button_number)
	{
		case 1:	return 7;
				break;
		case 2:	return 8;
				break;
		case 3:	return 9;
				break;
		case 4:	return '%'; /*ASCII of '%'*/
				break;
		case 5:	return 4;
				break;
		case 6:	return 5;
				break;
		case 7:	return 6;
				break;
		case 8:	return '*'; /*ASCII of '*'*/
				break;
		case 9:	return 1;
				break;
		case 10:return 2;
				break;
		case 11:return 3;
				break;
		case 12:return '-'; /*ASCII of '-'*/
				break;
		case 13:return 13;
				break;
		case 14:return 0;
				break;
		case 15:return '='; /*ASCII of '='*/
				break;
		case 16:return '+'; /*ASCII of '+'*/
				break;
		default: return button_number;
	}
}
#endif

