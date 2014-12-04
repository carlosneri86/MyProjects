/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "BoardConfig.h"
#include "SW_Timer.h"
#include "LCD_Driver.h"
#include "Heartbeat.h"

#define TEST  (3)

#define CHANGE_STRING_TIMER		(10000/SWTIMER_BASE_TIME)

void ChangeString_TimerCallback (void);

uint8_t gbaTestString1[] =
{
	"Test LCD Muchos caracteres aqui"	
		
};

uint8_t gbaTestString2[] =
{
	"Hola"	
		
};

uint8_t gbaTestString3[] =
{
	"Claus te amo"	
		
};

uint8_t gbaTestString4[] =
{
	"quiero probrar  el LCD"	
		
};

uint8_t gabStringSizes[] =
{
		sizeof(gbaTestString1),
		sizeof(gbaTestString2),
		sizeof(gbaTestString3),
		sizeof(gbaTestString4),
};

uint8_t * gabpStringsPointers[] =
{
		&gbaTestString1[0],
		&gbaTestString2[0],
		&gbaTestString3[0],
		&gbaTestString4[0],
};

volatile uint8_t gbChangeStringNow = 0;

volatile uint8_t bChangeStringTimer = 0;
int main(void)
{
	uint8_t bStatus = LCD_DRIVER_BUSY;
	uint8_t bChangeStringCounter = 0;
	
	BoardConfig_vfnInit();
	SWTimer_vfnInit();
	LCDDriver_Init();
	Heartbeat_Init();
	
	bChangeStringTimer = SWTimer_vfnAllocateChannel(CHANGE_STRING_TIMER,ChangeString_TimerCallback);
	
	SWTimer_vfnEnableTimer(bChangeStringTimer);
	
	for(;;) 
	{	   
		SWTimer_vfnServiceTimers();
		LCDDriver_Task();
		
		if(bStatus == LCD_DRIVER_BUSY)
		{
#if TEST == 1
			bStatus = LCDDriver_WriteString(&gbaTestString1[0], sizeof(gbaTestString1)-1u);
		}
#elif TEST == 2
			
			bStatus = LCDDriver_MoveCursor(5,2);
		}
#elif TEST == 3			
		}	
#endif
		if(!LCDDriver_vfnStatus() && gbChangeStringNow)
		{
			LCDDriver_WriteString(gabpStringsPointers[bChangeStringCounter], gabStringSizes[bChangeStringCounter]-1u);
			
			bChangeStringCounter++;
			
			gbChangeStringNow = 0;
			
			if(bChangeStringCounter == 4)
			{
				bChangeStringCounter = 0;
			}
			
			SWTimer_vfnEnableTimer(bChangeStringTimer);
		}
		
	}
	
	return 0;
}


void ChangeString_TimerCallback (void)
{
	gbChangeStringNow = 1;
	
	SWTimer_vfnDisableTimer(bChangeStringTimer);
}
