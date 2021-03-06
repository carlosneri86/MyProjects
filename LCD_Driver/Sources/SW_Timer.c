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
#include <stdlib.h>
#include "LPTimer.h"
#include "SW_Timer.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

#define SWTIMER_MAX_TIMERS	(16UL)

#define SWTIMER_HW_COUNTER	((BUS_CLOCK/2)/1000UL)
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	uint32_t dwCounterReload;
	uint32_t dwCounter;
}SWTimer_t;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section                 
///////////////////////////////////////////////////////////////////////////////////////////////////

static void (* SWTimer_vfnpCallbacks[SWTIMER_MAX_TIMERS])(void);

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

const static uint16_t  LPTimer_gwaEnableMasks[SWTIMER_MAX_TIMERS] =
{
		(1<<0), (1<<1), (1<<2), (1<<3), (1<<4),
		(1<<5), (1<<6), (1<<7), (1<<8), (1<<9),
		(1<<10), (1<<11), (1<<12), (1<<13), (1<<14),
		(1<<15), 
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

static uint16_t SWTimer_gwTimersEnabled = 0;

static SWTimer_t SWTimers_gtCounters[SWTIMER_MAX_TIMERS];

static uint8_t SWTimer_CurrrentTimers = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section                       
///////////////////////////////////////////////////////////////////////////////////////////////////

void SWTimer_Init(void)
{
	LPTimer_Init((uint8_t)LPTIMER_PRESCALER_BY_2,(uint8_t)LPTIMER_PRESCALER_CLOCK_3,SWTIMER_HW_COUNTER);
	
	LPTimer_EnableTimer();
}

uint8_t SWTimer_AllocateChannel(uint32_t dwCounter, void (* vfnpTimerCallback)(void))
{
	uint8_t bStatus = SWTIMER_ERROR;
	
	if(SWTIMER_MAX_TIMERS > SWTimer_CurrrentTimers)
	{
		SWTimers_gtCounters[SWTimer_CurrrentTimers].dwCounter = dwCounter;
		SWTimers_gtCounters[SWTimer_CurrrentTimers].dwCounterReload = dwCounter;
		
		if(vfnpTimerCallback != NULL)
		{
			SWTimer_vfnpCallbacks[SWTimer_CurrrentTimers] = vfnpTimerCallback;
		
			bStatus = SWTimer_CurrrentTimers;
			SWTimer_CurrrentTimers++;
			
		}
	}
	
	return(bStatus);
}

void SWTimer_EnableTimer(uint8_t bTimerToEnable)
{	
	if(SWTIMER_MAX_TIMERS > bTimerToEnable)
	{
		SWTimer_gwTimersEnabled |= LPTimer_gwaEnableMasks[bTimerToEnable];
	}
}

void SWTimer_UpdateCounter(uint8_t bTimerToUpdate, uint32_t dwNewCounter)
{
	if(SWTIMER_MAX_TIMERS > bTimerToUpdate)
	{
		SWTimers_gtCounters[bTimerToUpdate].dwCounter = dwNewCounter;
		SWTimers_gtCounters[bTimerToUpdate].dwCounterReload = dwNewCounter;
	}
}

void SWTimer_DisableTimer(uint8_t bTimerToDisable)
{
	if(SWTIMER_MAX_TIMERS > bTimerToDisable)
	{
		SWTimer_gwTimersEnabled &= ~LPTimer_gwaEnableMasks[bTimerToDisable];
	}
}

void SWTimer_ServiceTimers(void)
{
	uint8_t bMaxCounter = SWTIMER_MAX_TIMERS - 1;
	
	if(LPTIMER_CHECK_STATUS(LPTIMER_COUNTER_MATCH_MASK_STATUS))
	{
		LPTIMER_CLEAR_STATUS(LPTIMER_COUNTER_MATCH_MASK_STATUS);
		
		do 
		{
			if(SWTimer_gwTimersEnabled&LPTimer_gwaEnableMasks[bMaxCounter])
			{
				SWTimers_gtCounters[bMaxCounter].dwCounter--;
				
				if(!SWTimers_gtCounters[bMaxCounter].dwCounter)
				{
					SWTimer_vfnpCallbacks[bMaxCounter]();
					SWTimers_gtCounters[bMaxCounter].dwCounter = SWTimers_gtCounters[bMaxCounter].dwCounterReload;
				}
			}
			
		}while(bMaxCounter--);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
