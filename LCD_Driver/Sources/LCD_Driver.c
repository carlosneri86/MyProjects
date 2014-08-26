/*HEADER******************************************************************************************
*
* Comments:
*
*
**END********************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "StateMachine.h"
#include "LCD_Driver.h"
#include "SW_Timer.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

#define LCD_DRIVER_CONTROL_E_MASK	(1<<LCD_DRIVER_E_PIN)

#define LCD_DRIVER_CONTROL_RS_MASK	(1<<LCD_DRIVER_RS_PIN)

#define LCD_DRIVER_RESET_SIZE		(4)

#define LCD_DRIVER_INIT_SIZE		(6)

#define LCD_DRIVER_COLUMNS			(16)

#define LCD_DRIVER_ROWS				(2)

#define LCD_DRIVER_MAX_CHARACTERES			(LCD_DRIVER_COLUMNS * LCD_DRIVER_LINES)

#define LCD_DRIVER_HOME_COMMAND		(0x01)

#define LCD_DRIVER_LINE_1			(0x80)

#define LCD_DRIVER_LINE_2			(0xC0)

#define LCD_DRIVER_RESET_TIMEOUT	(50/SWTIMER_BASE_TIME)

#define LCD_DRIVER_TIMEOUT			(5/SWTIMER_BASE_TIME)

#define LCD_DRIVER_SET_STATUS(status)		(LCDDriver_Status |= status)

#define LCD_DRIVER_CLEAR_STATUS(status)		(LCDDriver_Status &= ~status)

#define LCD_DRIVER_CHECK_STATUS(status)		(LCDDriver_Status & status)
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	LCD_DRIVER_IDLE_STATE = 0,
	LCD_DRIVER_INITIALIZATION_STATE,
	LCD_DRIVER_STRING_STATE,
	LCD_DRIVER_WAIT_TIMER_STATE,
	LCD_DRIVER_MAX_STATES
}eLCDDriverStates;

typedef enum
{
	LCD_DRIVER_TIMEOUT_STATUS = 0,
	LCD_DRIVER_BUSY_STATUS,
	LCD_DRIVER_RESET_STATUS
}eLCDDriverStatus;

typedef enum
{
	LCD_DRIVER_TIMEOUT_MASK_STATUS = (1<<LCD_DRIVER_TIMEOUT_STATUS),
	LCD_DRIVER_BUSY_MASK_STATUS = (1<<LCD_DRIVER_BUSY_STATUS),
	LCD_DRIVER_RESET_MASK_STATUS = (1<<LCD_DRIVER_RESET_STATUS) 
	
}eLCDDriverMasksStatus;

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section                 
///////////////////////////////////////////////////////////////////////////////////////////////////

void LCDDriver_TimerCallback(void);

static void LCDDriver_WriteCommand(uint8_t bCommand);

static void LCDDriver_WriteCharacter(uint8_t bCharacter);

static void LCDDriver_IdleState(void);

static void LCDDriver_InitState(void);

static void LCDDriver_WaitTimerState(void);

static void LCDDriver_WriteStringState(void);

static void (* const LCDDriver_vfnpaStateMachine[LCD_DRIVER_MAX_STATES])(void) =
{
		LCDDriver_IdleState,
		LCDDriver_InitState,
		LCDDriver_WriteStringState,
		LCDDriver_WaitTimerState
		
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

static const uint8_t LCDDriver_gbaInit[LCD_DRIVER_INIT_SIZE] =
{
		0x28,		/* two lines */
		0x28,		/* two lines */
		0x28,		/* two lines */
		0x0F,		/* display off, cursor off */
		0x01,		/* clear screen, home */
		0x06		/* increment cursor*/
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

static state_machine_t LCDDriver_tStateMachine;

static uint8_t * LCDDriver_bpStringToDisplay;

static uint8_t LCDDriver_bCharacterstoWrite = 0;

static uint8_t LCDDriver_Timer;

static volatile uint8_t LCDDriver_Status;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section                       
///////////////////////////////////////////////////////////////////////////////////////////////////

void LCDDriver_Init(void)
{
	LCD_DRIVER_SET_STATUS(LCD_DRIVER_BUSY_MASK_STATUS);
	
	LCDDriver_Timer = SWTimer_AllocateChannel(LCD_DRIVER_RESET_TIMEOUT,LCDDriver_TimerCallback);
	
	SWTimer_EnableTimer(LCDDriver_Timer);
	
	LCDDriver_tStateMachine.bCurrentState = LCD_DRIVER_WAIT_TIMER_STATE;
	
	LCDDriver_tStateMachine.bNextState = LCD_DRIVER_INITIALIZATION_STATE;
}

void LCDDriver_Task (void)
{
	LCDDriver_vfnpaStateMachine[LCDDriver_tStateMachine.bCurrentState]();
}

uint8_t LCDDriver_WriteString(uint8_t * bpStringToWrite, uint8_t bAmountOfCharacters)
{
	
	uint8_t bError = LCD_DRIVER_BUSY;
	
	if(!LCD_DRIVER_CHECK_STATUS(LCD_DRIVER_BUSY_MASK_STATUS))
	{
		LCD_DRIVER_SET_STATUS(LCD_DRIVER_BUSY_MASK_STATUS);
		
		LCDDriver_bpStringToDisplay = bpStringToWrite;
		
		LCDDriver_bCharacterstoWrite = bAmountOfCharacters;
		
		LCDDriver_WriteCommand((LCD_DRIVER_HOME_COMMAND&0xF0)>>4);
		
		LCDDriver_WriteCommand((LCD_DRIVER_HOME_COMMAND&0x0F));
		
		LCDDriver_tStateMachine.bCurrentState = LCD_DRIVER_WAIT_TIMER_STATE;
		
		LCDDriver_tStateMachine.bNextState = LCD_DRIVER_STRING_STATE;
		
		SWTimer_EnableTimer(LCDDriver_Timer);
		
		bError = LCD_DRIVER_OK;
	}
	
	return(bError);
	
}

uint8_t LCDDriver_MoveCursor(uint8_t bXAxis, uint8_t bYAxis)
{
	uint8_t bError = LCD_DRIVER_BUSY;
	uint8_t bCursorPosition;
		
	if(!LCD_DRIVER_CHECK_STATUS(LCD_DRIVER_BUSY_MASK_STATUS))
	{
		if(LCD_DRIVER_COLUMNS >= bXAxis)
		{
			if(LCD_DRIVER_ROWS >= bYAxis)
			{
				bCursorPosition = LCD_DRIVER_LINE_1|((bYAxis - 1u)<<6)|(bXAxis - 1u);
							
				LCDDriver_WriteCommand((bCursorPosition&0xF0)>>4);
						
				LCDDriver_WriteCommand((bCursorPosition&0x0F));
				
				LCDDriver_tStateMachine.bCurrentState = LCD_DRIVER_WAIT_TIMER_STATE;
				
				LCDDriver_tStateMachine.bNextState = LCD_DRIVER_IDLE_STATE;
				
				bError = LCD_DRIVER_OK;
				
				SWTimer_EnableTimer(LCDDriver_Timer);
			}
		}
	}
	
	return(bError);
}

static void LCDDriver_WriteCommand(uint8_t bCommand)
{
	LCD_DRIVER_CONTROL_CLEAR_PORT = LCD_DRIVER_CONTROL_RS_MASK;
	
	LCD_DRIVER_CONTROL_SET_PORT = LCD_DRIVER_CONTROL_E_MASK;
	
	LCD_DRIVER_DATA_PORT = ((bCommand&0x0F) << LCD_DRIVER_DATA_SHIFT);
	
	LCD_DRIVER_CONTROL_CLEAR_PORT = LCD_DRIVER_CONTROL_E_MASK;
	
}

static void LCDDriver_WriteCharacter(uint8_t bCharacter)
{
	LCD_DRIVER_CONTROL_SET_PORT = LCD_DRIVER_CONTROL_RS_MASK;
	
	LCD_DRIVER_CONTROL_SET_PORT = LCD_DRIVER_CONTROL_E_MASK;
	
	LCD_DRIVER_DATA_PORT = ((bCharacter&0x0F) << LCD_DRIVER_DATA_SHIFT);
	
	LCD_DRIVER_CONTROL_CLEAR_PORT = LCD_DRIVER_CONTROL_E_MASK;
	
}

static void LCDDriver_IdleState(void)
{
	
}

static void LCDDriver_InitState(void)
{
	static uint8_t bInitOffset = 0;
	static uint8_t bCommandToExecute = 0;

	if(LCD_DRIVER_INIT_SIZE > bInitOffset)
	{

		if(!bInitOffset)
		{
		
			bCommandToExecute = (LCDDriver_gbaInit[bInitOffset]&0xF0)>>4;
			
			LCDDriver_WriteCommand(bCommandToExecute);
			
			bCommandToExecute = (LCDDriver_gbaInit[bInitOffset]&0x0F);
			
			LCDDriver_WriteCommand(bCommandToExecute);
			
			LCDDriver_tStateMachine.bCurrentState = LCD_DRIVER_WAIT_TIMER_STATE;
			
			LCDDriver_tStateMachine.bNextState = LCD_DRIVER_INITIALIZATION_STATE;
			
			SWTimer_EnableTimer(LCDDriver_Timer);

		}
		else
		{
			bCommandToExecute = (LCDDriver_gbaInit[bInitOffset]&0xF0)>>4;
						
			LCDDriver_WriteCommand(bCommandToExecute);
		}
		
		bInitOffset++;

	}
	else
	{
		LCDDriver_tStateMachine.bCurrentState = LCD_DRIVER_IDLE_STATE;
		bInitOffset = 0;
		
		LCD_DRIVER_CLEAR_STATUS(LCD_DRIVER_BUSY_MASK_STATUS);
	}
}

static void LCDDriver_WaitTimerState(void)
{
	if(LCD_DRIVER_CHECK_STATUS(LCD_DRIVER_TIMEOUT_MASK_STATUS))
	{
		
		LCD_DRIVER_CLEAR_STATUS(LCD_DRIVER_TIMEOUT_MASK_STATUS);
		
		LCDDriver_tStateMachine.bCurrentState = LCDDriver_tStateMachine.bNextState;
	}
}

static void LCDDriver_WriteStringState(void)
{
	static uint8_t bStringOffset = 0;
	static uint8_t bCurrentColumn = 0;
	
	if(LCDDriver_bCharacterstoWrite > bStringOffset)
	{
		
		if(LCD_DRIVER_COLUMNS > bCurrentColumn)
		{
			LCDDriver_WriteCharacter((LCDDriver_bpStringToDisplay[bStringOffset]&0xF0)>>4);
			
			LCDDriver_WriteCharacter((LCDDriver_bpStringToDisplay[bStringOffset]&0x0F));
			bCurrentColumn++;
			bStringOffset++;
		}
		else
		{
			bCurrentColumn = 0;
	
			LCDDriver_WriteCommand((LCD_DRIVER_LINE_2&0xF0)>>4);
			
			LCDDriver_WriteCommand((LCD_DRIVER_LINE_2&0x0F));
		}
		
		SWTimer_EnableTimer(LCDDriver_Timer);
		
		LCDDriver_tStateMachine.bCurrentState = LCD_DRIVER_WAIT_TIMER_STATE;
		
		LCDDriver_tStateMachine.bNextState = LCD_DRIVER_STRING_STATE;
	}
	else
	{
		bStringOffset = 0;
		bCurrentColumn = 0;
		
		LCDDriver_tStateMachine.bCurrentState = LCD_DRIVER_IDLE_STATE;
		
		LCD_DRIVER_CLEAR_STATUS(LCD_DRIVER_BUSY_MASK_STATUS);
	}
}

void LCDDriver_TimerCallback(void)
{
	LCD_DRIVER_SET_STATUS(LCD_DRIVER_TIMEOUT_MASK_STATUS);
	
	if(!LCD_DRIVER_CHECK_STATUS(LCD_DRIVER_RESET_MASK_STATUS))
	{
		LCD_DRIVER_SET_STATUS(LCD_DRIVER_RESET_MASK_STATUS);
		
		SWTimer_UpdateCounter(LCDDriver_Timer, LCD_DRIVER_TIMEOUT);
	}
	
	
	SWTimer_DisableTimer(LCDDriver_Timer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
