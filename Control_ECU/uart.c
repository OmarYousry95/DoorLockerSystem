/*******************************************************************************************
 * [FILE NAME]:		uart.c
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	20 Jan 2020
 * [DESCRIPTION]:	This c file contains implementation of functions for UART Module
 * 					in AVR ATMEGA-16 Micro-controller
 *******************************************************************************************/


#include "uart.h"

/******************************************************************
 * 						Global Variables						  *
 ******************************************************************/
#if(UART_MODE == UART_INTERRUPT)
/*Global pointer to function that takes void and return void to hold the address
 *of the address of passed callback function */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;
/*Global variable to hold byte to be used in receiveByte API in case of receiving
 *with interrupts and to be extern-ed*/
volatile uint8 g_RxData;
/*Global variable to hold string to be used in receiveString API in case of receiving
 *with interrupts and to be extern-ed*/
volatile uint8 g_RxString[20];
#endif

/******************************************************************
 * 				   Interrupt Service Routines					  *
 ******************************************************************/
#if(UART_MODE == UART_INTERRUPT)
/*ISR of USART, Rx Complete*/
/*Once the RXC flag is set, this ISR will be executed indicating that
 *the data is ready to be read from UDR register*/
ISR(USART_RXC_vect)
{
	/*Error Checking*/
	if (UCSRA & ((1<<FE)|(1<<DOR)|(1<<PE)))
	{
		return;
	}

	/*Read data byte from UDR buffer in global variable*/
	/*Automatically, the RXC flag will be cleared  when the data is read from UDR buffer*/
	if(IS_BIT_SET(UCSRB,RXB8))
	{
		SET_BIT(g_RxData,8);
	}
	g_RxData=(g_RxData&0x100)|(UDR);
	if(g_callBackPtr != NULL_PTR)
	{
		/*Call callback function through callback pointer*/
		(*g_callBackPtr)();
	}
}
#endif
/******************************************************************
 * 				  	  Functions Definitions				 		  *
 ******************************************************************/
/*Description: This function initialises UART Module
 * 1. Enables UART Module
 * 2. Sets baud rate according to desired rate
 * 3. Determines frame shape:
 * 		a. Parity type (even/odd/disabled)
 * 		b. Number of stop bit(s) (1/2)
 * 		c. Number of data bits (5/6/7/8/9)
 * 4. Sets mode of UART (polling/interrupt)*/
void UART_init(const UART_ConfigType * Config_Ptr)
{
	/*1. SETS BAUD RATE VALUE*/
	/*Variable to hold UBRR register value*/
	uint16 UBRRval=BAUD_RATE(Config_Ptr->baudRate);
	/*Set UBBR register value to set baud rate as desired*/
	UBRRL=UBRRval;
	if(UBRRval>255)
	{
		/*Clear URSEL bit to write in UBRRH register*/
		CLEAR_BIT(UBRRH,URSEL);
		/*Inserts last 4 bits of UBRRval calculated in 4 LSB of UBRRH register*/
		UBRRH=(UBRRH&0xF0) | (UBRRval>>8);
	}
	/****************************************************************************/
	/*Clear Data Buffer register*/
	UDR=0;
	/*Clear UCSRA register*/
	UCSRA=0;
	/*2. Enable Double UART Transmission Speed*/
	SET_BIT(UCSRA,U2X);
	/****************************************************************************/
	/*Clear UCSRB register
	 * 1. Clears UCSZ2 bit in case of data frame less than 9-bit
	 * 2. Disable RX Complete, Data Register Empty and TX complete Interrupts*/
	UCSRB=0;
	#if(UART_MODE == UART_INTERRUPT)
	/*Enable RX Complete Interrupt for receiving API with interrupts*/
		SET_BIT(UCSRB,RXCIE);
	#endif
	/****************************************************************************/
	/*Clear UCSRC register
	 * 1. Clears UMSEL bit to operate in asynchronous mode*/
	UCSRC=0;
	/*Enable URSEL bit to write in UCSRC register*/
	SET_BIT(UCSRC,URSEL);
	/*3. Inserts character size type (data frame number of bits) in UCSZ1:0 bits in UCSRC register*/
	UCSRC=(UCSRC&0xF9) | ((Config_Ptr->characterSize)<<UCSZ0);
	/*4. Inserts parity mode type in UPM1:0 bits in UCSRC register*/
	UCSRC=(UCSRC&0xCF) | ((Config_Ptr->parity)<<UPM0);
	/****************************************************************************/
	/*5. Inserts stop bits type in USBS bit in UCSRC register*/
	UCSRC=(UCSRC&0xF7) | ((Config_Ptr->stopBit)<<USBS);
	/****************************************************************************/
	/*6. Enable UART Module after configuration is done*/
	SET_BIT(UCSRB,RXEN);
	SET_BIT(UCSRB,TXEN);
}



/*Description: This function sends a byte of data by UART protocol*/
void UART_sendByte(const uint8 data)
{
	/*Busy-wait loop till the UDR register is empty to transmit data*/
	while(IS_BIT_CLEAR(UCSRA,UDRE));
	/*Once the UDRE flag is set, the buffer is empty to put data inside it
	 *to be transmitted and the flag will be cleared automatically again when
	 *data is put inside UDR register*/
	/*Handle the 8th bit and insert it in TXB8 bit in USCRB register in case
	 *of 9-bits data frame*/
	if(IS_BIT_SET(data,8))
	{
		SET_BIT(UCSRB,TXB8);
	}
	UDR=data;
}

#if(UART_MODE == UART_POLLING)
/*Description: This function receives a byte using UART protocol*/
uint8 UART_receiveByte(void)
{
	/*Busy-wait loop till the buffer is full with data to read it*/
	while(IS_BIT_CLEAR(UCSRA,RXC));
	/*Error Checking*/
	if (UCSRA & ((1<<FE)|(1<<DOR)|(1<<PE)))
	{
		return -1;
	}
	/*Once the RXC flag is set, the data is ready to be read from UDR register
	 *and the flag will be cleared automatically when the data is read from UDR buffer*/
	return UDR;
}
#endif


/*Description: This function sends a string using UART protocol*/
void UART_sendString(const uint8 * str)
{
	uint8 i=0;
	while(str[i] != '\0')
	{
		UART_sendByte(str[i]);
		i++;
	}
}

#if(UART_MODE == UART_POLLING)
/*Description: This function receives a string using UART protocol*/
void UART_receiveString(uint8 * str)
{
	uint8 i=0;
	str[i]=UART_receiveByte();
	/*Loop till '#' character*/
	while(str[i] != '#')
	{
		i++;
		str[i]=UART_receiveByte();
	}
	/*Replace '#' symbol with null terminator*/
	str[i]='\0';
}
#endif


#if(UART_MODE == UART_INTERRUPT)
/*Description: This function sets call back function for UART module*/
void UART_setCallBack (void(*a_ptr)(void))
{
	g_callBackPtr=a_ptr;
}
#endif
