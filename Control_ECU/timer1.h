/*******************************************************************************************
 * [FILE NAME]:		timer1.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	11 Jan 2020
 * [DESCRIPTION]:	This header file contains static configurations and function prototypes
 * 					for TIMER1 Module in AVR ATMEGA-16 Micro-controller
 *******************************************************************************************/
#ifndef TIMER1_H_
#define TIMER1_H_

/******************************************************************
 * 				Common Header Files Inclusion					  *
 ******************************************************************/
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/******************************************************************
 * 				    User-defined Data Types					      *
 ******************************************************************/
typedef enum{
	OVF,CTC
}TIMER1_Mode;

typedef enum{
	NO_CLK_SOURCE,F_CPU_1,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXT_FALLING,EXT_RISING
}TIMER1_Clk;

typedef struct{
	TIMER1_Mode mode;
	TIMER1_Clk clock;
	uint16 initialVal;
	uint16 compareVal;
}TIMER1_ConfigType;

/******************************************************************
 * 				  Public Functions Prototypes					  *
 ******************************************************************/
/*Description: This Function initialises the TIMER1 Module:
 * 1. Determines desired mode for TIMER1 either Compare(CTC)/Normal-Overflow(OVF)
 * 2. Determines desired clock for TIMER1 (i.e. which prescalar to be used)
 * 3. Determines desired channel of TIMER1 will be used either A or B
 * 4. Determines desired initial value for TIMER1 to start counting from
 * 5. Determines desired compare value for TIMER1 to compare with for CTC mode*/
void TIMER1_init(const TIMER1_ConfigType * Config_Ptr);

/*Description: This function changes prescalar for clock of TIMER1*/
void TIMER1_changeClock(TIMER1_Clk a_clock);

/*Description: This Function stops/de-initialises the TIMER1 Module*/
void TIMER1_DeInit(void);

/*Description: This function sets call back function for TIMER1 module*/
void TIMER1_setCallBack (void(*a_ptr)(void));

#endif /* TIMER1_H_ */
