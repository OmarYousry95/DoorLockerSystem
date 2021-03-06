/***********************************************************************************************
 * [FILE NAME]:		HMI_ECU.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	27 Jan 2020
 * [DESCRIPTION]:	This header file contains configurations and file inclusions of HMI_ECU
 ***********************************************************************************************/

#ifndef HMI_ECU_H_
#define HMI_ECU_H_

/******************************************************************
 * 					  Header Files Inclusion					  *
 ******************************************************************/
#include "lcd.h"
#include "keypad.h"
#include "uart.h"

/******************************************************************
 * 				  			  Macros					          *
 ******************************************************************/
#define PASSWORD_SIZE				5u
#define HMI_ECU_READY				0x10
#define CHECK_FOR_SAVED_PASSWORD	0x11
#define NO_SAVED_PASSWORD			0x12
#define SAVED_PASSWORD				0x13
#define CORRECT_NEW_PASSWORD		0x14
#define NON_CORRECT_NEW_PASSWORD	0x15
#define CORRECT_PASSWORD			0x16
#define WRONG_PASSWORD 				0x17
#define THIEF						0x18
#define SYSTEM_UNLOCKED 			0x19
#define DOOR_UNLOCKING				0x20
#define DOOR_LOCKING				0x21
#define DOOR_LOCKED					0x22
#define OPEN_DOOR					0x23
#define CHANGE_PASSWORD				0x24
#define CHECK_DONE					0x25
#define KEY_RECEIVED				0x30
/******************************************************************
 * 				    Public Functions Prototypes					  *
 ******************************************************************/
/******************************************************************************
 *[Function Name] : HMI_welcome
 *[Description]   : This function is the first function to run in HMI ECU:
 *					1. Displays "Door Locker" statement on LCD first row
 *					2. Displays "Press ON to cont." statement on LCD second row
 *					3. Wait until user presses ON key from keypad
 *					4. Then, it clears LCD for other menus to be displayed
 *					5. Waits until Control ECU checks if there is a previously saved password or not
 *					6. If there is a previously saved password, it will take the user to main menu
 *					   to select what to do (open the door/change the password)
 *					7. If there is a previously saved password, it will take the user to set new
 *					   password menu
 *					### g_functionID = 0 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void HMI_welcome(void);

/******************************************************************************
 *[Function Name] : HMI_setNewPassword
 *[Description]   : This function is responsible for sending to Control ECU to set a new
 *					password for the door locker system, then moves execution to HMI_checkNewPassword
 *					function to check entered password is same or not
 *					### g_functionID = 1 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void HMI_setNewPassword(void);

/******************************************************************************
 *[Function Name] : HMI_checkNewPassword
 *[Description]   : This function is responsible for sending to Control ECU to another copy of entered
 *					password for the door locker system to be checked
 *					1. In case of matching between 2 entered passwords:
 *						Moves execution to HMI_mainMenu for user to select what to do
 *				    2. In case of not matching between 2 entered passwords:
 *				    	Moves execution to the HMI_setNewPassword again
 *					### g_functionID = 2 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void HMI_checkNewPassword(void);

/******************************************************************************
 *[Function Name] : HMI_mainMenu
 *[Description]   : This function is the main interface of application, it asks
 *                  the user to choose one option:
 *                  1. Open the door by pressing '+' on keypad
 *                  2. Change the old password by pressing '-' on keypad
 *					### g_functionID = 3 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void HMI_mainMenu(void);

/******************************************************************************
 *[Function Name] : HMI_enterPassword
 *[Description]   : This function asks the user to enter the password for open the door
 *					option and sends it to Control ECU to be checked
 *                  1. If it matches with saved password, "Unlocking door.." statement is displayed
 *                     on LCD while it's unlocking and then displays "Locking door.." statement till
 *                     it's locked. Then it goes to HMI_mainMenu function
 *                  2. If it doesn't match, it asks the user to enter the password 2 more times
 *                     At any time of them, if it's entered correctly it will execute step(1)
 *                     If the user failed 3 times to enter the password, "THIEF!!" message is displayed
 *                     on LCD until the system is unlocked again.
 *					### g_functionID = 4 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void HMI_enterPassword(void);

/******************************************************************************
 *[Function Name] : HMI_enterOldPassword
 *[Description]   : This function asks the user to enter the password for change the password
 *					option and sends it to Control ECU to be checked
 *                  1. If it matches with saved password, system goes to HMI_setNewPassword function
 *                  2. If it doesn't match, it asks the user to enter the password 2 more times
 *                     At any time of them, if it's entered correctly it will execute step(1)
 *                     If the user failed 3 times to enter the password, "THIEF!!" message is displayed
 *                     on LCD until the system is unlocked again.
 *					### g_functionID = 5 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void HMI_enterOldPassword(void);

#endif /* HMI_ECU_H_ */
