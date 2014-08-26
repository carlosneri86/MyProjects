/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "BoardConfig.h"
#include "SW_Timer.h"
#include "LCD_Driver.h"
#include "Heartbeat.h"

#define TEST  (1)

uint8_t gbaTestString[] =
{
	"Test LCD Muchos caracteres aqui"	
		
};

int main(void)
{
	uint8_t bStatus = LCD_DRIVER_BUSY;
	
	BoardConfig_vfnInit();
	SWTimer_Init();
	LCDDriver_Init();
	Heartbeat_Init();
	
	for(;;) 
	{	   
		SWTimer_ServiceTimers();
		LCDDriver_Task();
		
		if(bStatus == LCD_DRIVER_BUSY)
		{
#if TEST == 1
			bStatus = LCDDriver_WriteString(&gbaTestString[0], sizeof(gbaTestString)-1u);
#elif TEST == 2
			
			bStatus = LCDDriver_MoveCursor(5,2);
			
#endif
		}
		
	}
	
	return 0;
}
