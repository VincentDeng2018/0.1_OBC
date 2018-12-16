// ******************************************************************************************************
// *            CAENDAR.c
// ******************************************************************************************************
// ******************************************************************************************************
// *
// * THIS INFORMATION IS PROPRIETARY TO BYD Corporation
// *
// ******************************************************************************************************
// *
// *
// ******************************************************************************************************
// ******************************************************************************************************
// *    FILE NAME: CALENDAR.c
// *
// *    DESCRIPTION: using for Calendar implement.
// *
// *    ORIGINATOR: GHT
// *
// *    DATE: 2016/12/08
// *
// *             Version   Author     Modification
// *    HISTORY:  V1.0      GHT       Calendar implement
// ******************************************************************************************************

// *********************************************************************************************************
// *        INCLUDE FILES
// *********************************************************************************************************
/* Standard lib include */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Chip lib include */


/* User include */
#include "RTC_Calendar.h"

// ********************************************************************************************************
// * Global DECLARATIONS
// ********************************************************************************************************

// ********************************************************************************************************
// * LOCAL FUNCTION PROTOTYPES
// ********************************************************************************************************
static uint8_t RTC_IsLeapYear (uint16_t u16year);
static uint8_t RTC_IsValidDateTime(RTC_stTime *ptrRTC);

// ********************************************************************************************************
// * LOCAL VARIABLES
// ********************************************************************************************************
static uint8_t DayOfOneMonthValMAX[] = {0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static uint32_t DayOfCurrentMonthNormal[] = {0,0,31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
static uint32_t DayOfCurrentMonthLeap[] = {0,0,31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
static uint32_t s_RTC_Second = 0;


/****************************************************************************
*
*  Function: RTC_GetTime
*
*  Purpose :   get current time
*
*  Parms Passed   :  pointer to time struct
*
*  Returns        :   success: TRUE  Fail: FALSE
*
*  Description    :   using second variable to calculate current time
*
****************************************************************************/
uint8_t RTC_GetTime(RTC_stTime *ptrRTC)
{
    if(ptrRTC == NULL)
    {
        return FALSE;
    }
    
    RTC_GetTimeFromSecond(s_RTC_Second, ptrRTC);
    
    return TRUE;
}
/****************************************************************************
*
*  Function: RTC_SetTime
*
*  Purpose :   set current time
*
*  Parms Passed   :  pointer to time struct
*
*  Returns        :   success: TRUE  Fail: FALSE
*
*  Description    :   using time struct to set second variable
*
****************************************************************************/
uint8_t RTC_SetTime(RTC_stTime *ptrRTC)
{
    if(ptrRTC == NULL)
    {
        return FALSE;
    }
    
    if(RTC_IsValidDateTime(ptrRTC) == FALSE)
    return FALSE;
    
    s_RTC_Second = RTC_GetSecondFromTime(ptrRTC);
    
    return TRUE;
}

/****************************************************************************
*
*  Function: RTC_SetTime
*
*  Purpose :   set current time
*
*  Parms Passed   :  pointer to time struct
*
*  Returns        :   success: TRUE  Fail: FALSE
*
*  Description    :   using time struct to set second variable
*
****************************************************************************/
void RTC_SetTimeToDefault(void)
{
    /* 2018/8/21 9:26:14 */
    RTC_stTime RTCDefault = 
    {
        2018,
        8,
        21,
        0,
        0,
        0
    };
    
    RTC_SetTime(&RTCDefault);
}

/****************************************************************************
*
*  Function: RTC_ConfigInit
*
*  Purpose :    Initial RTC
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    :   
*
****************************************************************************/
void RTC_ConfigInit(void)
{
    RTC_SetTimeToDefault();
}
/****************************************************************************
*
*  Function: RTC_Timer1sCallback
*
*  Purpose :  Update second counter
*
*  Parms Passed   :   Nothing
*
*  Returns        :   Nothing
*
*  Description    : This function must be called every 1s
*
****************************************************************************/
void RTC_Timer1sCallback(void)
{
    ++s_RTC_Second;
}
/****************************************************************************
*
*  Function: RTC_GetSecondFromTime
*
*  Purpose :  get seconds since base time from RTC time struct
*
*  Parms Passed   :   pointer to time struct
*
*  Returns        :   seconds since base time
*
*  Description    :
*
****************************************************************************/
uint32_t RTC_GetSecondFromTime(RTC_stTime *ptrRTC)
{
    uint32_t year;
    uint32_t fouryear;
    uint32_t second;
    uint32_t *ptr;

    year = (uint32_t)((ptrRTC->u16year - (uint16_t)RTC_BASE_YEAR)&0x0000FFFF);
    fouryear = year /4;
    second = fouryear * RTC_DAY_OF_FOURYEAR;

    if(year/100) /*no leap year every 100 years*/
    {
        second -= year/100;
    }

    if(year/400)
    {
        second += year/400;
    }

    year %= 4;
    second += year * RTC_DAY_OF_NORMALYEAR;

    ptr = DayOfCurrentMonthNormal;

    if((year == 3) && (ptrRTC->u8month > 2) && RTC_IsLeapYear(ptrRTC->u16year))
    {
        ptr = DayOfCurrentMonthLeap;
    }

    second += ptr[ptrRTC->u8month];
    second += ptrRTC->u8date - 1;
    second *= 24;
    second += ptrRTC->u8hour;
    second *= 60;
    second += ptrRTC->u8minute;
    second *= 60;
    second += ptrRTC->u8second;

    return second;
}

/****************************************************************************
*
*  Function: RTC_GetTimeFromSecond
*
*  Purpose :  get RTC time struct from seconds since base time
*
*  Parms Passed   :  u32Sec: seconds since base time 
*                            ptrRTC:pointer to time struct
*
*  Returns        :   nothing
*
*  Description    :
*
****************************************************************************/
void RTC_GetTimeFromSecond(uint32_t u32Sec, RTC_stTime *ptrRTC)
{
    uint16_t year,month;
    uint32_t days,*ptr;

    days = u32Sec / RTC_SECOND_OF_DAY; /*days*/

    u32Sec = u32Sec % RTC_SECOND_OF_DAY;

    ptrRTC->u16year = RTC_BASE_YEAR;

    year = days /RTC_DAY_OF_FOURYEAR;
    year *= 4;

    ptrRTC->u16year += year;

    days %= RTC_DAY_OF_FOURYEAR;

    if(year/100)
    {
        days += year/100;
    }

    if(year/400)
    {
        days -= year/400;
    }

    year = days / RTC_DAY_OF_NORMALYEAR;

    ptrRTC->u16year += year;	 /* get year now */

    days %= RTC_DAY_OF_NORMALYEAR;

    ptr = DayOfCurrentMonthNormal;

    if((year == 3) && RTC_IsLeapYear(ptrRTC->u16year))
    {
        ptr = DayOfCurrentMonthLeap;
    }

    for(month=12;month>1;month--) /* calculate month */
    {
        if(days >= ptr[month])
        {
            days -= ptr[month];
            break;
        }
    }

    ptrRTC->u8month = month;
    ptrRTC->u8date = days+1;
    ptrRTC->u8hour = u32Sec / RTC_SECOND_OF_HOUR;

    u32Sec %= RTC_SECOND_OF_HOUR;
    ptrRTC->u8minute = u32Sec / RTC_SECOND_OF_MINUTE;
    ptrRTC->u8second = u32Sec % RTC_SECOND_OF_MINUTE;
}


/****************************************************************************
*
*  Function: RTC_IsLeapYear
*
*  Purpose :  judge specified year is a leap year or not
*
*  Parms Passed   :  year to be judged
*
*  Returns        :   TRUE: is a leap year  FALSE: not a leap year
*
*  Description    :
*
****************************************************************************/
static uint8_t RTC_IsLeapYear (uint16_t u16year)
{
    if ((!(u16year%4) && (u16year%100)) || !(u16year%400))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/****************************************************************************
*
*  Function: RTC_IsValidDateTime
*
*  Purpose :  judge specified time is valid time or not
*
*  Parms Passed   :  time to be judged
*
*  Returns        :   TRUE: is a valid time  FALSE: not a valid time
*
*  Description    :
*
****************************************************************************/
static uint8_t RTC_IsValidDateTime(RTC_stTime *ptrRTC)
{
    if(ptrRTC->u16year < RTC_YEAR_MIN || ptrRTC->u16year > RTC_YEAR_MAX)
    {
        return FALSE;
    }

    /* check month */
    if((ptrRTC->u8month < 1) || (ptrRTC->u8month > 12))
    {
        return FALSE;
    }

    /* check day */
    if((ptrRTC->u8date < 1) || (ptrRTC->u8date > 31))
    {
        return FALSE;
    }

    /*check solar month or not*/
    if( ptrRTC->u8date > DayOfOneMonthValMAX[ptrRTC->u8month])
    {
        /*leap year*/
        if(ptrRTC->u8month == 2)
        {
            if (RTC_IsLeapYear( ptrRTC->u16year))
            {
                if (ptrRTC->u8date > 29)
                {
                    return FALSE;
                }
            }
            else if (ptrRTC->u8date > 28)
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }

    /* check hour minute second */
    if ((ptrRTC->u8hour > 23) || (ptrRTC->u8minute > 59) || (ptrRTC->u8second > 59))
    {
        return FALSE;
    }

    return TRUE;
}


#ifdef RUN_IN_VC
void RTC_TestRtcModule(void)
{
    FILE* fpRtcFile;
    uint32_t timer_1s = 0;
    
    RTC_stTime stRtcTime = {0};
    
    fpRtcFile = fopen("RtcLog.txt", "w");
    
    RTC_ConfigInit();
    
    while(timer_1s++ < 36000)
    {
        RTC_Timer1sCallback();
        RTC_GetTime(&stRtcTime);
        fprintf(fpRtcFile, "RTC: %d-", stRtcTime.u16year);
        fprintf(fpRtcFile, "%d-", stRtcTime.u8month);
        fprintf(fpRtcFile, "%d", stRtcTime.u8date);
        fprintf(fpRtcFile, " %d:", stRtcTime.u8hour);
        fprintf(fpRtcFile, " %d:", stRtcTime.u8minute);
        fprintf(fpRtcFile, " %d\r\n", stRtcTime.u8second);
    }

    fclose(fpRtcFile);
}
#endif
