/*HEADER******************************************************************************************
*
* Comments:
*
*
**END********************************************************************************************/
#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                                         
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "derivative.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Defines & Macros Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////

#define LCD_DRIVER_DATA_0_PIN	(10)
#define LCD_DRIVER_DATA_1_PIN	(11)
#define LCD_DRIVER_DATA_2_PIN	(12)
#define LCD_DRIVER_DATA_3_PIN	(13)

#define LCD_DRIVER_DATA_SHIFT	(LCD_DRIVER_DATA_0_PIN)


#define LCD_DRIVER_E_PIN		(6)
#define LCD_DRIVER_RS_PIN		(7)

#define LCD_DRIVER_CONTROL_SHIFT	(LCD_DRIVER_E_PIN)

#define LCD_DRIVER_DATA_PORT	(GPIOC_PDOR)

#define LCD_DRIVER_CONTROL_PORT			(GPIOD_PDOR)
#define LCD_DRIVER_CONTROL_SET_PORT		(GPIOD_PSOR)
#define LCD_DRIVER_CONTROL_CLEAR_PORT	(GPIOD_PCOR)

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Typedef Section                                          
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	LCD_DRIVER_OK = 0,
	LCD_DRIVER_BUSY
}eLCDDriverErrors;
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                Function-like Macros Section                                   
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Extern Constants Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Extern Variables Section                                     
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                Function Prototypes Section                                    
///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

void LCDDriver_Task (void);

void LCDDriver_Init(void);

uint8_t LCDDriver_WriteString(uint8_t * bpStringToWrite, uint8_t bAmountOfCharacters);

uint8_t LCDDriver_MoveCursor(uint8_t bXAxis, uint8_t bYAxis);

uint8_t LCDDriver_vfnStatus(void);
#if defined(__cplusplus)
}
#endif // __cplusplus


#endif /* LCD_DRIVER_H_ */
///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
