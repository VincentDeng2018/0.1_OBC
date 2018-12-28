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
#define BATT_ABS_UV_ACTIVE_CNT    60
#define BATT_UV_ACTIVE_CNT        10
#define BATT_OV_ACTIVE_CNT        10
#define FAN_FAIL_ACTIVE_CNT       10
#define RELAY_FAIL_ACTIVE_CNT     10
#define BATT_OT_ACTIVE_CNT        10
#define BATT_UT_ACTIVE_CNT        10
#define BATT_OC_ACTIVE_CNT        10
#define CHGBOARD_OT_ACTIVE_CNT    10
#define AC_LOW_ACTIVE_CNT         10
#define AC_OFF_ACTIVE_CNT         10
#define CONN_OK_ACTIVE_CNT        10
#define CONN_REVERSE_ACTIVE_CNT   10
/************************* Signal debounce detection counter end ****************************/



/************************* Batt And Charger settings start ****************************/
#define PRECHARGE_DUTY_X100   10  // ?
#define T_CHAGE_FILTER_TIME_MS   10000u
#define T_DELAY_DETECTION_MS    5000u
#define U_BATT_UV_MV            9000
#define U_BATT_UV_RESUME_MV     10000

#define U_BATT_OV_MV            16000
#define U_BATT_OV_RESUME_MV    15000

#define U_ATT_ABSUV_MV          7000u
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
