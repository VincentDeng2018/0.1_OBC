/******************************************************************************************************
*            PWM_Globals.h
******************************************************************************************************
*
******************************************************************************************************
******************************************************************************************************
*    FILE NAME: PWM_Globals.h
*
*    DESCRIPTION: Header file for task define
*
*    ORIGINATOR: DYH
*
*    DATE: 2018/5/24 21:48:19
*
*             Version   Author     Modification                            Date
*    HISTORY:  V1.0      DYH       Initial Version                      2018/5/24 21:48:23
******************************************************************************************************/

#ifndef _PWM_GLOBALS_H
#define _PWM_GLOBALS_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "PWM_Config.h"
/**********************************************************************************************
* Macros
**********************************************************************************************/
#define I_PWM   0u
#define U_PWM   1u

#define PWM_VAL_MAX     9600
#define PWM_STEP_VAL    (PWM_VAL_MAX / 100)
#define PWM_CNT_0_1_DUTY    (PWM_VAL_MAX / 1000)

/**********************************************************************************************
* Global Functions
**********************************************************************************************/
extern void PWM_Initial(void);
extern void PWM_IRefUpdate(uint16_t dutyX10);
extern void PWM_URefUpdate(uint16_t dutyX10);
extern uint16_t PWM_GetPwmDuty(uint8_t PwmChannel);

#endif