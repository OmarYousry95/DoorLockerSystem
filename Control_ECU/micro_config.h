/*******************************************************************************************
 * [FILE NAME]:		micro_config.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	29 Dec 2019
 * [DESCRIPTION]:	This header file contains the configuration of microcontroller ATMEGA16
 *******************************************************************************************/

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_

/*Microcontroller's Frequency*/

#ifndef F_CPU
#define F_CPU 8000000UL /*F_CPU = 1 MHz*/
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#endif /* MICRO_CONFIG_H_ */
