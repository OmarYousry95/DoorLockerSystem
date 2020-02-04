/*******************************************************************************************
 * [FILE NAME]:		timer1.c
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	11 Jan 2020
 * [DESCRIPTION]:	This c file contains implementation of functions for TIMER0 Module
 * 					in AVR ATMEGA-16 Micro-controller
 *******************************************************************************************/

#include "timer1.h"

/******************************************************************
 * 						Global Variables						  *
 ******************************************************************/
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/******************************************************************
 * 				  Interrupt Service Routines					  *
 ******************************************************************/
ISR(TIMER1_COMPA_vect)
{
	/*Go to callback function*/
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}
}

ISR(TIMER1_OVF_vect)
{
	/*Go to callback function*/
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();
	}
}

/******************************************************************
 * 				  Public Functions Definitions					  *
 ******************************************************************/
/*Description: This Function initialises the TIMER1 Module:
 * 1. Determines desired mode for TIMER1 either Compare(CTC)/Normal-Overflow(OVF)
 * 2. Determines desired clock for TIMER1 (i.e. which prescalar to be used)
 * 3. Determines desired channel of TIMER1 will be used either A or B
 * 4. Determines desired initial value for TIMER1 to start counting from
 * 5. Determines desired compare value for TIMER1 to compare with for CTC mode*/
void TIMER1_init(const TIMER1_ConfigType * Config_Ptr)
{
	/*Clear COM1A1 and COM1B1 bits for both CTC and OVF mode*/
	CLEAR_BIT(TCCR1A,COM1A1);
	CLEAR_BIT(TCCR1A,COM1B1);

	/*Set both FOC1A and FOC1B bits for non-pwm mode*/
	SET_BIT(TCCR1A,FOC1A);
	SET_BIT(TCCR1A,FOC1B);

	/*Clear WGM10 and WGM11 bits in TCCR1A register and WGM13 bit in TCCR1B
	 *for both CTC and OVF mode*/
	CLEAR_BIT(TCCR1A,WGM10);
	CLEAR_BIT(TCCR1A,WGM11);
	CLEAR_BIT(TCCR1B,WGM13);
	/*Inserts mode bit in WGM12 bit in TCCR1B register*/
	TCCR1B = (TCCR1B&0xF7) | ((Config_Ptr->mode)<<3);

	/*Inserts initial value in TCNT1 register*/
	TCNT1=Config_Ptr->initialVal;

	/*Inserts compare value in OCR1A or OCR1B register if it's compare mode
	 *and enable output compare match interrupt*/
	if(Config_Ptr->mode == CTC)
	{
		/*Inserts mode bit in COM1A0 bit*/
		TCCR1A = (TCCR1A&0xBF) | ((Config_Ptr->mode)<<6);
		/*Inserts compare value in OCR1A register*/
		OCR1A=Config_Ptr->compareVal;
		/*Enable Timer1 Output Compare A Match Interrupt Enable*/
		SET_BIT(TIMSK,OCIE1A);
	}
	/*Enable overflow interrupt if it's overflow mode*/
	if(Config_Ptr->mode == OVF)
	{
		/*Inserts mode bit in COM1A0 and COM1B0 bits*/
		CLEAR_BIT(TCCR1A,COM1A0);
		CLEAR_BIT(TCCR1A,COM1B0);
		/*Enable Timer1 Overflow Interrupt Enable*/
		SET_BIT(TIMSK,TOIE1);
	}
	/*Inserts clock bits in CS12:0 bits in TCCR1B register*/
	TCCR1B = (TCCR1B&0xF8) | (Config_Ptr->clock);
}

/*Description: This function changes prescalar for clock of TIMER1*/
void TIMER1_changeClock(TIMER1_Clk a_clock)
{
	/*Inserts clock bits in CS12:0 bits in TCCR1B register*/
	TCCR1B = (TCCR1B&0xF8) | (a_clock);
}


/*Description: This Function stops/de-initialises the TIMER1 Module*/
void TIMER1_DeInit(void)
{
	/*Clear all timer0 registers*/
	TCCR1A=0;
	TCCR1B=0;
	TCNT1=0;
	OCR1A=0;
	OCR1B=0;
	/*Disable interrupt enable for both OVF and CTC*/
	CLEAR_BIT(TIMSK,OCIE1A);
	CLEAR_BIT(TIMSK,OCIE1B);
	CLEAR_BIT(TIMSK,TOIE1);
}

/*Description: This function sets call back function for TIMER1 module*/
void TIMER1_setCallBack (void(*a_ptr)(void))
{
	g_callBackPtr=a_ptr;
}

