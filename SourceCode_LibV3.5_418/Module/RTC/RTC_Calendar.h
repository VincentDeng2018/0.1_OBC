

#ifndef _RTC_CALENDAR_H
#define _RTC_CALENDAR_H

#include <stdint.h>
/**********************************************************************************************
* Macros
**********************************************************************************************/
#ifndef FALSE
#define FALSE 0
#define TRUE !FALSE
#endif

#define RTC_YEAR_MIN 2016
#define RTC_YEAR_MAX 2100
#define RTC_BASE_YEAR 2016
#define RTC_DAY_OF_FOURYEAR 1461
#define RTC_DAY_OF_NORMALYEAR 365
#define RTC_SECOND_OF_DAY 86400
#define RTC_SECOND_OF_HOUR 3600
#define RTC_SECOND_OF_MINUTE 60
#define RTC_TICK_FOR_SECOND 250
/**********************************************************************************************
* Define the struct type for calendar.
**********************************************************************************************/
typedef struct
{
    uint16_t u16year;
    uint8_t  u8month;
    uint8_t  u8date;
    uint8_t  u8hour;
    uint8_t  u8minute;
    uint8_t  u8second;
}RTC_stTime;

/**********************************************************************************************
* Define the Interrupt Service Routine for different Vectors.
**********************************************************************************************/


/**********************************************************************************************
* Global Functions
**********************************************************************************************/
extern uint8_t RTC_GetTime(RTC_stTime *ptrRTC);
extern uint8_t RTC_SetTime(RTC_stTime *ptrRTC);
extern void RTC_SetTimeToDefault(void);
extern uint32_t RTC_GetSecondFromTime(RTC_stTime *ptrRTC);
extern void RTC_GetTimeFromSecond(uint32_t u32Sec, RTC_stTime *ptrRTC);
extern void RTC_ConfigInit(void);
extern void RTC_GetCompileTime(RTC_stTime *ptrRTC);
extern void RTC_Timer1sCallback(void);

#endif /* _RTC_CALENDAR_H */
