 /**********************************************************************************************//*!
 *
 * @file FileName.c
 *
 * @author B04198
 *
 * @date 4/02/2012
 *
 * @brief Brief description of the file
 *************************************************************************************************/
/*************************************************************************************************/
/*                                      Includes Section                                         */
/*************************************************************************************************/
#include <stdint.h>
#include "derivative.h"
#include "GPIO_Mux.h"

/*************************************************************************************************/
/*                                  Function Prototypes Section                                  */
/*************************************************************************************************/

/*************************************************************************************************/
/*                                   Defines & Macros Section                                    */
/*************************************************************************************************/

/*************************************************************************************************/
/*                                       Typedef Section                                         */
/*************************************************************************************************/

/*************************************************************************************************/
/*                                   Global Variables Section                                    */
/*************************************************************************************************/

/*************************************************************************************************/
/*                                   Static Variables Section                                    */
/*************************************************************************************************/

/*************************************************************************************************/
/*                                   Global Constants Section                                    */
/*************************************************************************************************/
static volatile uint32_t * const GPIO_gadwPortGPIO[GPIO_PORT_MAX] =
{
		&PORTA_PCR0,
		&PORTB_PCR0,
		&PORTC_PCR0,
		&PORTD_PCR0,
		&PORTE_PCR0,
};

const uint32_t GPIO_gadwPortGPIOClockGateMask[GPIO_PORT_MAX] =
{
		SIM_SCGC5_PORTA_MASK,
		SIM_SCGC5_PORTB_MASK,
		SIM_SCGC5_PORTC_MASK,
		SIM_SCGC5_PORTD_MASK,
		SIM_SCGC5_PORTE_MASK
};
/*************************************************************************************************/
/*                                   Static Constants Section                                    */
/*************************************************************************************************/

/*************************************************************************************************/
/*                                      Functions Section                                        */
/*************************************************************************************************/
/*!
      \fn  GPIO_vfnPinMux
      \param ePort: Port to be used. Values available on GPIO.h
      \param bPin: The pin to be configured
      \param bMuxSelection: Select the MUX option. Values available on GPIO.h
      \brief  Initializes the respective pin to the desired mux selection
      \return
*/
void GPIO_vfnPinMux(uint8_t ePort, uint8_t bPin, uint8_t bMuxSelection)
{
	uint32_t * pdwPortRegister;
	/* confirm there port is available */
	if(ePort < GPIO_PORT_MAX)
	{
		/* enable port clock gate */
		SIM_SCGC5 |= GPIO_gadwPortGPIOClockGateMask[ePort];
		
		/* take the PORTx_PCR0 address */
		pdwPortRegister = (uint32_t*)GPIO_gadwPortGPIO[ePort];
		pdwPortRegister[bPin] = PORT_PCR_MUX(bMuxSelection);
		
		
	}
}
