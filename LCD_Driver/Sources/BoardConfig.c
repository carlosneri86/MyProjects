
/*HEADER******************************************************************************************
*
* Comments:
*
*
**END********************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <MKL25Z4.h>
#include "BoardConfig.h"
#include "GPIO_Mux.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section                 
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section                       
///////////////////////////////////////////////////////////////////////////////////////////////////

void BoardConfig_vfnInit(void)
{
	/* SIM_SOPT2: PLLFLLSEL=1 */
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; /* Select PLL as a clock source for various peripherals */
	/* SIM_SOPT1: OSC32KSEL=3 */
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0x03); /* LPO 1kHz oscillator drives 32 kHz clock for various peripherals */

	/* Switch to FBE Mode */
	/* MCG_C2: LOCRE0=0,??=0,RANGE0=2,HGO0=0,EREFS0=1,LP=0,IRCS=0 */
	MCG_C2 = (MCG_C2_RANGE0(0x02) | MCG_C2_EREFS0_MASK);
	/* OSC0_CR: ERCLKEN=1,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
	OSC0_CR = OSC_CR_ERCLKEN_MASK;
	/* MCG_C1: CLKS=2,FRDIV=3,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	MCG_C1 = (MCG_C1_CLKS(0x02) | MCG_C1_FRDIV(0x03) | MCG_C1_IRCLKEN_MASK);
	/* MCG_C4: DMX32=0,DRST_DRS=0 */
	MCG_C4 &= (uint8_t)~(uint8_t)((MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x03)));
	/* MCG_C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=3 */
	MCG_C5 = MCG_C5_PRDIV0(0x03);
	/* MCG_C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0 */
	MCG_C6 = MCG_C6_VDIV0(0x00);
	while((MCG_S & MCG_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock. */
	}
	while((MCG_S & 0x0CU) != 0x08U) {    /* Wait until external reference clock is selected as MCG output */
	}
	/* Switch to PBE Mode */
	/* MCG_C6: LOLIE0=0,PLLS=1,CME0=0,VDIV0=0 */
	MCG_C6 = (MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0x00));
	while((MCG_S & 0x0CU) != 0x08U) {    /* Wait until external reference clock is selected as MCG output */
	}
	while((MCG_S & MCG_S_LOCK0_MASK) == 0x00U) { /* Wait until locked */
	}
	
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK;
	
	
	/* LCD Pins */
	GPIO_vfnPinMux(GPIO_PORT_C,10,GPIO_MUX_ALT_1);
	
	GPIO_vfnPinMux(GPIO_PORT_C,11,GPIO_MUX_ALT_1);
	
	GPIO_vfnPinMux(GPIO_PORT_C,12,GPIO_MUX_ALT_1);
	
	GPIO_vfnPinMux(GPIO_PORT_C,13,GPIO_MUX_ALT_1);
	
	GPIO_vfnPinMux(GPIO_PORT_D,7,GPIO_MUX_ALT_1);
	
	GPIO_vfnPinMux(GPIO_PORT_D,6,GPIO_MUX_ALT_1);
	
	GPIO_vfnPinMux(GPIO_PORT_B,19,GPIO_MUX_ALT_1);
	
	/* LCD data and control pins set up */
	
	GPIOC_PDDR |= (1<<10) | (1<<11) | (1<<12) | (1<<13);
	
	GPIOD_PDDR |= (1<<7) | (1<<6);
	
	GPIOB_PDDR |= (1<<19);
	
	/* I2C Pins set up */
	GPIO_vfnPinMux(GPIO_PORT_E,24,GPIO_MUX_ALT_5);
		
	GPIO_vfnPinMux(GPIO_PORT_E,25,GPIO_MUX_ALT_5);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
