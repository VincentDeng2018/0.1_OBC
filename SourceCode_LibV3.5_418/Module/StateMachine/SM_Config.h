/******************************************************************************************************
*            SM_Config.h
******************************************************************************************************
******************************************************************************************************
*
* THIS INFORMATION IS PROPRIETARY TO BYD Corporation
*
******************************************************************************************************
*
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: SM_Config.h
*
*    DESCRIPTION: define all the state-machine configuration parameters. Refer product specification
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/4/19 13:21:54
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                       2018/4/19 13:21:56
******************************************************************************************************/

#ifndef _SM_CONFIG_H
#define _SM_CONFIG_H

#include <stdint.h>

/**********************************************************************************************
* Macros
**********************************************************************************************/
/************************* Temperature limitation settings start ****************************/
/*Over temperature, unit is 0.1mC */
#define CHG_BOARD_OT_100MC            900
#define CHG_BOARD_OT_RESUME_100MC     850

#define BATT_OT_100MC                 900
#define BATT_OT_RESUME_100MC          850

#define BATT_UT_100MC                (-100)
#define BATT_UT_RESUME_100MC          0
/************************* Temperature limitation settings end ****************************/


/************************* Signal debounce detection counter ****************************/
#define BATT_ABS_UV_ACTIVE_CNT    60u
#define BATT_UV_ACTIVE_CNT        10u
#define BATT_OV_ACTIVE_CNT        10u
#define FAN_FAIL_ACTIVE_CNT       10u
#define RELAY_FAIL_ACTIVE_CNT     10u
#define BATT_OT_ACTIVE_CNT        10u
#define BATT_UT_ACTIVE_CNT        10u
#define BATT_OC_ACTIVE_CNT        10u
#define CHGBOARD_OT_ACTIVE_CNT    10u
#define AC_LOW_ACTIVE_CNT         10u
#define AC_OFF_ACTIVE_CNT         10u
#define CONN_OK_ACTIVE_CNT        10u
#define CONN_REVERSE_ACTIVE_CNT   10u
/************************* Signal debounce detection counter end ****************************/



/************************* Batt And Charger settings start ****************************/
#define PRECHARGE_DUTY_X100   10  // ?
#define T_CHAGE_FILTER_TIME_MS   10000u
#define T_DELAY_DETECTION_MS    5000u
/************************* Charge settings end ****************************/


static inline uint32_t CoverterMinToMs(uint16_t min)
{
    return ((uint32_t)min * 60000);
}


/************************* Sleeping settings *******************************/


/*  Legacy data and structure */
#define VOLTAGE_TYPE_NUM                4
#define CURRENT_TYPE_NUM                6

#define VOLT_FILT_TM			6
#define CURR_FILT_TM			6

/**********************************************************************************************
* Global Functions
**********************************************************************************************/


#endif /* _VERSION_H */
