/*******************************************************************************************
 * [FILE NAME]:		std_types.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	29 Dec 2019
 * [DESCRIPTION]:	This header file contains the type definitions of used variables
 *******************************************************************************************/

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/**********************BOOLEAN VALUES**************************/
#ifndef TRUE
#define TRUE	(1u)
#endif

#ifndef FALSE
#define FALSE	(0u)
#endif

/**********************DIGITAL VALUES**************************/
#ifndef HIGH
#define	HIGH	(1u)
#endif

#ifndef LOW
#define	LOW		(0u)
#endif

/**********************NULL VALUE****************************/
#define NULL_PTR ((void*)0)

/**********************BOOLEAN TYPE**************************/
typedef unsigned char 		bool;

/**********************UNSIGNED DECIMAL TYPES**************************/
typedef unsigned char 		uint8;			/*1 Byte		0 -> 255*/
typedef unsigned short 		uint16;			/*2 Bytes		0 -> 65536*/
typedef unsigned long 		uint32;			/*4 Bytes		0 -> 4294967296*/
typedef unsigned long long 	uint64;			/*8 Bytes		0 -> 18446744073709551615*/

/**********************SIGNED DECIMAL TYPES**************************/
typedef signed char 		sint8;			/*1 Byte		       -128 -> +127*/
typedef signed short 		sint16;			/*2 Bytes		     -32768 -> +32767*/
typedef signed long 		sint32;			/*4 Bytes		-2147483648 -> +2147483647*/
typedef signed long long 	sint64;			/*8 Bytes								  */

/**********************FLOAT TYPES**************************/
typedef float				float32;		/*4 Bytes*/
typedef double 				double64;		/*8 Bytes*/

#endif /* STD_TYPES_H_ */
