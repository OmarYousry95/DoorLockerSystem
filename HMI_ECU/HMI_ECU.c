/***********************************************************************************************
 * [FILE NAME]:		HMI_ECU.c
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	27 Jan 2020
 * [DESCRIPTION]:	This c file contains application code of HMI ECU
 ***********************************************************************************************/

#include "HMI_ECU.h"

/*Global array of pointer to functions, each function is called in main function through its ID in array
 *and at the end of each function, the needed function is called through changing the global variable
 *holding ID
 *-------------------------------
 *[Function ID]	| [Function Name]
 *-------------------------------
 *		0		| HMI_welcome
 *		1		| HMI_setNewPassword
 *		2		| HMI_checkNewPassword
 *		3		| HMI_mainMenu
 *		4		| HMI_enterPassword
 *		5		| HMI_enterOldPassword
 *******************************************************************************************************/
void (*func[6])(void)={HMI_welcome,HMI_setNewPassword,HMI_checkNewPassword,HMI_mainMenu,
					   HMI_enterPassword,HMI_enterOldPassword};

/*Global variable to hold the function ID being executed,
 *initially it's 0 to execute HMI_welcome function*/
uint8 g_functionID=0;

int main()
{
	/*Initialises LCD*/
	LCD_init();
	/*Configuration structure for UART module:
	 * 1. Baud rate = 9600
	 * 2. No parity bits is used (parity is disabled)
	 * 3. One stop bit is used
	 * 4. Data frame is 8-bit data*/
	UART_ConfigType UART_Config={9600,NO_PARITY,ONE_STOP_BIT,EIGHT_BITS};
	/*Initialises UART module with UART_Config structure parameters*/
	UART_init(&UART_Config);
	while(1)
	{
		/*Call function through pointer to function from the array of pointers to functions*/
		(*func[g_functionID])();
	}
}

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
void HMI_welcome(void)
{
	/*Display welcome message on LCD screen*/
	LCD_displayStringRowColumn(0,3,"Door Locker");
	/*Display a message for user to press ON to continue to other screens*/
	LCD_displayStringRowColumn(1,0,"Press ON to cont");
	/*Busy-wait till ON key is pressed in keypad*/
	while(KEYPAD_getPressedKey() != 13);
	/*Delay for the press time*/
	_delay_ms(300);
	/*Clears screen for further options to be displayed*/
	LCD_clearScreen();
	/*Check for password from Control ECU, if there is a saved password go to HMI_mainMenu function
	 *if not, go to HMI_setNewPassword function*/
	UART_sendByte(CHECK_FOR_SAVED_PASSWORD);
	if(UART_receiveByte() == NO_SAVED_PASSWORD)
	{
		/*Go to HMI_setNewPassword function to set a new password*/
		g_functionID=1;

	}
	else
	{
		/*Go to HMI_mainMenu function to select what to do*/
		g_functionID=3;

	}
}


/******************************************************************************
 *[Function Name] : HMI_setNewPassword
 *[Description]   : This function is responsible for sending to Control ECU to set a new
 *					password for the door locker system, then moves execution to HMI_checkNewPassword
 *					function to check entered password is same or not
 *					### g_functionID = 1 ###
 *[Arguments]     : void
 *[Return]        : void
 ******************************************************************************/
void HMI_setNewPassword(void)
{
	/*Send a ready signal to Control ECU to be ready to receive the new password*/
	UART_sendByte(HMI_ECU_READY);
	/*Variable to for loop till password size*/
	uint8 loop_idx=0;
	/*Display a message for user to set new password*/
	LCD_displayStringRowColumn(0,0,"Set new password");
	/*Move cursor to second row first place to display pressed key as '*'*/
	LCD_goToRowColumn(1,0);
	/*For loop from 0 --> password size
	 *Each loop, send the pressed key to Control ECU*/
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		/*Send pressed key to Control ECU*/
		UART_sendByte(KEYPAD_getPressedKey());
		/*Display * on LCD for each pressed key*/
		LCD_displayCharacter('*');
		/*Delay for press time*/
		_delay_ms(500);
	}
	/*Go to HMI_checkNewPassword function to ask user to re-enter password*/
	g_functionID=2;
	/*Clears screen for the coming screen on LCD*/
	LCD_clearScreen();
}

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
void HMI_checkNewPassword(void)
{
	/*Send a ready signal to Control ECU to be ready to receive the new password*/
	UART_sendByte(HMI_ECU_READY);
	/*Variable to for loop till password size*/
	uint8 loop_idx=0;
	/*Display a message for user to reenter password*/
	LCD_displayStringRowColumn(0,0,"Reenter password");
	/*Move cursor to second row first place to display pressed key as '*'*/
	LCD_goToRowColumn(1,0);
	/*For loop from 0 --> password size
	 *Each loop, send the pressed key to Control ECU*/
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		/*Send pressed key to Control ECU*/
		UART_sendByte(KEYPAD_getPressedKey());
		/*Display * on LCD for each pressed key*/
		LCD_displayCharacter('*');
		/*Delay for press time*/
		_delay_ms(500);
	}
	/*Clears the screen for coming screens on LCD*/
	LCD_clearScreen();
	/*Check if the same password is entered in both times
	 * 1. If received signal from Control_ECU indicates a correct compare match, go to main menu
	 * 2. If received signal from Control_ECU indicates a non-correct compare match, display
	 *	  "wrong password" message and goes again to set a new password screen*/
	uint8 flag=UART_receiveByte();
	if(flag == CORRECT_NEW_PASSWORD)
	{
		/*Go to HMI_mainMenu*/
		g_functionID=3;
	}
	else
	{
		/*Display "Wrong Password!" message on LCD*/
		LCD_displayStringRowColumn(0,0,"Wrong Password!");
		/*Delay to display this message*/
		_delay_ms(300);
		/*Clears the screen for coming screens on LCD*/
		LCD_clearScreen();
		/*Go again to HMI_setNewPassword function to enter a new password*/
		g_functionID=1;
	}
}

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
void HMI_mainMenu(void)
{
	/*Display on LCD the two options for user
	 * 1. Open the door
	 * 2. Change the password*/
	LCD_displayStringRowColumn(0,0,"(+) Open Door");
	LCD_displayStringRowColumn(1,0,"(-) Change Pass");

	/*Check for pressed key if it's '+' or '-'*/
	if(KEYPAD_getPressedKey() == '+')
	{
		/*Press time delay*/
		_delay_ms(500);
		/*Send an OPEN_DOOR signal to Control ECU to inform it that option 1 is selected*/
		UART_sendByte(OPEN_DOOR);
		/*Go to HMI_enterPassword function*/
		g_functionID=4;
		/*Clears the screen for coming screens on LCD*/
		LCD_clearScreen();
	}
	else if(KEYPAD_getPressedKey() == '-')
	{
		/*Press time delay*/
		_delay_ms(500);
		/*Send an CHANGE_PASSWORD signal to Control ECU to inform it that option 2 is selected*/
		UART_sendByte(CHANGE_PASSWORD);
		/*Go to HMI_enterOldPassword function*/
		g_functionID=5;
		/*Clears the screen for coming screens on LCD*/
		LCD_clearScreen();
	}
}

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
void HMI_enterPassword(void)
{
	/*Variable to for loop till password size*/
	uint8 loop_idx=0;
	/*Display a message for user to enter password*/
	LCD_displayStringRowColumn(0,0,"Enter password");
	/*Move cursor to second row first place to display pressed key as '*'*/
	LCD_goToRowColumn(1,0);
	/*For loop from 0 --> password size
	 *Each loop, send the pressed key to Control ECU*/
	/*Send a ready signal to Control ECU to be ready to receive the new password*/
	UART_sendByte(HMI_ECU_READY);
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		/*Send pressed key to Control ECU*/
		UART_sendByte(KEYPAD_getPressedKey());
		/*Display * on LCD for each pressed key*/
		LCD_displayCharacter('*');
		/*Delay for press time*/
		_delay_ms(500);
	}
	UART_sendByte(OPEN_DOOR);
	uint8 key=UART_receiveByte();
	if(key == CORRECT_PASSWORD)
	{
		LCD_clearScreen();
		while(UART_receiveByte() != DOOR_UNLOCKING);
		LCD_displayStringRowColumn(0,0,"Unlocking door..");
		while(UART_receiveByte() != DOOR_LOCKING);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Locking door..");
		while(UART_receiveByte() != DOOR_LOCKED);
		LCD_clearScreen();
		g_functionID=3;
	}
	else
	{
		LCD_clearScreen();
		if(key == WRONG_PASSWORD)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Wrong Password!");
			_delay_ms(300);
			LCD_clearScreen();
			g_functionID=4;
		}
		else
		{
			LCD_displayStringRowColumn(0,4,"THIEF!!!");
			LCD_displayStringRowColumn(1,1,"SYSTEM LOCKED");
			while(UART_receiveByte() != SYSTEM_UNLOCKED);
			LCD_clearScreen();
			g_functionID=3;
		}
	}
}


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
void HMI_enterOldPassword(void)
{
	/*Variable to for loop till password size*/
	uint8 loop_idx=0;
	/*Display a message for user to enter password*/
	LCD_displayStringRowColumn(0,0,"Enter old pass");
	/*Move cursor to second row first place to display pressed key as '*'*/
	LCD_goToRowColumn(1,0);
	/*For loop from 0 --> password size
	 *Each loop, send the pressed key to Control ECU*/
	/*Send a ready signal to Control ECU to be ready to receive the new password*/
	UART_sendByte(HMI_ECU_READY);
	for(loop_idx=0;loop_idx<PASSWORD_SIZE;loop_idx++)
	{
		/*Send pressed key to Control ECU*/
		UART_sendByte(KEYPAD_getPressedKey());
		/*Display * on LCD for each pressed key*/
		LCD_displayCharacter('*');
		/*Delay for press time*/
		_delay_ms(500);
	}
	UART_sendByte(CHANGE_PASSWORD);
	uint8 key=UART_receiveByte();
	if(key == CORRECT_PASSWORD)
	{
		LCD_clearScreen();
		g_functionID=1;
	}
	else
	{
		LCD_clearScreen();
		if(key == WRONG_PASSWORD)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Wrong Password!");
			_delay_ms(300);
			LCD_clearScreen();
			g_functionID=5;
		}
		else
		{
			LCD_displayStringRowColumn(0,4,"THIEF!!!");
			LCD_displayStringRowColumn(1,1,"SYSTEM LOCKED");
			while(UART_receiveByte() != SYSTEM_UNLOCKED);
			LCD_clearScreen();
			g_functionID=3;
		}
	}
}
