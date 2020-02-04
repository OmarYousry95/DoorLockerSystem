/*******************************************************************************************
 * [FILE NAME]:		common_macros.h
 * [AUTHOR]:		Omar Yousry
 * [DATE CREATED]:	29 Dec 2019
 * [DESCRIPTION]:	This header file contains the protoypes of some useful common macros:
 * 					1- SET_BIT(REG,BIT_NUM)
 * 					2- CLEAR_BIT(REG,BIT_NUM)
 * 					3- TOGGLE_BIT(REG,BIT_NUM)
 * 					4- ROR(REG,BIT_NUM)
 * 					5- ROL(REG,BIT_NUM)
 * 					6- IS_BIT_SET(REG,BIT_NUM)
 * 					7- IS_BIT_CLEAR(REG,BIT_NUM)
 *******************************************************************************************/

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

/***************************************************************
 * This macro sets a certain bit in a certain register/variable
 ***************************************************************/
#define SET_BIT(REG,BIT_NUM) (REG = REG | (1<<(BIT_NUM)))


/***************************************************************
 * This macro clears a certain bit in a certain register/variable
 ***************************************************************/
#define CLEAR_BIT(REG,BIT_NUM) (REG = REG & ~(1<<(BIT_NUM)))


/***************************************************************
 * This macro toggles a certain bit in a certain register/variable
 ***************************************************************/
#define TOGGLE_BIT(REG,BIT_NUM) (REG = REG ^ (1<<(BIT_NUM)))


/***************************************************************
 * This macro rotates a register/variable to the right by BIT_NUM
 ***************************************************************/
#define ROR(REG,BIT_NUM) (REG = (REG>>BIT_NUM) | (REG<<((sizeof(REG)*8)-BIT_NUM)))


/***************************************************************
 * This macro rotates a register/variable to the left by BIT_NUM
 ***************************************************************/
#define ROL(REG,BIT_NUM) (REG = (REG<<BIT_NUM) | (REG>>((sizeof(REG)*8)-BIT_NUM)))


/***************************************************************
 * This macro checks if a certain bit in a certain register/variable is set or not
 ***************************************************************/
#define IS_BIT_SET(REG,BIT_NUM) (REG & (1<<(BIT_NUM)))


/***************************************************************
 * This macro checks if a certain bit in a certain register/variable is clear or not
 ***************************************************************/
#define IS_BIT_CLEAR(REG,BIT_NUM) (!(REG & (1<<(BIT_NUM))))


#endif /* COMMON_MACROS_H_ */
