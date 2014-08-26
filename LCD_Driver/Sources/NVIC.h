/****************************************************************************************************/
/**
Copyright (c) 2008 Freescale Semiconductor
Freescale Confidential Proprietary
\file       NVIC.h
\brief
\author     Freescale Semiconductor
\author     Guadalajara Applications Laboratory RTAC Americas
\author     B22385
\version    0.1
\date       May 30, 2012
*/
/****************************************************************************************************/
/*                                                                                                  */
/* All software, source code, included documentation, and any implied know-how are property of      */
/* Freescale Semiconductor and therefore considered CONFIDENTIAL INFORMATION.                       */
/* This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.                      */
/*                                                                                                  */
/* All Confidential Information remains the property of Freescale Semiconductor and will not be     */
/* copied or reproduced without the express written permission of the Discloser, except for copies  */
/* that are absolutely necessary in order to fulfill the Purpose.                                   */
/*                                                                                                  */
/* Services performed by FREESCALE in this matter are performed AS IS and without any warranty.     */
/* CUSTOMER retains the final decision relative to the total design and functionality of the end    */
/* product.                                                                                         */
/* FREESCALE neither guarantees nor will be held liable by CUSTOMER for the success of this project.*/
/*                                                                                                  */
/* FREESCALE disclaims all warranties, express, implied or statutory including, but not limited to, */
/* implied warranty of merchantability or fitness for a particular purpose on any hardware,         */
/* software ore advise supplied to the project by FREESCALE, and or any product resulting from      */
/* FREESCALE services.                                                                              */
/* In no event shall FREESCALE be liable for incidental or consequential damages arising out of     */
/* this agreement. CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or */
/* actions by anyone on account of any damage,or injury, whether commercial, contractual, or        */
/* tortuous, rising directly or indirectly as a result of the advise or assistance supplied CUSTOMER*/
/* in connectionwith product, services or goods supplied under this Agreement.                      */
/*                                                                                                  */
/****************************************************************************************************/

/*****************************************************************************************************
* Module definition against multiple inclusion
*****************************************************************************************************/

#ifndef NVIC_H_
#define NVIC_H_
/*****************************************************************************************************
* Include files
*****************************************************************************************************/


/*****************************************************************************************************
* Declaration of project wide TYPES
*****************************************************************************************************/
enum eNVICIrqSources
{
	NVIC_DMA0 = 0,              
	NVIC_DMA1,             
	NVIC_DMA2,             
	NVIC_DMA3,             
	NVIC_Reserved0,       
	NVIC_FTFA,             
	NVIC_LVD_LVW,          
	NVIC_LLW,              
	NVIC_I2C0,             
	NVIC_I2C1,             
	NVIC_SPI0,             
	NVIC_SPI1,             
	NVIC_UART0,            
	NVIC_UART1,            
	NVIC_UART2,            
	NVIC_ADC0,             
	NVIC_CMP0,             
	NVIC_TPM0,             
	NVIC_TPM1,             
	NVIC_TPM2,             
	NVIC_RTC,              
	NVIC_RTC_Seconds,      
	NVIC_PIT,              
	NVIC_Reserved1,       
	NVIC_USB0,             
	NVIC_DAC0,             
	NVIC_TSI0,             
	NVIC_MCG,              
	NVIC_LPTIMER,          
	NVIC_Reserved2,       
	NVIC_PORTA,            
	NVIC_PORTD             
};
/*****************************************************************************************************
* Definition of project wide VARIABLES
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of project wide MACROS / #DEFINE-CONSTANTS
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of project wide FUNCTIONS
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide FUNCTIONs - NOT for use in other modules
*****************************************************************************************************/
void NVIC_vfnEnableIRQ(uint32_t dwIRQtoEnable);
void NVIC_vfnDisableIRQ(uint32_t dwIRQtoDisable);
#endif /*NVIC_H_*/

