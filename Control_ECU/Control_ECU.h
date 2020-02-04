/***********************************************************************************************
 * [FILE NAME]:		Control_ECU.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	27 Jan 2020
 * [DESCRIPTION]:	This header file contains configurations and file inclusions of Control_ECU
 ***********************************************************************************************/

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

/******************************************************************
 * 					  Header Files Inclusion					  *
 ******************************************************************/
#include "uart.h"
#include "timer1.h"
#include "external_eeprom.h"


/******************************************************************
 * 				  			  Macros					          *
 ******************************************************************/
#define PASSWORD_SIZE				5u
/*Signals to communicate between two ECUs*/
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
#define MOTOR_CLK_STATE				0x26
#define MOTOR_ANTI_CLK_STATE		0x27
#define BUZZER_STATE				0x28
/*Static Configuration for motor and buzzer pins*/
#define MOTOR_PIN1		PB0
#define MOTOR_PIN2		PB1
#define MOTOR_EN		PB2
#define BUZZER			PB7

/******************************************************************
 * 				    Public Functions Prototypes					  *
 ******************************************************************/
/******************************************************************************
 *[Function Name] : Control_checkForSavedPassword
 *[Description]   : This function is the first function to run in Control ECU:
 *					1. It waits until HMI ECU sends a signal for checking if there exists a previously
 *					   saved password or not
 *					2. It checks if there is a saved password in EEPROM or not
 *					3. If not exist, send a NO_SAVED_PASSWORD signal to HMI ECU
 *					4. If exists, send a SAVED_PASSWORD signal to HMI ECU
 *					### g_functionID = 0 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void Control_checkForSavedPassword(void);

/******************************************************************************
 *[Function Name] : Control_setNewPassword
 *[Description]   : This function sets a new password for the system
 *					It waits until HMI ECU sends a ready signal then it receives a password
 *					### g_functionID = 1 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void Control_setNewPassword(void);

/******************************************************************************
 *[Function Name] : Control_checkNewPassword
 *[Description]   : This function checks the new entered password for the system
 *					1. It waits until HMI ECU sends a ready signal then it receives a password
 *					2. It compares each element of the password to the previously received one
 *					3. If it matches, it sends a CORRECT_NEW_PASSWORD signal to HMI ECU to
 *					   go to main menu options and also goes to wait function
 *					4. It it doesn't match, it Send a NON_CORRECT_NEW_PASSWORD signal to
 *					   HMI ECU to ask user to re-enter password and goes again to Control_setNewPassword
 *					   function
 *					### g_functionID = 2 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void Control_checkNewPassword(void);

/******************************************************************************
 *[Function Name] : Control_receiveAndCheckPassword
 *[Description]   : This function receives a password from HMI ECU and compares it with saved one
 *					1. If it matches, it sends to HMI ECU that door is unlocking then door is locking
 *					2. If it doesn't match, it sends to HMI to ask the user to enter it one more time
 *					   Total number of trials = 3
 *					3. If after 3 trials the password doesn't match at any time, it informs the HMI
 *					   ECU to view a thief message and lock the system for 1 minute
 *					### g_functionID = 3 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void Control_receiveAndCheckPassword(void);

/******************************************************************************
 *[Function Name] : motorRotateClockwise
 *[Description]   : This function rotates the motor in clockwise direction
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void motorRotateClockwise (void);

/******************************************************************************
 *[Function Name] : motorRotateAntiClockwise
 *[Description]   : This function rotates the motor in anti-clockwise direction
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void motorRotateAntiClockwise (void);

/******************************************************************************
 *[Function Name] : motorStop
 *[Description]   : This function stops the rotation of motor
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void motorStop(void);

/******************************************************************************
 *[Function Name] : buzzerON
 *[Description]   : This function fires the buzzer ON
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void buzzerON (void);

/******************************************************************************
 *[Function Name] : buzzerON
 *[Description]   : This function turns the buzzer OFF
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void buzzerOFF (void);


/******************************************************************************
 *[Function Name] : fireBuzzerOrOpenDoor
 *[Description]   : This function is the called back function by Timer1 when it reaches 5 seconds
 *					1. After the first 3 (5 seconds) i.e. 15 seconds --> rotate motor in anti-clkwise
 *					   direction
 *					2. After the second 3 (5 seconds) i.e. 30 seconds --> stop motor rotation
 *					3. When it reaches a minute, stops the buzzer
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void fireBuzzerOrOpenDoor(void);

#endif /* CONTROL_ECU_H_ */
