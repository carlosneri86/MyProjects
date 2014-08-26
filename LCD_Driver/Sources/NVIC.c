/****************************************************************************************************/
/**
Copyright (c) 2008 Freescale Semiconductor
Freescale Confidential Proprietary
\file       NVIC.c
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
* Include files
*****************************************************************************************************/
#include "derivative.h"
#include "NVIC.h"
#include <stdint.h>
/*****************************************************************************************************
* Declaration of module wide FUNCTIONs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs - NOT for use in other modules
*****************************************************************************************************/
#define NVIC_MAX_IRQ	(INT_PORTD)
/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Code of project wide FUNCTIONS
*****************************************************************************************************/
/*!
      \fn  dwfnNVIC_EnableIRQ
      \param dwIRQtoEnable: IRQ to enable. Extracted from the enum on NVIC.h 
      \brief  Enables the NVIC bit for IRQn
      \return
*/
void NVIC_vfnEnableIRQ(uint32_t dwIRQtoEnable)
{
	uint32_t dwNVICBit;

	if(dwIRQtoEnable < NVIC_MAX_IRQ)
	{
		dwNVICBit      = (dwIRQtoEnable)%32;

		NVIC_ISER |= (1 << dwNVICBit);

	}
}
/*!
      \fn  dwfnNVIC_DisableIRQ
      \param dwfnNVIC_DisableIRQ: IRQ number to disable. Extracted from the enum on NVIC.h
      \brief  Disables the NVIC bit for IRQn
      \return
*/
void dwfnNVIC_DisableIRQ(uint32_t dwIRQtoDisable)
{
	uint32_t dwNVICBit;

	if(dwIRQtoDisable < NVIC_MAX_IRQ)
	{
		
		dwNVICBit      = (dwIRQtoDisable)%32;

		NVIC_ICER = (1 << dwNVICBit);

	}

}
