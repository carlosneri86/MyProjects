/*HEADER******************************************************************************************
*
* Comments:
*
*
**END********************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "I2C.h"
#include "derivative.h"
#include "NVIC.h"
#include "StateMachine.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

#define I2C_SET_STATUS(x)	(I2C_gbStatus |= x)

#define I2C_CHECK_STATUS(x)	(I2C_gbStatus & x)

#define I2C_CLEAR_STATUS(x)	(I2C_gbStatus &= ~x)

#define I2C_CLEAR_ALL_STATUS	(I2C_gbStatus &= ~0xFF)
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	I2C_IDLE_STATE = 0,
	I2C_START_STATE,
	I2C_WRITE_STATE,
	I2C_READ_STATE,
	I2C_STOP_STATE,
	I2C_WAIT_INTERRUPT_STATE,
	I2C_MAX_STATE
}eI2CStates;

typedef enum
{
	I2C_BUSY = 0,
	I2C_WRITE,
	I2C_READ,
	I2C_RANDOM_READ,
	I2C_ERROR,
	I2C_TX_DONE,
}eI2CStatus;

typedef enum
{
	I2C_BUSY_MASK = (1 << I2C_BUSY),
	I2C_WRITE_MASK = (1 << I2C_WRITE),
	I2C_READ_MASK = (1 << I2C_READ),
	I2C_RANDOM_READ_MASK = (1 << I2C_RANDOM_READ),
	I2C_ERROR_MASK = (1 << I2C_ERROR),
	I2C_TX_DONE_MASK = (1 << I2C_TX_DONE)
}eI2CStatusMasks;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section                 
///////////////////////////////////////////////////////////////////////////////////////////////////

void (*const I2C_vfnpStateMachine[I2C_MAX_STATE])(void) = 
{
		
};

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
static uint8_t I2C_gbSlaveAddress = 0;

static uint8_t * I2C_gbpOutputBuffer = 0;

static uint8_t * I2C_gbpInputBuffer = 0;

static uint8_t I2C_gbDataToSend = 0;

static uint8_t I2C_gbDataToReceive = 0;

static uint8_t I2C_gbStatus = 0;

static uint8_t I2C_gbLastHWStatus = 0;

static state_machine_t I2C_StateMachine;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section                       
///////////////////////////////////////////////////////////////////////////////////////////////////

void I2C_vfnIniti(void)
{
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
	
	I2C0_F = I2C_F_MULT(I2C_MULT)|I2C_F_ICR(I2C_ICR);
	
	NVIC_vfnEnableIRQ(NVIC_I2C0);
	
	I2C0_C1 = I2C_C1_IICEN_MASK;
	
	I2C_StateMachine.bCurrentState = I2C_IDLE_STATE;

}

void I2C_vfnStateMachine(void)
{
	
}


void I2C_Write(uint8_t bSlaveAddress, uint8_t const * pbBufferToWrite, uint8_t bBufferSize)
{
	/*The drive assumes the 7-bit address is shift 1 bit to left*/
	/*Set 0 the RW bit */
	I2C_gbSlaveAddress = (bSlaveAddress&0xFE);
	
	I2C_gbDataToSend = bBufferSize;
	
	I2C_gbpOutputBuffer = pbBufferToWrite;
	I2C_SET_STATUS(I2C_BUSY_MASK);
	I2C_SET_STATUS(I2C_WRITE_MASK);
	I2C_CLEAR_STATUS(I2C_ERROR_MASK);
	
	I2C_StateMachine.bCurrentState = I2C_START_STATE;
}

void I2C0_IRQHandler (void)
{
	if(I2C0_S & I2C_S_TCF_MASK)
	{
		I2C_SET_STATUS(I2C_TX_DONE_MASK);
	}
	I2C_gbLastHWStatus = I2C0_S;
	I2C0_S |= I2C_S_IICIF_MASK;
}

static void I2C_Start_State(void)
{
	I2C_StateMachine.bCurrentState = I2C_WAIT_INTERRUPT_STATE;
	
	if((I2C_CHECK_STATUS(I2C_WRITE_MASK)) || (I2C_SET_STATUS(I2C_RANDOM_READ_MASK)))
	{
		I2C_StateMachine.bNextState = I2C_WRITE_STATE;
	}
	else
	{
		I2C_StateMachine.bNextState = I2C_READ_STATE;
	}
	
	/*Generate start and set I2C in TX mode. Then send slave address*/
	I2C0_C1 |= I2C_C1_MST_MASK|I2C_C1_TX_MASK;
	
	I2C0_D = I2C_gbSlaveAddress;
}

static void I2C_Write_State(void)
{
	static uint16_t wBufferOffset = 0;
	
	/* first confirm if an ACK was received */
	if(!(I2C_gbLastHWStatus & I2C_S_RXAK_MASK))
	{
		if(I2C_gbDataToSend)
		{
			I2C_gbDataToSend--;
			
			I2C0_D = I2C_gbpOutputBuffer[wBufferOffset++];
			
			I2C_StateMachine.bCurrentState = I2C_WAIT_INTERRUPT_STATE;
			
			I2C_StateMachine.bNextState = I2C_WRITE_STATE;
			
		}
		else
		{
			wBufferOffset = 0;
			
			I2C_StateMachine.bCurrentState = I2C_STOP_STATE;
			
			I2C_CLEAR_STATUS(I2C_WRITE_MASK);
		}
	}
	else
	{
		wBufferOffset = 0;
					
		I2C_StateMachine.bCurrentState = I2C_STOP_STATE;
		
		I2C_SET_STATUS(I2C_ERROR_MASK);
		
		I2C_CLEAR_STATUS(I2C_WRITE_MASK);
	}
}

static void I2C_Read_State(void)
{
	static uint16_t wInputBufferOffset = 0;

	if(I2C_gbDataToReceive)
	{
				
		I2C_StateMachine.bCurrentState = I2C_WAIT_INTERRUPT_STATE;
		
		I2C_StateMachine.bNextState = I2C_READ_STATE;
		
		/* Set RX mode */
		if(!wInputBufferOffset)
		{
			I2C0_C1 &= ~(I2C_C1_TXAK_MASK|I2C_C1_TX_MASK);
		}
		
		
		/* if just 1 data left, send NACK */
		if(I2C_gbDataToReceive == 1)
		{
			I2C0_C1 |= I2C_C1_TXAK_MASK;
		}
		
		I2C_gbDataToReceive--;
		
		I2C_gbpInputBuffer[wInputBufferOffset++] = I2C0_D;
	}
	else
	{
		wInputBufferOffset = 0;
		
		I2C_StateMachine.bCurrentState = I2C_STOP_STATE;
		
		I2C_CLEAR_STATUS(I2C_READ_MASK);
	}

	
}

static void I2C_Wait_Interrupt_State(void)
{
	if(I2C_CHECK_STATUS(I2C_TX_DONE_MASK))
	{
			
		I2C_StateMachine.bCurrentState = I2C_StateMachine.bNextState;
		
		I2C_CLEAR_STATUS(I2C_TX_DONE_MASK);
	}
}

static void I2C_Stop_State(void)
{
	/* confirm bus is not busy */
	if(!(I2C0_S & I2C_S_BUSY_MASK))
	{
		I2C_StateMachine.bCurrentState = I2C_IDLE_STATE;
		
		I2C_CLEAR_STATUS(I2C_BUSY_MASK);
	}
	else
	{
		/*set up a timer?*/
		/* Generate stop signal */
		I2C0_C1 &= ~I2C_C1_MST_MASK;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
