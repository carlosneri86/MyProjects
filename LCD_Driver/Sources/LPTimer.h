/*HEADER******************************************************************************************
*
* Copyright 2013 Freescale Semiconductor, Inc.
*
* Freescale Confidential Proprietary. Licensed under the Freescale AOA Protocol Software License. 
* See the FREESCALE_AOA_Protocol_LICENSE file distributed with this work for more details. You may 
* not use this file except in compliance with the License.
*
**************************************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR IMPLIED WARRANTIES, 
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************************************
*
* Comments:
*
*
**END********************************************************************************************/
#ifndef LPTIMER_H_
#define LPTIMER_H_


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                                         
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "BoardConfig.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Defines & Macros Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////
#

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Typedef Section                                          
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	LPTIMER_PRESCALER_BY_2 = 0,
	LPTIMER_PRESCALER_BY_4,
	LPTIMER_PRESCALER_BY_8,
	LPTIMER_PRESCALER_BY_16,
	LPTIMER_PRESCALER_BY_32,
	LPTIMER_PRESCALER_BY_64,
	LPTIMER_PRESCALER_BY_128,
	LPTIMER_PRESCALER_BY_256,
	LPTIMER_PRESCALER_BY_512,
	LPTIMER_PRESCALER_BY_1024,
	LPTIMER_PRESCALER_BY_2048,
	LPTIMER_PRESCALER_BY_4096,
	LPTIMER_PRESCALER_BY_8192,
	LPTIMER_PRESCALER_BY_16384,
	LPTIMER_PRESCALER_BY_32768,
	LPTIMER_PRESCALER_BY_65536
}eLPTimerPresacaler;

typedef enum
{
	LPTIMER_PRESCALER_CLOCK_0 = 0,
	LPTIMER_PRESCALER_CLOCK_1,
	LPTIMER_PRESCALER_CLOCK_2,
	LPTIMER_PRESCALER_CLOCK_3
}eLPTimerClockSelect;

typedef enum
{
	LPTIMER_COUNTER_MATCH_STATUS = 0
}eLPTimerStatus;

typedef enum
{
	LPTIMER_COUNTER_MATCH_MASK_STATUS = (1 << LPTIMER_COUNTER_MATCH_STATUS)
}eLPTimerStatusMasks;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                Function-like Macros Section                                   
///////////////////////////////////////////////////////////////////////////////////////////////////
#define LPTIMER_CHECK_STATUS(status)	(LPTimer_gbStatus & status)

#define LPTIMER_CLEAR_STATUS(status)	(LPTimer_gbStatus &= ~status)

#define LPTIMER_SET_STATUS(status)	(LPTimer_gbStatus |= status)
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Extern Constants Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Extern Variables Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////
extern volatile uint8_t LPTimer_gbStatus;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                Function Prototypes Section                                    
///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

void LPTimer_Init(uint8_t bPrescaler,uint8_t bClockSource, uint32_t dwCompareValue);

void LPTimer_EnableTimer (void);

#if defined(__cplusplus)
}
#endif // __cplusplus


#endif /* LPTIMER_H_ */
///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
