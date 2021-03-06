/***********************************************************************************************
 * [FILE NAME]:		HMI_ECU.c
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	27 Jan 2020
 * [DESCRIPTION]:	This c file contains application code of Control ECU
 ***********************************************************************************************/

#include "Control_ECU.h"

/*Base address of memory where password is saved starting from it*/
uint16 eeprom_addr=0x0300;

/*Address of memory where the chkFlag is saved inside it to indicate if there is a saved
 *password or not*/
uint16 eeprom_flag=0x05FF;

/*Global array to hold the saved password from EEPROM in it*/
uint8 g_eeprom[PASSWORD_SIZE];

/*Global variable to hold the function ID being executed,
 *initially it's 0 to execute Control_checkForSavedPassword function*/
uint8 g_functionID=0;

/*Variable to hold state of system, either the password is entered correctly or not*/
uint8 g_state=0;

/*Configuration structure for Timer1 module:
 * 1. Compare match (CTC) mode
 * 2. Prescalar = 1024 --> T_Timer = 128 usec
 * 3. Initial value = 0
 * 4. Compare value = 39063 --> Interrupt every 5 sec*/
TIMER1_ConfigType Timer1_Config={CTC,F_CPU_1024,0,39063};

/*Global array of pointer to functions, each function is called in main function through its ID in array
 *and at the end of each function, the needed function is called through changing the global variable
 *holding ID
 *-------------------------------
 *[Function ID]	| [Function Name]
 *-------------------------------
 *		0		| Control_checkForSavedPassword
 *		1		| Control_setNewPassword
 *		2		| Control_checkNewPassword
 *		3		| Control_receiveAndCheckPassword
 *******************************************************************************************************/
void (*func[4])(void)={Control_checkForSavedPassword,Control_setNewPassword,Control_checkNewPassword,
		               Control_receiveAndCheckPassword};

int main()
{
	/*Enable global interrupts for timer1 to operate*/
	sei();
	/*Configuration structure for UART module:
	 * 1. Baud rate = 9600
	 * 2. No parity bits is used (parity is disabled)
	 * 3. One stop bit is used
	 * 4. Data frame is 8-bit data*/
	UART_ConfigType UART_Config={9600,NO_PARITY,ONE_STOP_BIT,EIGHT_BITS};

	/*Set callback function for Timer1*/
	TIMER1_setCallBack(fireBuzzerOrOpenDoor);
	/*Initialises UART module with UART_Config structure parameters*/
	UART_init(&UART_Config);

	/*Initialise EEPROM*/
	EEPROM_init();

	/*Set direction of motor pins to be output pins*/
	SET_BIT(DDRB,MOTOR_PIN1);
	SET_BIT(DDRB,MOTOR_PIN2);
	SET_BIT(DDRB,MOTOR_EN);
	/*Set direction of buzzer pin to be output pin*/
	SET_BIT(DDRB,BUZZER);

	while(1)
	{
		/*Call function through pointer to function from the array of pointers to functions*/
		(*func[g_functionID])();
	}
}

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
void Control_checkForSavedPassword(void)
{
	/*Variable to for loop till password size*/
	uint8 loop_idx=0;
	/*Variable to  save it in memory to indicate there is a saved password or not*/
	uint8 chkFlag;

	/*Wait until HMI ECU sends CHECK_FOR_SAVED_PASSWORD signal so as to begin checking*/
	while(UART_receiveByte() != CHECK_FOR_SAVED_PASSWORD);

	/*Check if there is a saved password in EEPROM or not*/
	/*Read from memory eeprom_flag address the value of saved flag*/
	EEPROM_readByte(eeprom_flag,&chkFlag);

	/*Check on the value of flag
	 * 1. if chkFlag = NO_PASSWORD value, go to Control_setNewPassword function
	 * 2. if chkFlag = PASSWORD_EXIST value, go to Control_receiveAndSavePassword function*/
	if(chkFlag == SAVED_PASSWORD)
	{
		/*Send to HMI ECU a SAVED_PASSWORD signal to indicate that a password is saved*/
		UART_sendByte(SAVED_PASSWORD);
		/*Read the saved password from EEPROM in g_eeprom array*/
		for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
		{
			EEPROM_readByte((eeprom_addr)+loop_idx,&g_eeprom[loop_idx]);
		}
		/*Go to Control_receiveAndSavePasswordfunction and wait for password to be checked
		 *with the saved one in EEPROM*/
		g_functionID=3;
	}
	if(chkFlag == NO_SAVED_PASSWORD)
	{
		/*Send to HMI ECU a NO_SAVED_PASSWORD signal to indicate no password is saved*/
		UART_sendByte(NO_SAVED_PASSWORD);
		/*Go to Control_setNewPassword function and wait for new password to be sent
		 *to save it in EEPROM*/
		g_functionID=1;
	}
}


/******************************************************************************
 *[Function Name] : Control_setNewPassword
 *[Description]   : This function sets a new password for the system
 *					It waits until HMI ECU sends a ready signal then it receives a password
 *					### g_functionID = 1 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void Control_setNewPassword(void)
{
	/*Variable to for loop till password size*/
	uint8 loop_idx=0;
	/*Wait until HMI ECU sends a ready signal*/
	while(UART_receiveByte()!=HMI_ECU_READY);
	/*Get the input password from HMI ECU in g_eeprom array*/
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		g_eeprom[loop_idx]=UART_receiveByte();
	}
	/*Go to Control_checkNewPassword function to check if the password is re-entered correctly or not*/
	g_functionID=2;
}

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
void Control_checkNewPassword(void)
{
	/*Variable to for loop till password size*/
	uint8 loop_idx=0;
	/*Variable used as a flag to check if password matches or not*/
	uint8 mismatch=0;
	/*Wait until HMI ECU sends a ready signal*/
	while(UART_receiveByte()!=HMI_ECU_READY);
	/*For loop to get a key by key and save it then compares at each time if each
	 *character matches or not*/
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		/*For each mismatching received character, increment the mismatch counter*/
		if(UART_receiveByte() != g_eeprom[loop_idx])
		{
			mismatch++;
		}
	}
	/*Checks for matching between two entered passwords*/
	if(mismatch==0)
	{
		/*Send a CORRECT_NEW_PASSWORD signal to HMI ECU to go to main menu options*/
		UART_sendByte(CORRECT_NEW_PASSWORD);
		/*Write the password to EEPROM*/
		for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
		{
			EEPROM_writeByte((eeprom_addr)+loop_idx,g_eeprom[loop_idx]);
			_delay_ms(10);
		}
		/*Write in the eeprom_flag address a PASSWROD_EXIST constant*/
		EEPROM_writeByte(eeprom_flag,SAVED_PASSWORD);
		_delay_ms(10);
		/*Go to Control_receiveAndCheckPassword function*/
		g_functionID=3;
	}
	else
	{
		/*Send a NON_CORRECT_NEW_PASSWORD signal to HMI ECU to ask user to re-enter password*/
		UART_sendByte(NON_CORRECT_NEW_PASSWORD);
		/*Go again to Control_setNewPassword function to save*/
		g_functionID=1;
	}
}

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
void Control_receiveAndCheckPassword(void)
{
	/*Variable to for loop till password size*/
	uint8 loop_idx=0;

	/*Local array to hold the value of entered password to check it with saved one*/
	uint8 chkPassword[PASSWORD_SIZE];

	/*Variable used as a flag for mismatches in received password*/
	uint8 mismatch=0;

	/*Variable to count trials of password entry
	 *It's static to keep the number of trials every time this function is called*/
	static uint8 trial=1;

	/*Busy-wait loop till receiving a ready signal from HMI ECU*/
	while(UART_receiveByte()!= HMI_ECU_READY);
	/*For loop to receive the password character by character and checks if there a mismatch with
	 *the saved one or not*/
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		chkPassword[loop_idx]=UART_receiveByte();
	}
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		EEPROM_readByte((eeprom_addr)+loop_idx,&g_eeprom[loop_idx]);
	}
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		SET_BIT(PORTA,7-loop_idx);
		if(chkPassword[loop_idx] != g_eeprom[loop_idx])
		{
			mismatch++;
		}
	}
	/*Get the option either to open the door or change password*/
	uint8 key = UART_receiveByte();
	/*If entered password matches the saved one*/
	if(mismatch == 0)
	{
		/*Send to HMI ECU that the password is entered correctly*/
		UART_sendByte(CORRECT_PASSWORD);
		/*Clear the state variable indicating that password is entered correctly*/
		g_state=0;
		/*Return number of trials to 1 again*/
		trial=1;
		/*If the option is open the door
		 * - Rotate the motor clockwise (open the door)
		 * - Send to HMI ECU that the door is unlocking now to display on LCD
		 * - Start the timer to count 15 seconds, when reaching 15 seconds, it goes to call back function
		 *   to rotate the motor anticlockwise for another 15 seconds then stops it*/
		if(key == OPEN_DOOR)
		{
			/*Open the door*/
			motorRotateClockwise();
			UART_sendByte(DOOR_UNLOCKING);
			TIMER1_init(&Timer1_Config);
		}
		/*If the option is change the password, go to Control_setNewPassword function*/
		if(key == CHANGE_PASSWORD)
		{
			g_functionID=1;
		}
	}
	/*If password is wrongly entered*/
	else
	{
		/*Set the state variable indicating that password is wrongly entered*/
		g_state=1;
		/*Increment number of trials*/
		trial++;
		/*Check on number of trials
		 *If it reaches 4, this means that user entered password wrongly 3 times*/
		if(trial==4)
		{
			/*Send to HMI ECU a thief signal*/
			UART_sendByte(THIEF);
			/*Fire the buzzer on*/
			buzzerON();
			/*Start the timer to count 1 min, when reaching 1 min, it goes to call back function to stop
			 *buzzer and send to HMI ECU that system is unlocked*/
			TIMER1_init(&Timer1_Config);
		}
		/*If number of trials is less than 4, this means that there still exists number of trials for
		 *the user to enter it*/
		else
		{
			/*Send to HMI ECU that wrong password is entered*/
			UART_sendByte(WRONG_PASSWORD);
		}
		/*Go again to Control_receiveAndCheckPassword function*/
		g_functionID=3;
	}
}


/******************************************************************************
 *[Function Name] : motorRotateClockwise
 *[Description]   : This function rotates the motor in clockwise direction
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void motorRotateClockwise (void)
{
	/*Set enable pin*/
	SET_BIT(PORTB,MOTOR_EN);
	/*Pin1 : 1
	 *Pin2 : 0 to rotate clockwise*/
	SET_BIT(PORTB,MOTOR_PIN1);
	CLEAR_BIT(PORTB,MOTOR_PIN2);
}

/******************************************************************************
 *[Function Name] : motorRotateAntiClockwise
 *[Description]   : This function rotates the motor in anti-clockwise direction
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void motorRotateAntiClockwise (void)
{
	/*Set enable pin*/
	SET_BIT(PORTB,MOTOR_EN);
	/*Pin1 : 0
	 *Pin2 : 1 to rotate anti-clockwise*/
	CLEAR_BIT(PORTB,MOTOR_PIN1);
	SET_BIT(PORTB,MOTOR_PIN2);
}

/******************************************************************************
 *[Function Name] : motorStop
 *[Description]   : This function stops the rotation of motor
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void motorStop(void)
{
	/*Clear Enable pin*/
	CLEAR_BIT(PORTB,MOTOR_EN);
	/*Clear 2 motor pins*/
	CLEAR_BIT(PORTB,MOTOR_PIN1);
	CLEAR_BIT(PORTB,MOTOR_PIN2);
}

/******************************************************************************
 *[Function Name] : buzzerON
 *[Description]   : This function fires the buzzer ON
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void buzzerON (void)
{
	/*Fire the buzzer by setting buzzer bit*/
	SET_BIT(PORTB,BUZZER);
}

/******************************************************************************
 *[Function Name] : buzzerON
 *[Description]   : This function turns the buzzer OFF
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void buzzerOFF (void)
{
	/*Stop the buzzer by clearing buzzer bit*/
	CLEAR_BIT(PORTB,BUZZER);
}

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
void fireBuzzerOrOpenDoor(void)
{
	/*Static variable to hold the number of times the system reaches to this function*/
	static uint8 count=0;
	/*Increment the count by 1 indicating a system reach to this function*/
	count++;
	/*When count=3, this means 15 sec time
	 *When g_state=0, this means password is entered correctly*/
	if(count==3 && g_state==0)
	{
		/*Close the door*/
		motorRotateAntiClockwise();
		/*Send to HMI ECU that the door is being locked now to display on screen*/
		UART_sendByte(DOOR_LOCKING);
	}
	/*When count=6, this means 30 sec time
	 *When g_state=0, this means password is entered correctly*/
	if(count==6 && g_state==0)
	{
		/*Lock the door*/
		motorStop();
		/*Send to HMI ECU that the door is locked now to return back to main menu*/
		UART_sendByte(DOOR_LOCKED);
		/*Return the counter back to zero for further usages*/
		count=0;
		/*Stop Timer1*/
		TIMER1_DeInit();
		/*Go again to Control_receiveAndCheckPassword function*/
		g_functionID=3;
	}
	/*When count=12, this means 60 sec time i.e. 1 min
	 *When g_state=1, this means that password is not entered correctly*/
	if(count==12 && g_state==1)
	{
		/*Stops the buzzer after 1 min of buzzerON*/
		buzzerOFF();
		/*Send to HMI ECU that is system is unlocked again to return back to main menu*/
		UART_sendByte(SYSTEM_UNLOCKED);
		/*Return the counter back to zero for further usages*/
		count=0;
		/*Stop Timer1*/
		TIMER1_DeInit();
	}
}
